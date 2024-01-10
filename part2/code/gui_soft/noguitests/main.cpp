
#include <cstdint>

#include <gtest/gtest.h>

#include <QSignalSpy>

#include "processingclient.h"

TEST(Computing, simpletest) {
    MathComputerProxyClient client;

    qRegisterMetaType<uint32_t>("uint32_t");

    QSignalSpy spy(&client, SIGNAL(resultReady(uint32_t)));

    client.connectToHost("localhost", 12345);
    client.compute(1, 2, 3);

    spy.wait(5000);

    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal

    bool ok = true;
    EXPECT_EQ(arguments.at(0).toULongLong(&ok), 7); // verify the first argument

}


int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
