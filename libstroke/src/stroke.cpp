#include "stroke.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSet>
#include <qqml.h>
#include <algorithm>

StrokeRecognition::StrokeRecognition(QObject *parent)
    : QObject(parent)
{
    loadDictionary();
}

QStringList StrokeRecognition::recognize(const QString &strokeSequence)
{
    matchStroke(strokeSequence);
    emit candidatesChanged();
    return m_candidates;
}

QStringList StrokeRecognition::candidates() const
{
    return m_candidates;
}

QString StrokeRecognition::pick(int index)
{
    if (index < 0 || index >= m_candidates.size())
        return QString();
    return m_candidates.at(index);
}

void StrokeRecognition::loadDictionary()
{
    QFile f(":/data/dict.txt");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "StrokeRecognition: cannot open dictionary resource";
        return;
    }
    QTextStream in(&f);
    in.setCodec("UTF-8");
    while (!in.atEnd()) {
        QString line = in.readLine();
        if (line.isEmpty() || line.startsWith('#'))
            continue;
        QStringList parts = line.split('\t');
        if (parts.size() < 3)
            continue;
        Entry e;
        e.ch = parts[0];
        e.code = parts[1];
        e.freq = parts[2].toInt();
        if (e.ch.isEmpty() || e.code.isEmpty() || e.freq <= 0)
            continue;
        m_entries.append(e);
    }
    std::sort(m_entries.begin(), m_entries.end(),
              [](const Entry &a, const Entry &b) {
                  if (a.freq != b.freq)
                      return a.freq > b.freq;
                  return a.code < b.code;
              });
    qDebug() << "StrokeRecognition: loaded" << m_entries.size() << "entries";
}

void StrokeRecognition::matchStroke(const QString &strokeSequence)
{
    m_candidates.clear();

    if (strokeSequence.isEmpty())
        return;

    QVector<int> matchIndices;
    for (int i = 0; i < m_entries.size(); ++i) {
        const Entry &e = m_entries.at(i);
        if (e.code.startsWith(strokeSequence))
            matchIndices.append(i);
    }

    std::stable_sort(matchIndices.begin(), matchIndices.end(),
                     [this, &strokeSequence](int a, int b) {
                         const Entry &ea = m_entries.at(a);
                         const Entry &eb = m_entries.at(b);
                         bool exactA = (ea.code == strokeSequence);
                         bool exactB = (eb.code == strokeSequence);
                         if (exactA != exactB)
                             return exactA;
                         if (ea.freq != eb.freq)
                             return ea.freq > eb.freq;
                         return ea.code.length() < eb.code.length();
                     });

    QSet<QString> seen;
    for (int idx : matchIndices) {
        const QString &ch = m_entries.at(idx).ch;
        if (seen.contains(ch))
            continue;
        seen.insert(ch);
        m_candidates.append(ch);
        if (m_candidates.size() >= 30)
            break;
    }
}

void QmlStrokePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<StrokeRecognition>(uri, 1, 0, "StrokeRecognition");
}
