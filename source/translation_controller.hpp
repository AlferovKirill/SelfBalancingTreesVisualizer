#pragma once

#include <QLocale>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QTranslator>

class TranslationController : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString currentLanguage READ currentLanguage NOTIFY languageChanged)
    Q_PROPERTY(QStringList supportedLanguagesList READ getSupportedLanguagesList)

public:
    explicit TranslationController(QQmlApplicationEngine& engine, QObject* parent = nullptr);

    /**
     * @brief Sets the application language
     * @param language Language code (for example, "en", "ru")
     */
    Q_INVOKABLE void setLanguage(const QString& language);

    auto currentLanguage() const -> QString;
    void initializeSystemLocale();

    static auto getSystemLanguageCode() -> QString;
    static auto getSupportedLanguagesList() -> QStringList;

signals:
    void languageChanged();

private:
    QQmlApplicationEngine& m_engine;

    QTranslator m_translator;
    QString m_current_language;

    auto loadTranslation(const QString& language) -> bool;
};
