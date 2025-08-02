#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "translation_controller.hpp"

auto main(int argc, char *argv[]) -> int {
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    TranslationController translation_controller(engine);
    engine.rootContext()->setContextProperty("translation_controller", &translation_controller);

    const QUrl url(QStringLiteral("qrc:/app/qml/ui/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &obj_url) {
        if (obj == nullptr && url == obj_url) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    translation_controller.initializeSystemLocale();

    return QGuiApplication::exec();
}
