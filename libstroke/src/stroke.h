#ifndef STROKE_H
#define STROKE_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QQmlExtensionPlugin>

class StrokeRecognition : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList candidates READ candidates NOTIFY candidatesChanged)

public:
    explicit StrokeRecognition(QObject *parent = nullptr);

    Q_INVOKABLE QStringList recognize(const QString &strokeSequence);
    QStringList candidates() const;

signals:
    void candidatesChanged();

private:
    void initializeStrokeMap();
    QStringList matchStroke(const QString &strokeSequence);

    QStringList m_candidates;
    QMap<QString, QStringList> m_strokeMap;
};

class QmlStrokePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri) override;
};

#endif // STROKE_H
