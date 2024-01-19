
#include <cstdint>

#include <gtest/gtest.h>

#include <QSignalSpy>

#include "processingclient.h"

void do_test(unsigned a, unsigned b, unsigned c, unsigned expected) {
    MathComputerProxyClient client;

    qRegisterMetaType<uint32_t>("uint32_t");

    QSignalSpy spy(&client, SIGNAL(resultReady(uint32_t)));

    client.connectToHost("localhost", 12345);
    client.compute(a, b, c);

    spy.wait(5000);

    ASSERT_EQ(spy.count(), 1);
    QList<QVariant> arguments = spy.takeFirst(); // take the first signal

    bool ok = true;
    EXPECT_EQ(arguments.at(0).toULongLong(&ok), expected); // verify the first argument
}

TEST(Computing, simpletest) {
    do_test(2, 4, 5, 28);
}

TEST(Computing, val0test) {
    do_test(0, 0, 0, 0);
}

TEST(Computing, val0btest) {
    do_test(0, 42, 0, 0);
}

TEST(Computing, val0ctest) {
    do_test(0, 0, 42, 0);
}

TEST(Computing, valMax) {
    do_test(30, 7707, 5, 65535);
}

// This test will give 8200000 in simulation and 8000 in reality
// This is because the FPGA actually does the simulation in 16 bits
// In a way, the simulation is a bit 
TEST(Computing, valOOB) {
    do_test(200, 400, 500, 8000);
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    testing::InitGoogleTest(&argc, argv);
    std::cout << "The last test should fail if it is in simulation mode !" << std::endl;
    return RUN_ALL_TESTS();
}
