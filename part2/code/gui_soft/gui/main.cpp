#include <iostream>

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "controller.h"


#ifdef CONFIG_GUITEST
#include "../guitests/guitest.h"
#endif // CONFIG_GUITEST

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    // Get the Root Context to set the controller
    auto rootContext = engine.rootContext();
    auto controller = new Controller();
    rootContext->setContextProperty("controller", controller);

    auto rootObject = engine.rootObjects().at(0);
    if (rootObject == nullptr) {
        std::cout << "Error: Can not get the main window QML object" << std::endl;
        return -1;
    }

    if (!QObject::connect(controller, SIGNAL(changeStatus(QString)), rootObject, SIGNAL(setStatus(QString)))) {
        std::cout << "Error with signal-slot connection. Exiting" << std::endl;
        return -1;
    }

    if (!QObject::connect(controller, SIGNAL(getResult(QString)), rootObject, SIGNAL(setResult(QString)))) {
        std::cout << "Error with signal-slot connection. Exiting" << std::endl;
        return -1;
    }

#ifdef CONFIG_GUITEST

    controller->engine = &engine;

    // Instantiate and run tests
    //GuiTests tests(mainWindow->getWindow());
    auto mainWindow = qobject_cast<QQuickWindow*>(engine.rootObjects().at(0));

    SpixGTest tests(controller, mainWindow, argc, argv);
    auto bot = new spix::QtQmlBot();
    bot->runTestServer(tests);
#endif // CONFIG_GUITEST

    int result = app.exec();


#ifdef CONFIG_GUITEST
    result = tests.testResult();
    // We could get rid of the next line if we want to automatically quit
    app.exec();
#endif // CONFIG_GUITEST

    return result;
}
