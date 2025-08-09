#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "translation_controller.hpp"
#include "config/project_description.h"

#include "avl_tree.hpp"

auto main(int argc, char* argv[]) -> int {
    QGuiApplication app(argc, argv);

    QGuiApplication::setApplicationName(project_name);
    QGuiApplication::setApplicationVersion(project_version);
    QGuiApplication::setOrganizationDomain(project_homepage_url);
    QGuiApplication::setOrganizationName(author);

    TranslationController translation_controller;
    AVLTree tree_view_model;

    QQmlApplicationEngine engine;

    translation_controller.setEngine(&engine);

    engine.rootContext()->setContextProperty("translation_controller", &translation_controller);
    engine.rootContext()->setContextProperty("tree_view_model", &tree_view_model);

    const QUrl url(QStringLiteral("qrc:/app/qml/ui/Main.qml"));

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject* obj, const QUrl& obj_url){
        if (obj == nullptr && url == obj_url) {
            QCoreApplication::exit(-1);
        }
    }, Qt::QueuedConnection);

    engine.load(url);

    translation_controller.initializeSystemLocale();

    return QGuiApplication::exec();
}
