#include "guitest.h"

//@@license@@

#include <QQuickWindow>

#include <QTest>
#include <QQmlApplicationEngine>

#include <gtest/gtest.h>

#include <Spix/QtQmlBot.h>
//#include <Spix/Events/KeyCodes.h>

#include "guitest.h"
#include "controller.h"

class SpixGTest;
SpixGTest* srv;


int waitTime1 = 1;
int waitTimeLong = 10;

SpixGTest::SpixGTest(Controller *mainWindowController, QQuickWindow *window, int argc, char* argv[])
{
    m_controller = mainWindowController;
    m_window = window;
    m_argc = argc;
    m_argv = argv;
}

int SpixGTest::testResult() { return m_result.load(); }

void SpixGTest::executeTest()
{
    srv = this;
    ::testing::InitGoogleTest(&m_argc, m_argv);
    auto testResult = RUN_ALL_TESTS();
    m_result.store(testResult);

    srv->quit();
}

bool SpixGTest::synchronize()
{
    return existsAndVisible(spix::ItemPath("paththatdonotexist"));
}


QObject *SpixGTest::getObjectByName(QObject *root, std::string name)
{
    for(auto child : root->children()) {
        if (child->objectName().toStdString() == name) {
            return child;
        }
    }

    for(auto child : root->children()) {
        auto result = getObjectByName(child, name);
        if (result != nullptr) {
            return result;
        }
    }
    return nullptr;
}

void SpixGTest::mouseClickIfPathOk(std::string clickPath)
{
    if (srv->existsAndVisible(spix::ItemPath(clickPath)))
    {
        std::cout << "Path exists : " << clickPath << std::endl;

        srv->mouseClick(spix::ItemPath(clickPath));
        srv->synchronize();
    }
    else std::cout << "Path does not exists : " << clickPath << std::endl;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-

void SpixGTest::findObjectAndSetValue(QString objectName, int propertyInput)
{
    // retrieves object by name and puts it into an item to set or get its properties (value)
    auto item = srv->m_controller->getRootObject()->findChild<QObject*>(objectName);
    if (item != (0x0))
        item->setProperty("value", propertyInput);
    else std::cout << "Item not found !" << std::endl;

    srv->waitPeriod(waitTime1);
}

