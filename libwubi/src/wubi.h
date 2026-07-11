#ifndef WUBI_H
#define WUBI_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QQmlExtensionPlugin>

class WubiRecognition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentInput READ currentInput NOTIFY currentInputChanged)
    Q_PROPERTY(QStringList candidates READ candidates NOTIFY candidatesChanged)
    Q_PROPERTY(int candidateCount READ candidateCount NOTIFY candidatesChanged)
    Q_PROPERTY(QString version READ version WRITE setVersion NOTIFY versionChanged)

public:
    explicit WubiRecognition(QObject *parent = nullptr);

    QString currentInput() const { return m_currentInput; }
    QStringList candidates() const { return m_candidates; }
    int candidateCount() const { return m_candidates.size(); }
    QString version() const { return m_version; }
    void setVersion(const QString &v);

    Q_INVOKABLE void input(const QString &code);
    Q_INVOKABLE void recognize(const QString &code);
    Q_INVOKABLE void backspace();
    Q_INVOKABLE void clear();
    Q_INVOKABLE QString pick(int index);

signals:
    void currentInputChanged();
    void candidatesChanged();
    void versionChanged();

private:
    struct Entry {
        QString ch;
        QString code;
        int freq;
    };

    void loadDictionary();
    void matchWubi();

    QVector<Entry> m_chars;
    QVector<Entry> m_phrases;
    QString m_currentInput;
    QStringList m_candidates;
    QString m_version;
};

class QmlWubiPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif // WUBI_H
