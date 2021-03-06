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

#ifndef PINYINDECODERSERVICE_H
#define PINYINDECODERSERVICE_H

#include <QObject>
#include <QVector>
#include <QVariant>


class PinyinDecoderService : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PinyinDecoderService)


public:
    ~PinyinDecoderService();

    static PinyinDecoderService *getInstance();
    explicit PinyinDecoderService(QObject *parent = 0);
    Q_INVOKABLE bool init();
    Q_INVOKABLE void setUserDictionary(bool enabled);
    bool isUserDictionaryEnabled() const;
    void setLimits(int maxSpelling, int maxHzsLen);
    Q_INVOKABLE int search(const QString &spelling);
    int deleteSearch(int pos, bool isPosInSpellingId, bool clearFixedInThisStep);
    Q_INVOKABLE void resetSearch();
    Q_INVOKABLE QString pinyinString(bool decoded);
    int pinyinStringLength(bool decoded);
    Q_INVOKABLE QList<QString> spellingStartPositions();
    Q_INVOKABLE int getSplStart();
    Q_INVOKABLE QString candidateAt(int index);
    QList<QString> fetchCandidates(int index, int count, int sentFixedLen);
    Q_INVOKABLE int chooceCandidate(int index);
    int cancelLastChoice();
    Q_INVOKABLE int fixedLength();
    void flushCache();
    Q_INVOKABLE QList<QString> predictionList(const QString &history, int fetchSize);

private:
    static QScopedPointer<PinyinDecoderService> _instance;
    bool initDone;
};

#endif // PINYINDECODERSERVICE_H
