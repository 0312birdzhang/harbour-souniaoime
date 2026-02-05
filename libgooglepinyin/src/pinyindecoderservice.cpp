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
#include <QFileInfo>
#include <QDir>
#include <QtCore/QLibraryInfo>
#include <QSysInfo>


using namespace ime_pinyin;

QScopedPointer<PinyinDecoderService> PinyinDecoderService::_instance;

/*!
    \class QtVirtualKeyboard::PinyinDecoderService
    \internal
*/

PinyinDecoderService::PinyinDecoderService(QObject *parent) :
    QObject(parent),
    initDone(false)
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
    QString usrDictPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QFileInfo usrDictInfo(usrDictPath + QLatin1String("/pinyin/usr_dict.dat"));
    
    // 确保目录存在
    QDir().mkpath(usrDictInfo.absolutePath());
    
    // 确保文件存在且不为空
    if (!usrDictInfo.exists() || usrDictInfo.size() == 0) {
        QFile file(usrDictInfo.absoluteFilePath());
        if (file.open(QIODevice::WriteOnly)) {
            // 写入词典版本号 (使用与 UserDict 中相同的版本号)
            uint32 version = 0x0ABCDEF0;
            file.write((const char*)&version, sizeof(version));
            
            // 写入空的词典信息 (根据 UserDictInfo 结构体)
            // 对应 UserDict::reset 方法的实现
            char info[40] = {0}; // 足够容纳 UserDictInfo 结构体
            file.write(info, 40);
            file.close();
        }
    }

    // 尝试初始化解码器
    initDone = im_open_decoder(sysDict.toUtf8().constData(), usrDictInfo.absoluteFilePath().toUtf8().constData());
    
    // 如果初始化失败，检查文件大小
    if (!initDone) {
        QFileInfo checkInfo(usrDictInfo.absoluteFilePath());
        if (checkInfo.size() == 0) {
            // 文件仍然为空，再次尝试创建
            QFile file(usrDictInfo.absoluteFilePath());
            if (file.open(QIODevice::WriteOnly)) {
                // 写入词典版本号
                uint32 version = 0x0ABCDEF0;
                file.write((const char*)&version, sizeof(version));
                // 写入空的词典信息
                char info[40] = {0};
                file.write(info, 40);
                file.close();
                // 再次尝试初始化
                initDone = im_open_decoder(sysDict.toUtf8().constData(), usrDictInfo.absoluteFilePath().toUtf8().constData());
            }
        }
    }
    
    return initDone;
}

void PinyinDecoderService::setUserDictionary(bool enabled)
{
    if (enabled == im_is_user_dictionary_enabled())
        return;
    if (enabled) {
        QString usrDictPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
        QFileInfo usrDictInfo(usrDictPath + QLatin1String("/pinyin/usr_dict.dat"));
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
    QList<QString> predictList;
    char16 (*predictItems)[kMaxPredictSize + 1] = 0;
    int predictNum = int(im_get_predicts(history.utf16(), predictItems));
    fetchSize = predictNum > fetchSize ? fetchSize : predictNum;
    predictList.reserve(fetchSize);
    for (int i = 0; i < fetchSize; i++)
        predictList.append(QString((QChar *)predictItems[i]));
    return predictList;
}

