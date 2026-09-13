/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** Copyright (C) 2018-2019 BirdZhang
**
** This file is part of the Qt Virtual Keyboard module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "pinyindecoderservice.h"
#include "pinyinime.h"
#include "dictdef.h"
#include "userdict.h"
#include <QStandardPaths>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDateTime>
#include <QRegExp>
#include <QtCore/QLibraryInfo>
#include <QSysInfo>


using namespace ime_pinyin;

QScopedPointer<PinyinDecoderService> PinyinDecoderService::_instance;

static QString userDictionaryFilePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
            + QLatin1String("/harbour-souniaoime/pinyin/usr_dict.dat");
}

/*!
    \class QtVirtualKeyboard::PinyinDecoderService
    \internal
*/

PinyinDecoderService::PinyinDecoderService(QObject *parent) :
    QObject(parent),
    initDone(false),
    userDictionaryMTime(-1),
    userDictionarySize(-1)
{
}

PinyinDecoderService::~PinyinDecoderService()
{
    if (initDone) {
        im_close_decoder();
        initDone = false;
    }
}

PinyinDecoderService *PinyinDecoderService::getInstance()
{
    if (!_instance)
        _instance.reset(new PinyinDecoderService());
    if (!_instance->init())
        return 0;
    return _instance.data();
}

bool PinyinDecoderService::init()
{
    if (initDone)
        return true;

    QString sysDict(QString::fromLatin1(qgetenv("QT_VIRTUALKEYBOARD_PINYIN_DICTIONARY").constData()));
    QString osArch = QSysInfo::buildAbi();
    if (sysDict.isEmpty()){
        if(osArch.contains("64")){
           sysDict = "/usr/share/harbour-souniaoime/data/dict_pinyin_64.dat";
        }else{
           sysDict = "/usr/share/harbour-souniaoime/data/dict_pinyin.dat";
        }

    }
    QFileInfo usrDictInfo(userDictionaryFilePath());
    
    // 确保目录存在
    QDir().mkpath(usrDictInfo.absolutePath());

    // Keep existing learned phrases when upgrading from the old location.
    QString legacyPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation)
            + QLatin1String("/pinyin/usr_dict.dat");
    if (!usrDictInfo.exists() && QFileInfo(legacyPath).exists())
        QFile::copy(legacyPath, usrDictInfo.absoluteFilePath());
    
    // UserDict::load_dict() validates and creates the binary file itself.
    // Do not duplicate its ABI-dependent on-disk structure here.
    initDone = im_open_decoder(sysDict.toUtf8().constData(), usrDictInfo.absoluteFilePath().toUtf8().constData());
    usrDictInfo.refresh();
    userDictionaryMTime = usrDictInfo.lastModified().toMSecsSinceEpoch();
    userDictionarySize = usrDictInfo.size();
    return initDone;
}

void PinyinDecoderService::setUserDictionary(bool enabled)
{
    if (enabled == im_is_user_dictionary_enabled())
        return;
    if (enabled) {
        QFileInfo usrDictInfo(userDictionaryFilePath());
        im_init_user_dictionary(usrDictInfo.absoluteFilePath().toUtf8().constData());
    } else {
        im_init_user_dictionary(NULL);
    }
}

bool PinyinDecoderService::isUserDictionaryEnabled() const
{
    return im_is_user_dictionary_enabled();
}

void PinyinDecoderService::setLimits(int maxSpsLen, int maxHzsLen)
{
    if (maxSpsLen <= 0)
        maxSpsLen = kMaxSearchSteps - 1;
    if (maxHzsLen <= 0)
        maxHzsLen = kMaxSearchSteps;
    im_set_max_lens(size_t(maxSpsLen), size_t(maxHzsLen));
}

int PinyinDecoderService::search(const QString &spelling)
{
    reloadUserDictionaryIfChanged();
    QByteArray spellingBuf = spelling.toLatin1();
    return int(im_search(spellingBuf.constData(), spellingBuf.length()));
}

int PinyinDecoderService::deleteSearch(int pos, bool isPosInSpellingId, bool clearFixedInThisStep)
{
    if (pos <= 0)
        pos = 0;
    return int(im_delsearch(size_t(pos), isPosInSpellingId, clearFixedInThisStep));
}

void PinyinDecoderService::resetSearch()
{
    im_reset_search();
}

QString PinyinDecoderService::pinyinString(bool decoded)
{
    size_t py_len;
    const char *py = im_get_sps_str(&py_len);
    if (!decoded)
        py_len = strlen(py);

    return QString(QLatin1String(py, (int)py_len));
}

