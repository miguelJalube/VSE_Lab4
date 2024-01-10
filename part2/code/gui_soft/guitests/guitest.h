#ifndef GUITEST_H
#define GUITEST_H


#include <QTest>

#include <Spix/QtQmlBot.h>

#include <QQuickWindow>

class Controller;

class SpixGTest : public spix::TestServer {
public:
    SpixGTest(Controller *controller, QQuickWindow *window, int argc, char* argv[]);
    int testResult();

    QQuickWindow* m_window;

    Controller* m_controller;

    QObject *getObjectByName(QObject *root, std::string name);

    bool synchronize();

    void waitPeriod(unsigned int nbPeriods = 1)
    {
        wait(m_period * nbPeriods);
        synchronize();
    }

    void waitForSync();

    void findObjectAndSetValue(QString objectName, int inputProperty);

    void mouseClickIfPathOk(std::string clickPath);


protected:
    int m_argc;
    char** m_argv;
    std::atomic<int> m_result {0};

    std::chrono::milliseconds m_period{std::chrono::milliseconds(500)};



    void executeTest() override;
};


#define CHECKSPIXERRORS \
{auto spixErrors = srv->getErrors(); \
        EXPECT_EQ(spixErrors.size(), 0); \
        for (const auto &spixError : spixErrors) { \
            EXPECT_EQ(spixError, ""); \
    }} \

#endif // GUITEST_H
