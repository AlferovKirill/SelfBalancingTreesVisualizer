#include <QCoreApplication>
#include <QDebug>

#include "translation_controller.hpp"

#include "config/translations.h"

TranslationController::TranslationController(QObject* parent)
    : QObject(parent)
    , m_engine(nullptr) {}

void TranslationController::setLanguage(const QString& language) {
    if (m_current_language == language) {
        return;
    }

    if (loadTranslation(language)) {
        m_current_language = language;
        emit languageChanged();

        if (m_engine != nullptr) {
            m_engine->retranslate();
        }
    }
}

auto TranslationController::setEngine(QQmlApplicationEngine* engine) -> void {
    m_engine = engine;
}

auto TranslationController::currentLanguage() const -> QString {
    return m_current_language;
}

auto TranslationController::initializeSystemLocale() -> void {
    QString system_lang = getSystemLanguageCode();

    if (getSupportedLanguagesList().contains(system_lang)) {
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
