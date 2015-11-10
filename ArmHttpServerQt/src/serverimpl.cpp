#include "serverimpl.h"

#include <qhttpresponse.h>
#include <qhttprequest.h>

#include <QTimer>
#include <QThread>
#include <QStringList>
#include <QApplication>
#include <QXmlStreamWriter>
#include <QJsonDocument>
#include <QJsonObject>

#include <QFile>


ServerImpl::ServerImpl(const quint16 port, QObject *parent) :
    QObject(parent),
    mServer(new QHttpServer(this)),
    mPort(port),
    mTimer(new QTimer(this))
{
    connect(mServer, &QHttpServer::newRequest,
            this, &ServerImpl::handleRequest);
    connect(mTimer, &QTimer::timeout,
            this, &ServerImpl::checkFiles);
}

bool ServerImpl::listen()
{
    bool retVal = false;
    if (mServer && mTimer) {
        if (mServer->listen(mPort)) {
            mTimer->start(60000);
            retVal = true;
        }
    }
    emit serverStatus(retVal);
    return retVal;
}

void ServerImpl::stop()
{
        mTimer->stop();
        mServer->close();
}

void ServerImpl::handleRegisterFiles(QHttpRequest *req, QHttpResponse *resp)
{
    req->storeBody();
    connect(req, &QHttpRequest::end,  [this, req, resp]() {
        QByteArray jsonTextBin = req->body();
        QJsonObject json = QJsonDocument::fromJson(jsonTextBin).object();
        QByteArray content("ok");
        if (!json.isEmpty()) {
            FileInfo fi;
            fi.filename = json["filename"].toString();
            fi.filepath = json["filepath"].toString();
            fi.hash = json["md5"].toString();
            fi.ttl = json["ttl"].toInt();
            fi.registeredTime = QDateTime::currentDateTime();

            if (!fi.filename.isEmpty() && !fi.hash.isEmpty() && QFile::exists(fi.filepath)) {
                mRegisteredFiles[fi.hash] = fi;
            }
        }

        resp->setHeader("Content-Length", QString::number(content.length()));
        setContentType(resp, "text/plain");
        resp->writeHead(200); // everything is OK
        resp->write(content);
        resp->end();
    });
}

void ServerImpl::handleGetFile(QHttpResponse *resp, const QString &fileHash)
{

    QFile file(mRegisteredFiles.value(fileHash).filepath);
    if (!file.fileName().isEmpty() && file.open(QIODevice::ReadOnly)) {
        QByteArray content = file.readAll();
        resp->setHeader("Content-Length", QString::number(content.length()));
        setContentType(resp, "application/octet-stream");
        resp->writeHead(200);
        resp->write(content);
        resp->end();
    } else {
        resp->writeHead(404);
        resp->end();
    }
}

void ServerImpl::handleRoot(QHttpResponse *resp)
{
    QString templateFile(":/resources/template.html");
    QFile f(templateFile);
    f.open(QIODevice::ReadOnly);
    QString html = QString::fromUtf8(f.readAll());

    QString data;

    {
        QXmlStreamWriter writer(&data);
        writer.setAutoFormatting(true);

        QHashIterator<QString, FileInfo> it(mRegisteredFiles);
        while (it.hasNext()) {
            it.next();
            writer.writeStartElement("tr");

            writer.writeTextElement("td", it.value().hash);
            writer.writeTextElement("td", it.value().filename);
            writer.writeTextElement("td", it.value().filepath);
            writer.writeTextElement("td", it.value().registeredTime.toString());
            writer.writeTextElement("td", QString::number(it.value().ttl));

            writer.writeEndElement();
        }

    }
    QByteArray content = html.arg(data).toUtf8();


    resp->setHeader("Content-Length", QString::number(content.length()));
    resp->setHeader("Content-Type", "text/html");
    resp->writeHead(200); // everything is OK
    resp->write(content);
    resp->end();
}

void ServerImpl::setContentType(QHttpResponse *resp, const QString &contentType)
{
    resp->setHeader("Content-Type", contentType);
}


void ServerImpl::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
#if 0
    {
      "filename": "filename",
      "filepath": "filepath",
      "ttl": "10",
      "md5": "12312301092343295023942345234"
    }
#endif
    QByteArray content = req->body();
    content += '\n' + req->url().toString().toUtf8() + '\n';
    content += req->methodString().toUtf8();
    QStringList url = req->url().toString().split("/",QString::SkipEmptyParts);
    bool processed = false;
    if (url.size()) {
        if (url.first().toLower() == "register") {
            handleRegisterFiles(req, resp);
            processed = true;
        } else if (url.first().toLower() == "get" && url.size() > 1) {
            handleGetFile(resp, url.at(1));
            processed = true;
        }
    } else {
        handleRoot(resp);
        processed = true;
    }

    if (!processed) {
        content = "error";
        resp->setHeader("Content-Length", QString::number(content.length()));
        resp->writeHead(404); // something is wrong
        resp->write(content);
        resp->end();
    }
}

void ServerImpl::checkFiles()
{
    QMutableHashIterator <QString, FileInfo> it(mRegisteredFiles);
    QDateTime currentDateTime = QDateTime::currentDateTime();
    while (it.hasNext()) {
        it.next();
        if (it.value().ttl) {
            if (it.value().registeredTime.secsTo(currentDateTime) >= it.value().ttl * 60) {
                it.remove();
            }
        }
    }
}
