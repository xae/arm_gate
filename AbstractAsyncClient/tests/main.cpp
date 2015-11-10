#include "testpayloadpacket.cpp"
#include "testabstractasyncclient.cpp"

#include <QCoreApplication>

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);
   Q_UNUSED(app)
   int status = 0;
   auto ASSERT_TEST = [&status, argc, argv](QObject* obj) {
        status |= QTest::qExec(obj, argc, argv);
        delete obj;
      };
   ASSERT_TEST(new TestAbstractAsyncClient());
   ASSERT_TEST(new TestPayloadPacket());
   return status;
}