int PinyinDecoderService::pinyinStringLength(bool decoded)
{
    size_t py_len;
    const char *py = im_get_sps_str(&py_len);
    if (!decoded)
        py_len = strlen(py);
    return (int)py_len;
}

int PinyinDecoderService::getSplStart()
{
    const unsigned short *spl_start;
    return (int)im_get_spl_start_pos(spl_start);
}

QList<QString> PinyinDecoderService::spellingStartPositions()
{
    const unsigned short *spl_start;
    int len;
    // There will be len + 1 elements in the buffer when len > 0.
    len = (int)im_get_spl_start_pos(spl_start);

    QList<QString> arr;
//    arr.resize(len + 2);
//    arr[0] = len; // element 0 is used to store the length of buffer.
//    for (int i = 0; i <= len; i++)
//        arr[i + 1] = spl_start[i];

    arr.append(QString::number(len)); // element 0 is used to store the length of buffer.
    for (int i = 0; i <= len; i++)
        arr.append(QString::number(spl_start[i]));
    return arr;
}

QString PinyinDecoderService::candidateAt(int index)
{
    Q_ASSERT(index >= 0);
    QVector<QChar> candidateBuf;
    candidateBuf.resize(kMaxSearchSteps + 1);
    if (!im_get_candidate(size_t(index), (char16 *)candidateBuf.data(), candidateBuf.length() - 1))
        return QString();
    candidateBuf.last() = 0;
    return QString(candidateBuf.data());
}

QList<QString> PinyinDecoderService::fetchCandidates(int index, int count, int sentFixedLen)
{
    QList<QString> candidatesList;
    for (int i = index; i < index + count; i++) {
        QString retStr = candidateAt(i);
        if (0 == i)
            retStr.remove(0, sentFixedLen);
        candidatesList.append(retStr);
    }
    return candidatesList;
}

int PinyinDecoderService::chooceCandidate(int index)
{
    return int(im_choose(index));
}

int PinyinDecoderService::cancelLastChoice()
{
    return int(im_cancel_last_choice());
}

int PinyinDecoderService::fixedLength()
{
    return (int)im_get_fixed_len();
}

void PinyinDecoderService::flushCache()
{
    im_flush_cache();
}

QList<QString> PinyinDecoderService::predictionList(const QString &history, int fetchSize)
{
    reloadUserDictionaryIfChanged();
    QList<QString> predictList;
    char16 (*predictItems)[kMaxPredictSize + 1] = 0;
    int predictNum = int(im_get_predicts(history.utf16(), predictItems));
    fetchSize = predictNum > fetchSize ? fetchSize : predictNum;
    predictList.reserve(fetchSize);
    for (int i = 0; i < fetchSize; i++)
        predictList.append(QString((QChar *)predictItems[i]));
    return predictList;
}

QString PinyinDecoderService::addUserPhrase(const QString &phrase, const QString &pinyin)
{
    if (!init())
        return QString::fromUtf8("词典初始化失败");

    QString word = phrase.trimmed();
    QString spelling = pinyin.trimmed().toLower();
    spelling.replace(QRegExp(QLatin1String("\\s+")), QLatin1String("'"));
    spelling.replace(QRegExp(QLatin1String("'+")), QLatin1String("'"));
    if (word.length() < 2 || word.length() > kMaxLemmaSize)
        return QString::fromUtf8("词组长度需为 2—8 个汉字");
    if (spelling.isEmpty())
        return QString::fromUtf8("请输入完整拼音");
    for (int i = 0; i < word.length(); ++i) {
        if (word.at(i).unicode() < 0x3400 || word.at(i).unicode() > 0x9fff)
            return QString::fromUtf8("词组只能包含汉字");
    }
    QByteArray spellingBytes = spelling.toLatin1();
    if (!im_add_user_lemma(reinterpret_cast<const char16 *>(word.utf16()),
                           uint16(word.length()), spellingBytes.constData(),
                           uint16(spellingBytes.length())))
        return QString::fromUtf8("添加失败：拼音音节数必须与汉字数相同");
    QFileInfo dictInfo(userDictionaryFilePath());
    userDictionaryMTime = dictInfo.lastModified().toMSecsSinceEpoch();
    userDictionarySize = dictInfo.size();
    return QString();
}

int PinyinDecoderService::userDictionaryEntryCount() const
{
    return int(im_get_user_dictionary_lemma_count());
}

