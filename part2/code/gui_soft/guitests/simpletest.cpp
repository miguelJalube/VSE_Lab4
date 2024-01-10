
#include <QQuickWindow>

#include <QTest>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>

#include "guitest.h"

extern SpixGTest* srv;

TEST(SimpleTest, Test1)
{
    srv->waitPeriod();
    srv->setStringProperty(spix::ItemPath("mainWindow/textA"), "text", "4");
    srv->waitPeriod();
    srv->setStringProperty(spix::ItemPath("mainWindow/textB"), "text", "5");
    srv->waitPeriod();
    srv->setStringProperty(spix::ItemPath("mainWindow/textC"), "text", "6");
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/computeButton"));
    srv->waitPeriod();
    srv->synchronize();

    std::string resultText = srv->getStringProperty(spix::ItemPath("mainWindow/textResult"), "text");
    EXPECT_EQ(resultText, "94");


    srv->synchronize();


    srv->waitPeriod();
    srv->setStringProperty(spix::ItemPath("mainWindow/textA"), "text", "6");
    srv->waitPeriod();
    srv->setStringProperty(spix::ItemPath("mainWindow/textB"), "text", "5");
    srv->waitPeriod();
    srv->setStringProperty(spix::ItemPath("mainWindow/textC"), "text", "4");
    srv->waitPeriod();
    srv->mouseClick(spix::ItemPath("mainWindow/computeButton"));
    srv->waitPeriod();
    srv->synchronize();

    resultText = srv->getStringProperty(spix::ItemPath("mainWindow/textResult"), "text");
    EXPECT_EQ(resultText, "236");



    CHECKSPIXERRORS;
}
