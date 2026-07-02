#ifndef STROKE_H
#define STROKE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QQmlExtensionPlugin>

class StrokeRecognition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList candidates READ candidates NOTIFY candidatesChanged)
    Q_PROPERTY(int candidateCount READ candidateCount NOTIFY candidatesChanged)

public:
    explicit StrokeRecognition(QObject *parent = nullptr);

    Q_INVOKABLE QStringList recognize(const QString &strokeSequence);
    QStringList candidates() const;
    int candidateCount() const { return m_candidates.size(); }

    Q_INVOKABLE QString pick(int index);

signals:
    void candidatesChanged();

private:
    struct Entry {
        QString ch;
        QString code;
        int freq;
    };

    void loadDictionary();
    void matchStroke(const QString &strokeSequence);

    QVector<Entry> m_entries;
    QStringList m_candidates;
};

class QmlStrokePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif // STROKE_H