QVariantList PinyinDecoderService::userDictionaryEntries() const
{
    QVariantList entries;
    const size_t count = im_get_user_dictionary_lemma_count();
    for (size_t i = 0; i < count; ++i) {
        char16 phrase[kMaxLemmaSize + 1] = { 0 };
        char spelling[64] = { 0 };
        if (!im_get_user_dictionary_lemma(i, phrase, kMaxLemmaSize + 1,
                                          spelling, sizeof(spelling)))
            continue;
        QVariantMap entry;
        entry.insert(QLatin1String("phrase"), QString(reinterpret_cast<QChar *>(phrase)));
        entry.insert(QLatin1String("pinyin"), QString::fromLatin1(spelling).toLower());
        entries.append(entry);
    }
    return entries;
}

QList<QString> PinyinDecoderService::userDictionaryEntryList() const
{
    QList<QString> result;
    QVariantList entries = userDictionaryEntries();
    for (int i = 0; i < entries.length(); ++i) {
        QVariantMap entry = entries.at(i).toMap();
        result.append(entry.value(QLatin1String("phrase")).toString()
                      + QLatin1Char('\t')
                      + entry.value(QLatin1String("pinyin")).toString());
    }
    return result;
}

QList<QString> PinyinDecoderService::userDictionaryEntryPage(int offset, int limit) const
{
    QList<QString> result;
    const int total = int(im_get_user_dictionary_lemma_count());
    int rawIndex = qMax(0, offset);
    const int pageSize = qBound(1, limit, 500);
    QList<QString> rows;
    while (rawIndex < total && rows.length() < pageSize) {
        char16 phrase[kMaxLemmaSize + 1] = { 0 };
        char spelling[64] = { 0 };
        if (im_get_user_dictionary_lemma(size_t(rawIndex), phrase,
                                         kMaxLemmaSize + 1, spelling,
                                         sizeof(spelling))) {
            rows.append(QString(reinterpret_cast<QChar *>(phrase))
                        + QLatin1Char('\t')
                        + QString::fromLatin1(spelling).toLower());
        }
        ++rawIndex;
    }
    // The first row is paging metadata and is never displayed.
    result.append(QLatin1String("@next:") + QString::number(rawIndex)
                  + QLatin1Char(':') + QString::number(total));
    result.append(rows);
    return result;
}

QString PinyinDecoderService::removeUserPhrase(const QString &phrase, const QString &pinyin)
{
    QString spelling = pinyin.trimmed().toLower();
    spelling.replace(QRegExp(QLatin1String("\\s+")), QLatin1String("'"));
    QByteArray bytes = spelling.toLatin1();
    if (!im_remove_user_lemma(reinterpret_cast<const char16 *>(phrase.utf16()),
                              uint16(phrase.length()), bytes.constData(),
                              uint16(bytes.length())))
        return QString::fromUtf8("删除失败，词条可能已不存在");
    QFileInfo dictInfo(userDictionaryFilePath());
    userDictionaryMTime = dictInfo.lastModified().toMSecsSinceEpoch();
    userDictionarySize = dictInfo.size();
    return QString();
}

QString PinyinDecoderService::updateUserPhrase(const QString &oldPhrase,
                                                const QString &oldPinyin,
                                                const QString &newPhrase,
                                                const QString &newPinyin)
{
    if (oldPhrase == newPhrase.trimmed() &&
            oldPinyin.simplified() == newPinyin.trimmed().simplified())
        return QString();
    QString error = addUserPhrase(newPhrase, newPinyin);
    if (!error.isEmpty())
        return error;
    error = removeUserPhrase(oldPhrase, oldPinyin);
    if (!error.isEmpty())
        return QString::fromUtf8("新词条已保存，但旧词条删除失败");
    return QString();
}

QString PinyinDecoderService::userDictionaryPath() const
{
    return userDictionaryFilePath();
}

bool PinyinDecoderService::resetUserDictionary()
{
    if (!init())
        return false;
    QByteArray path = userDictionaryPath().toUtf8();
    bool ok = im_reset_user_dictionary(path.constData());
    if (ok) {
        QFileInfo dictInfo(userDictionaryFilePath());
        userDictionaryMTime = dictInfo.lastModified().toMSecsSinceEpoch();
        userDictionarySize = dictInfo.size();
    }
    return ok;
}

bool PinyinDecoderService::reloadUserDictionaryIfChanged()
{
    if (!init())
        return false;
    QFileInfo dictInfo(userDictionaryFilePath());
    const qint64 mtime = dictInfo.lastModified().toMSecsSinceEpoch();
    const qint64 size = dictInfo.size();
    if (mtime == userDictionaryMTime && size == userDictionarySize)
        return false;

    QByteArray path = dictInfo.absoluteFilePath().toUtf8();
    im_init_user_dictionary(path.constData());
    userDictionaryMTime = mtime;
    userDictionarySize = size;
    return im_is_user_dictionary_enabled();
}

