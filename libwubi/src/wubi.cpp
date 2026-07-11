#include "wubi.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QSet>
#include <qqml.h>
#include <algorithm>

WubiRecognition::WubiRecognition(QObject *parent)
    : QObject(parent)
    , m_version(QStringLiteral("wubi86"))
{
    loadDictionary();
}

void WubiRecognition::setVersion(const QString &v)
{
    QString normalized = v.toLower();
    if (normalized != QStringLiteral("wubi86")
            && normalized != QStringLiteral("wubi98")
            && normalized != QStringLiteral("wubixsj")) {
        normalized = QStringLiteral("wubi86");
    }
    if (normalized == m_version)
        return;
    m_version = normalized;
    m_chars.clear();
    m_phrases.clear();
    loadDictionary();
    matchWubi();
    emit versionChanged();
}

void WubiRecognition::input(const QString &code)
{
    if (code.isEmpty())
        return;
    QString cleaned;
    for (int i = 0; i < code.length(); i++) {
        QChar c = code.at(i);
        if (c.isLetter())
            cleaned += c.toLower();
    }
    if (cleaned.isEmpty())
        return;
    m_currentInput += cleaned;
    matchWubi();
    emit currentInputChanged();
}

void WubiRecognition::recognize(const QString &code)
{
    QString cleaned;
    for (int i = 0; i < code.length(); i++) {
        QChar c = code.at(i);
        if (c.isLetter())
            cleaned += c.toLower();
    }
    m_currentInput = cleaned;
    matchWubi();
    emit currentInputChanged();
}

void WubiRecognition::backspace()
{
    if (m_currentInput.isEmpty())
        return;
    m_currentInput.chop(1);
    matchWubi();
    emit currentInputChanged();
}

void WubiRecognition::clear()
{
    if (m_currentInput.isEmpty() && m_candidates.isEmpty())
        return;
    m_currentInput.clear();
    m_candidates.clear();
    emit currentInputChanged();
    emit candidatesChanged();
}

QString WubiRecognition::pick(int index)
{
    if (index < 0 || index >= m_candidates.size())
        return QString();
    QString text = m_candidates.at(index);
    clear();
    return text;
}

void WubiRecognition::loadDictionary()
{
    auto loadFile = [](const QString &path, QVector<Entry> &out) {
        QFile f(path);
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "WubiRecognition: cannot open" << path;
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
            e.code = parts[1].toLower();
            e.freq = parts[2].toInt();
            if (e.ch.isEmpty() || e.code.isEmpty() || e.freq <= 0)
                continue;
            out.append(e);
        }
    };

    QString dictFile;
    QString phraseFile;
    if (m_version == QStringLiteral("wubi98")) {
        dictFile = QStringLiteral(":/data/dict98.txt");
        phraseFile = QStringLiteral(":/data/phrases98.txt");
    } else if (m_version == QStringLiteral("wubixsj")) {
        dictFile = QStringLiteral(":/data/dictxsj.txt");
        phraseFile = QStringLiteral(":/data/phrasesxsj.txt");
    } else {
        dictFile = QStringLiteral(":/data/dict.txt");
        phraseFile = QStringLiteral(":/data/phrases.txt");
    }

    loadFile(dictFile, m_chars);
    loadFile(phraseFile, m_phrases);

    auto sortFn = [](const Entry &a, const Entry &b) {
        if (a.freq != b.freq)
            return a.freq > b.freq;
        return a.code < b.code;
    };
    std::sort(m_chars.begin(), m_chars.end(), sortFn);
    std::sort(m_phrases.begin(), m_phrases.end(), sortFn);

    qDebug() << "WubiRecognition[" << m_version << "]: loaded" << m_chars.size() << "chars," << m_phrases.size() << "phrases";
}

void WubiRecognition::matchWubi()
{
    m_candidates.clear();
    if (m_currentInput.isEmpty()) {
        emit candidatesChanged();
        return;
    }
    const QString &in = m_currentInput;
    int inLen = in.length();

    QSet<QString> seen;
    auto append = [&](const Entry &e, int penalty) {
        int effFreq = e.freq - penalty;
        if (effFreq <= 0)
            return;
        if (m_candidates.size() >= 30)
            return;
        if (seen.contains(e.ch))
            return;
        seen.insert(e.ch);
        m_candidates.append(e.ch);
        Q_UNUSED(effFreq);
    };

    // phrases first (longer matches are more interesting)
    for (const Entry &e : m_phrases) {
        if (e.code.startsWith(in) && inLen >= 2) {
            int penalty = (e.code.length() - inLen) * 5;
            if (inLen == e.code.length())
                penalty -= 50;
            append(e, penalty);
        }
    }

    // single chars
    for (const Entry &e : m_chars) {
        if (e.code.startsWith(in)) {
            int penalty = (e.code.length() - inLen) * 10;
            if (inLen == e.code.length())
                penalty -= 100;
            append(e, penalty);
        }
    }

    emit candidatesChanged();
}

void QmlWubiPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<WubiRecognition>(uri, 1, 0, "WubiRecognition");
}
