#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <iostream>

#include "lib.hpp"

auto main(int argc, char *argv[]) -> int {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    const QUrl url(QStringLiteral("qrc:/app/qml/ui/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &obj_url) {
        if (obj == nullptr && url == obj_url) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.addImportPath(":/qml/");
    engine.load(url);

    library lib;

    std::cout << "lib.name = " << lib.name << "\n";

    return QGuiApplication::exec();
}
