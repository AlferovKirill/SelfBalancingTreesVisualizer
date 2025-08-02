#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QCoreApplication>

#include "translation_controller.hpp"

#include "config/translations_cmake_variables.h"

TranslationController::TranslationController(QQmlApplicationEngine& engine, QObject* parent)
    : QObject(parent)
    , m_engine(engine) {
}

void TranslationController::setLanguage(const QString& language) {
    if (m_current_language == language) {
        return;
    }

    if (loadTranslation(language)) {
        m_current_language = language;
        emit languageChanged();

        m_engine.retranslate();
    }
}

auto TranslationController::currentLanguage() const -> QString {
    return m_current_language;
}

void TranslationController::initializeSystemLocale() {
    QString system_lang = getSystemLanguageCode();

    if (supported_languages.count(system_lang.toStdString()) != 0) {
        setLanguage(system_lang);
    } else {
        setLanguage("en");
    }
}

auto TranslationController::getSystemLanguageCode() -> QString {
    QLocale system_locale = QLocale::system();
    QString lang_code = system_locale.name().split('_').first();

    qDebug() << "System locale:" << system_locale.name();
    qDebug() << "Language code:" << lang_code;

    return lang_code;
}

auto TranslationController::getSupportedLanguagesList() -> QStringList {
    QStringList result;

    for (const auto& language : supported_languages) {
        result << QString::fromStdString(language);
    }

    return result;
}

auto TranslationController::loadTranslation(const QString& language) -> bool {
    QCoreApplication::removeTranslator(&m_translator);

    QString translation_file = QString(":/translations/SelfBalancingTreesVisualizer_%1").arg(language);

    if (m_translator.load(translation_file)) {
        QCoreApplication::installTranslator(&m_translator);
        qDebug() << "Translation loaded:" << translation_file;

        return true;
    }

    qDebug() << "Failed to load translation:" << translation_file;

    return false;
}
