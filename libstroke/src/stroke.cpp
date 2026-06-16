#include "stroke.h"
#include <QSet>
#include <QDebug>

StrokeRecognition::StrokeRecognition(QObject *parent) : QObject(parent)
{
    initializeStrokeMap();
}

QStringList StrokeRecognition::recognize(const QString &strokeSequence)
{
    m_candidates = matchStroke(strokeSequence);
    emit candidatesChanged();
    return m_candidates;
}

QStringList StrokeRecognition::candidates() const
{
    return m_candidates;
}

void StrokeRecognition::initializeStrokeMap()
{
    // Stroke sequence to character mapping
    // Format: strokeSequence -> characters

    // 1 stroke
    m_strokeMap["一"] = {"一"};
    m_strokeMap["丨"] = {"丨"};
    m_strokeMap["丿"] = {"丿"};
    m_strokeMap["丶"] = {"丶"};
    m_strokeMap["乛"] = {"乙", "乛"};

    // 2 strokes
    m_strokeMap["一一"] = {"二"};
    m_strokeMap["一丨"] = {"十", "丁"};
    m_strokeMap["一丿"] = {"厂", "七"};
    m_strokeMap["一丶"] = {"丈"};
    m_strokeMap["一乛"] = {"又", "力"};

    m_strokeMap["丨一"] = {"上"};
    m_strokeMap["丨丨"] = {"川", "州"};
    m_strokeMap["丨丿"] = {"卜"};
    m_strokeMap["丨乛"] = {"几", "九", "八"};

    m_strokeMap["丿一"] = {"千"};
    m_strokeMap["丿丨"] = {"个"};
    m_strokeMap["丿丿"] = {"人", "入"};
    m_strokeMap["丿丶"] = {"八"};
    m_strokeMap["丿乛"] = {"九", "几"};

    m_strokeMap["丶一"] = {"主"};
    m_strokeMap["丶丨"] = {"门"};
    m_strokeMap["丶丿"] = {"火"};
    m_strokeMap["丶丶"] = {"斗"};
    m_strokeMap["丶乛"] = {"为"};

    m_strokeMap["乛一"] = {"弓", "卫"};
    m_strokeMap["乛丨"] = {"了", "乃"};
    m_strokeMap["乛丿"] = {"刀"};
    m_strokeMap["乛丶"] = {"又"};
    m_strokeMap["乛乛"] = {"孑"};

    // 3 strokes
    m_strokeMap["一一一"] = {"三", "丰"};
    m_strokeMap["一一丨"] = {"工", "土", "干", "士"};
    m_strokeMap["一一丿"] = {"大"};
    m_strokeMap["一一丶"] = {"元"};
    m_strokeMap["一一乛"] = {"与", "万"};

    m_strokeMap["一丨一"] = {"土", "士", "工", "干"};
    m_strokeMap["一丨丨"] = {"川"};
    m_strokeMap["一丨丿"] = {"大", "丈"};
    m_strokeMap["一丨丶"] = {"下"};
    m_strokeMap["一丨乛"] = {"寸"};

    m_strokeMap["一丿一"] = {"厂"};
    m_strokeMap["一丿丨"] = {"不", "大"};
    m_strokeMap["一丿丿"] = {"天"};
    m_strokeMap["一丿丶"] = {"太"};
    m_strokeMap["一丿乛"] = {"龙"};

    m_strokeMap["一丶一"] = {"万"};
    m_strokeMap["一丶丿"] = {"丈"};
    m_strokeMap["一丶丶"] = {"头"};
    m_strokeMap["一乛一"] = {"七"};
    m_strokeMap["一乛丨"] = {"刀", "力"};
    m_strokeMap["一乛丿"] = {"又"};
    m_strokeMap["一乛乛"] = {"与"};

    m_strokeMap["丨一一"] = {"上", "止"};
    m_strokeMap["丨一丨"] = {"口", "中"};
    m_strokeMap["丨一丿"] = {"足"};
    m_strokeMap["丨一丶"] = {"心"};
    m_strokeMap["丨一乛"] = {"口"};

    m_strokeMap["丨丨一"] = {"止"};
    m_strokeMap["丨丨乛"] = {"山"};

    m_strokeMap["丨丿一"] = {"千"};
    m_strokeMap["丨丿丿"] = {"白"};
    m_strokeMap["丨丶一"] = {"上"};
    m_strokeMap["丨丶丿"] = {"小"};
    m_strokeMap["丨乛一"] = {"口", "中"};
    m_strokeMap["丨乛丨"] = {"山", "出"};

    m_strokeMap["丿一一"] = {"千", "午"};
    m_strokeMap["丿一丨"] = {"千"};
    m_strokeMap["丿一丿"] = {"生"};
    m_strokeMap["丿一乛"] = {"年"};
    m_strokeMap["丿丨一"] = {"生"};
    m_strokeMap["丿丨丨"] = {"川"};
    m_strokeMap["丿丨乛"] = {"几"};

    m_strokeMap["丿丿一"] = {"人"};
    m_strokeMap["丿丿丨"] = {"个"};
    m_strokeMap["丿丿丶"] = {"八"};
    m_strokeMap["丿丶一"] = {"人"};
    m_strokeMap["丿丶丨"] = {"入"};
    m_strokeMap["丿乛一"] = {"几"};
    m_strokeMap["丿乛丨"] = {"九"};

    m_strokeMap["丶一一"] = {"主"};
    m_strokeMap["丶一丨"] = {"主"};
    m_strokeMap["丶一丿"] = {"立"};
    m_strokeMap["丶一丶"] = {"文"};
    m_strokeMap["丶丨丿"] = {"火"};
    m_strokeMap["丶丶一"] = {"主"};
    m_strokeMap["丶丶丶"] = {"州"};
    m_strokeMap["丶乛丿"] = {"为"};
    m_strokeMap["丶乛丶"] = {"为"};

    m_strokeMap["乛一一"] = {"弓"};
    m_strokeMap["乛一丨"] = {"已", "己", "巳"};
    m_strokeMap["乛一丿"] = {"卫"};
    m_strokeMap["乛丨一"] = {"了", "子"};
    m_strokeMap["乛丨丿"] = {"乃"};
    m_strokeMap["乛丨乛"] = {"女"};
    m_strokeMap["乛丿一"] = {"女"};
    m_strokeMap["乛丿丶"] = {"刀"};
    m_strokeMap["乛丶一"] = {"又"};

    // 4 strokes
    m_strokeMap["一一一一"] = {"王", "丰"};
    m_strokeMap["一一一丨"] = {"王", "丰"};
    m_strokeMap["一一一丿"] = {"夫"};
    m_strokeMap["一一一丶"] = {"元", "天"};
    m_strokeMap["一一一乛"] = {"开"};

    m_strokeMap["一一丨一"] = {"王", "工"};
    m_strokeMap["一一丨丨"] = {"井"};
    m_strokeMap["一一丨丿"] = {"天"};
    m_strokeMap["一一丨丶"] = {"天"};
    m_strokeMap["一一丨乛"] = {"开"};

    m_strokeMap["一丨一一"] = {"王", "玉"};
    m_strokeMap["一丨一丨"] = {"日", "日", "早"};
    m_strokeMap["一丨一丿"] = {"大"};
    m_strokeMap["一丨一丶"] = {"犬"};
    m_strokeMap["一丨一乛"] = {"口", "日"};

    m_strokeMap["一丨丨一"] = {"曲"};
    m_strokeMap["一丨丨丿"] = {"大"};
    m_strokeMap["一丨丨乛"] = {"山"};

    m_strokeMap["一丨丿一"] = {"天"};
    m_strokeMap["一丨丿丨"] = {"不"};
    m_strokeMap["一丨丿乛"] = {"本"};
    m_strokeMap["一丨丶一"] = {"不"};

    m_strokeMap["一丿一一"] = {"夫", "天"};
    m_strokeMap["一丿一丨"] = {"天"};
    m_strokeMap["一丿一丿"] = {"天"};
    m_strokeMap["一丿一乛"] = {"龙"};
    m_strokeMap["一丿丨一"] = {"不"};
    m_strokeMap["一丿丨丿"] = {"不"};
    m_strokeMap["一丿丨乛"] = {"不"};
    m_strokeMap["一丿丿丶"] = {"木"};
    m_strokeMap["一丿丶一"] = {"太"};
    m_strokeMap["一丿丶丶"] = {"头"};

    m_strokeMap["一丶一丿"] = {"不"};
    m_strokeMap["一丶一丶"] = {"文"};
    m_strokeMap["一丶丿丶"] = {"文"};
    m_strokeMap["一丶乛一"] = {"户"};
    m_strokeMap["一乛一丿"] = {"龙"};
    m_strokeMap["一乛丨一"] = {"也"};
    m_strokeMap["一乛丨乛"] = {"女"};
    m_strokeMap["一乛丿一"] = {"女"};
    m_strokeMap["一乛乛一"] = {"弓"};
    m_strokeMap["一乛乛丨"] = {"子"};

    m_strokeMap["丨一一一"] = {"止"};
    m_strokeMap["丨一一丨"] = {"口", "中"};
    m_strokeMap["丨一一丿"] = {"止"};
    m_strokeMap["丨一丨一"] = {"口", "中"};
    m_strokeMap["丨一丨乛"] = {"口"};
    m_strokeMap["丨一丿一"] = {"足"};
    m_strokeMap["丨一乛一"] = {"口"};

    m_strokeMap["丨丨一一"] = {"止"};
    m_strokeMap["丨丨一乛"] = {"山"};
    m_strokeMap["丨乛一一"] = {"口"};
    m_strokeMap["丨乛一丨"] = {"口", "日"};
    m_strokeMap["丨乛一乛"] = {"口"};
    m_strokeMap["丨乛丨一"] = {"山", "出"};

    m_strokeMap["丿一一一"] = {"千", "王"};
    m_strokeMap["丿一一丨"] = {"生"};
    m_strokeMap["丿一丨一"] = {"生"};
    m_strokeMap["丿一乛丨"] = {"年"};
    m_strokeMap["丿丨一丨"] = {"生"};
    m_strokeMap["丿丨乛一"] = {"几"};
    m_strokeMap["丿丿一一"] = {"人"};
    m_strokeMap["丿丿一丨"] = {"个"};
    m_strokeMap["丿丿丨一"] = {"个"};
    m_strokeMap["丿丶一一"] = {"人"};
    m_strokeMap["丿丶一丨"] = {"入"};

    m_strokeMap["丶一一一"] = {"主"};
    m_strokeMap["丶一一丨"] = {"主"};
    m_strokeMap["丶一丿一"] = {"立"};
    m_strokeMap["丶一丿丿"] = {"立"};
    m_strokeMap["丶一丶一"] = {"文"};
    m_strokeMap["丶一丶丿"] = {"文"};
    m_strokeMap["丶丿一丿"] = {"火"};
    m_strokeMap["丶丿丿一"] = {"火"};
    m_strokeMap["丶乛一亅"] = {"为"};

    m_strokeMap["乛一一一"] = {"弓"};
    m_strokeMap["乛一丨一"] = {"已", "己"};
    m_strokeMap["乛一丨乛"] = {"已"};
    m_strokeMap["乛丨一一"] = {"了", "子"};
    m_strokeMap["乛丨一丿"] = {"子"};
    m_strokeMap["乛丨乛一"] = {"女"};
    m_strokeMap["乛丨乛丿"] = {"女"};
    m_strokeMap["乛丿一一"] = {"女"};
    m_strokeMap["乛丿一乛"] = {"刀"};

    // 5 strokes
    m_strokeMap["一一一一一"] = {"王", "玉"};
    m_strokeMap["一一一一丨"] = {"王"};
    m_strokeMap["一一一一丿"] = {"玉"};
    m_strokeMap["一一一丨一"] = {"王"};
    m_strokeMap["一一一丨乛"] = {"玉"};
    m_strokeMap["一一一丿一"] = {"夫", "天"};
    m_strokeMap["一一一丿丶"] = {"天"};
    m_strokeMap["一一一丶一"] = {"天"};
    m_strokeMap["一一丨一丨"] = {"正"};
    m_strokeMap["一一丨一乛"] = {"正"};
    m_strokeMap["一一丨丨一"] = {"井"};
    m_strokeMap["一一丨乛一"] = {"正"};

    m_strokeMap["一丨一一一"] = {"王"};
    m_strokeMap["一丨一丨一"] = {"日", "早"};
    m_strokeMap["一丨一丨乛"] = {"日"};
    m_strokeMap["一丨一丿丶"] = {"木"};
    m_strokeMap["一丨一丶丶"] = {"木"};
    m_strokeMap["一丨一乛一"] = {"日"};
    m_strokeMap["一丨一乛丿"] = {"口"};
    m_strokeMap["一丨一乛丶"] = {"口"};
    m_strokeMap["一丨一乛乛"] = {"口", "日"};

    m_strokeMap["一丨丿丶一"] = {"本"};
    m_strokeMap["一丨丿丶乛"] = {"本"};
    m_strokeMap["一丨乛一一"] = {"口"};
    m_strokeMap["一丨乛一丨"] = {"日"};
    m_strokeMap["一丨乛丿一"] = {"田"};
    m_strokeMap["一丨乛丿丶"] = {"田"};

    m_strokeMap["一丿一丨一"] = {"天"};
    m_strokeMap["一丿一丨乛"] = {"天"};
    m_strokeMap["一丿一丿一"] = {"天"};
    m_strokeMap["一丿一乛乛"] = {"龙"};
    m_strokeMap["一丿丨一丨"] = {"不"};
    m_strokeMap["一丿丨一丶"] = {"不"};
    m_strokeMap["一丿丨乛一"] = {"不"};
    m_strokeMap["一丿丿丨丶"] = {"术"};
    m_strokeMap["一丿丿丶丶"] = {"木"};
    m_strokeMap["一丿丶一丶"] = {"太"};
    m_strokeMap["一丿丶丿丶"] = {"木"};

    m_strokeMap["丨乛一一一"] = {"日"};
    m_strokeMap["丨乛一一丨"] = {"日", "田"};
    m_strokeMap["丨乛一丨一"] = {"日"};
    m_strokeMap["丨乛一丿丶"] = {"四"};
    m_strokeMap["丨乛一乛一"] = {"口"};
    m_strokeMap["丨乛丨乛一"] = {"山", "出"};

    m_strokeMap["丿一一一丨一"] = {"生"};
    m_strokeMap["丿一乛丨丨"] = {"年"};
    m_strokeMap["丿丨乛一乛"] = {"几"};

    m_strokeMap["丶一丿一一"] = {"立"};
    m_strokeMap["丶一丿丨一"] = {"立"};
    m_strokeMap["丶一丿丿一"] = {"立"};
    m_strokeMap["丶一丿丶一"] = {"立"};
    m_strokeMap["丶一丶一丿"] = {"文"};
    m_strokeMap["丶一丶一丶"] = {"文"};
    m_strokeMap["丶一丶丿一"] = {"文"};

    m_strokeMap["乛丨乛丿一"] = {"女"};
    m_strokeMap["乛丨乛一一"] = {"女"};
    m_strokeMap["乛丿一一一"] = {"女"};

    // Common characters with stroke sequences - organized by frequency
    m_strokeMap["一丨乛一一"] = {"日", "曰"};
    m_strokeMap["丨乛一一一"] = {"日", "曰"};
    m_strokeMap["丨乛一丨一"] = {"日", "曰"};
    m_strokeMap["丨乛一乛一"] = {"口"};
    m_strokeMap["一丨一乛一"] = {"日", "曰"};
    m_strokeMap["一丨一乛丿"] = {"口"};
    m_strokeMap["一丨一乛丶"] = {"口"};
    m_strokeMap["一丨乛一丨"] = {"日"};
    m_strokeMap["一丨乛丿一"] = {"田"};
    m_strokeMap["一丨乛丿丶"] = {"田"};
    m_strokeMap["一丨一丿丶"] = {"木", "本"};
    m_strokeMap["一丨丿丶一"] = {"木"};
    m_strokeMap["一丨丿丶乛"] = {"本"};
    m_strokeMap["一丿丨乛一"] = {"不"};
    m_strokeMap["一丿丨一丶"] = {"不"};
    m_strokeMap["一丿一丿丶"] = {"天"};
    m_strokeMap["一丿丿丨丶"] = {"术"};
    m_strokeMap["一乛丨乛一"] = {"也"};
    m_strokeMap["一乛丨乛丿"] = {"女"};
    m_strokeMap["一乛乛丨一"] = {"子"};

    m_strokeMap["丨乛一丿丶"] = {"四"};
    m_strokeMap["丨乛丨一丿"] = {"山"};
    m_strokeMap["丨乛丨乛一"] = {"出"};

    m_strokeMap["丿一丨一乛"] = {"年"};
    m_strokeMap["丿一乛丨丨"] = {"年"};
    m_strokeMap["丿一乛一一"] = {"年"};
    m_strokeMap["丿一一一丨一"] = {"生"};
    m_strokeMap["丿一丨一乛"] = {"生"};
    m_strokeMap["丿丨乛一丨"] = {"几"};
    m_strokeMap["丿丨乛一乛"] = {"几"};
    m_strokeMap["丿丿一丨一"] = {"个"};
    m_strokeMap["丿丿一丨乛"] = {"个"};
    m_strokeMap["丿丶一丨乛"] = {"入"};

    m_strokeMap["丶一丿一一"] = {"立"};
    m_strokeMap["丶一丿丨一"] = {"立"};
    m_strokeMap["丶一丿丿一"] = {"立"};
    m_strokeMap["丶一丿丶一"] = {"立"};
    m_strokeMap["丶一丶一丿"] = {"文"};
    m_strokeMap["丶一丶一丶"] = {"文"};
    m_strokeMap["丶一丶丿一"] = {"文"};

    // More common 4-stroke characters
    m_strokeMap["一丿一丨一"] = {"不"};
    m_strokeMap["一丿一丨丿"] = {"不"};
    m_strokeMap["一丿丿一一"] = {"天"};
    m_strokeMap["一丿丿一丨"] = {"天"};
    m_strokeMap["一丿一一一"] = {"天"};
    m_strokeMap["一丿一一丨"] = {"天"};
    m_strokeMap["一乛一丿丶"] = {"龙"};
    m_strokeMap["一乛一丿乛"] = {"龙"};
    m_strokeMap["一乛丿一乛"] = {"龙"};
    m_strokeMap["一乛丿丿丶"] = {"龙"};

    m_strokeMap["丨乛一一一"] = {"日"};
    m_strokeMap["丨乛一丨一"] = {"日"};
    m_strokeMap["丨乛一乛一"] = {"日"};
    m_strokeMap["丨乛一丿丶"] = {"四"};
    m_strokeMap["丨乛丨一丿"] = {"山"};
    m_strokeMap["丨乛丨乛一"] = {"出"};

    m_strokeMap["丿一丨一乛"] = {"生"};
    m_strokeMap["丿一丿一丨"] = {"生"};
    m_strokeMap["丿一乛丨丨"] = {"年"};
    m_strokeMap["丿一乛一一"] = {"年"};

    // Extended dictionary - common multi-stroke characters
    // 大
    m_strokeMap["一丿丶"] = {"大"};
    // 火
    m_strokeMap["丶丿丿丶"] = {"火"};
    // 水
    m_strokeMap["丨乛丿丶"] = {"水"};
    // 金
    m_strokeMap["丿丶一一丨乛一"] = {"金"};
    // 土
    m_strokeMap["一丨一"] = {"土", "士"};
    // 木
    m_strokeMap["一丨丿丶"] = {"木"};
    // 人
    m_strokeMap["丿丶"] = {"人"};
    // 口
    m_strokeMap["丨乛一"] = {"口"};
    // 手
    m_strokeMap["丿一一一乛"] = {"手"};
    // 山
    m_strokeMap["丨乛丨"] = {"山"};
    // 川
    m_strokeMap["丿丨丨"] = {"川"};
    // 工
    m_strokeMap["一丨一"] = {"工", "土", "士"};
    // 上
    m_strokeMap["丨一一"] = {"上"};
    // 下
    m_strokeMap["一丨丶"] = {"下"};
    // 小
    m_strokeMap["丨丿丶"] = {"小"};
    // 心
    m_strokeMap["丶乛丶丶"] = {"心"};
    // 门
    m_strokeMap["丶丨乛丨"] = {"门"};
    // 月
    m_strokeMap["丿乛一一"] = {"月"};
    // 风
    m_strokeMap["丿乛丿丶"] = {"风"};
    // 车
    m_strokeMap["一乛一丨"] = {"车"};
    // 马
    m_strokeMap["乛一一"] = {"马"};
    // 鸟
    m_strokeMap["丿乛丶乛一一"] = {"鸟"};
    // 鱼
    m_strokeMap["丿乛丨乛一一"] = {"鱼"};
    // 龙
    m_strokeMap["一丿乛丿乛丿丶"] = {"龙"};
    // 石
    m_strokeMap["一丿丨乛一"] = {"石"};
    // 目
    m_strokeMap["丨乛一一一"] = {"目"};
    // 田
    m_strokeMap["丨乛一丨一"] = {"田"};
    // 力
    m_strokeMap["乛丿"] = {"力"};
    // 刀
    m_strokeMap["乛丿"] = {"刀"};
    // 寸
    m_strokeMap["一乛丶"] = {"寸"};
    // 才
    m_strokeMap["一丿丨"] = {"才"};
    // 也
    m_strokeMap["乛丨丨乛"] = {"也"};
    // 飞
    m_strokeMap["乛丿丶"] = {"飞"};
    // 女
    m_strokeMap["乛丿一"] = {"女"};
    // 子
    m_strokeMap["乛乛一一"] = {"子", "了"};
    // 已
    m_strokeMap["乛一丨乛"] = {"已", "己", "巳"};
    // 干
    m_strokeMap["一一丨"] = {"干"};
    // 千
    m_strokeMap["丿一十"] = {"千"};
    // 午
    m_strokeMap["丿一一十"] = {"午"};
    // 牛
    m_strokeMap["丿一一一丨"] = {"牛"};
    // 羊
    m_strokeMap["丶丿一一一丨"] = {"羊"};
    // 马
    m_strokeMap["乛一一"] = {"马"};
    // 书
    m_strokeMap["乛乛丨丶"] = {"书"};
    // 长
    m_strokeMap["丿一乛丶"] = {"长"};
    // 片
    m_strokeMap["丿丨一乛"] = {"片"};
    // 斤
    m_strokeMap["丿丿一丨"] = {"斤"};
    // 爪
    m_strokeMap["丿丨一一丨"] = {"爪"};
    // 父
    m_strokeMap["丿丶丿丶"] = {"父"};
    // 月
    m_strokeMap["丿乛一一"] = {"月"};
    // 欠
    m_strokeMap["丿乛丿丶"] = {"欠"};
    // 止
    m_strokeMap["丨一丨一"] = {"止"};
    // 正
    m_strokeMap["一丨一丨一"] = {"正"};
    // 世
    m_strokeMap["一丨丨一乛"] = {"世"};
    // 甘
    m_strokeMap["一丨丨一乛"] = {"甘"};
    // 东
    m_strokeMap["一乛丨丿丶"] = {"东"};
    // 丝
    m_strokeMap["乛乛一一丨一一"] = {"丝"};
    // 业
    m_strokeMap["丨丨一一"] = {"业"};
    // 册
    m_strokeMap["丿乛丿乛一"] = {"册"};
    // 乐
    m_strokeMap["丿乛丨丿丶"] = {"乐"};
    // 白
    m_strokeMap["丿丨乛一一"] = {"白"};
    // 百
    m_strokeMap["一丿丨乛一一"] = {"百"};
    // 自
    m_strokeMap["丿丨乛一一一"] = {"自"};
    // 向
    m_strokeMap["丿丨乛丨乛一"] = {"向"};
    // 身
    m_strokeMap["丿丨乛一一丿丿"] = {"身"};
    // 行
    m_strokeMap["丿丿一一一丨"] = {"行"};
    // 很
    m_strokeMap["丿丿丨一一乛丿丶"] = {"很"};
    // 得
    m_strokeMap["丿丨丨一一一一丨丶"] = {"得"};
    // 从
    m_strokeMap["丿丶丿丶"] = {"从"};
    // 众
    m_strokeMap["丿丶丿丶丿丶"] = {"众"};
    // 林
    m_strokeMap["一丨丿丶一丨丿丶"] = {"林"};
    // 森
    m_strokeMap["一丨丿丶一丨丿丶一丨丿丶"] = {"森"};
    // 休
    m_strokeMap["丿丨一丨丿丶"] = {"休"};
    // 体
    m_strokeMap["丿丨一一丨一"] = {"体"};
    // 作
    m_strokeMap["丿丨一丿一丨一"] = {"作"};
    // 你
    m_strokeMap["丿丨丿乛丨丿丶"] = {"你"};
    // 他
    m_strokeMap["丿丨一乛丨乛一"] = {"他"};
    // 她
    m_strokeMap["乛丿一乛丨乛一"] = {"她"};
    // 们
    m_strokeMap["丿丨一丨乛一"] = {"们"};
    // 什
    m_strokeMap["丿丨一十"] = {"什"};
    // 么
    m_strokeMap["丿丶丶"] = {"么"};
    // 这
    m_strokeMap["丶一丿丶丶乛丶"] = {"这"};
    // 那
    m_strokeMap["乛一一丿乛丨"] = {"那"};
    // 哪
    m_strokeMap["丨乛一乛一一丿乛丨"] = {"哪"};
    // 谁
    m_strokeMap["丶乛一丿丨一丨一一"] = {"谁"};
    // 的
    m_strokeMap["丿丨乛一一丨乛丶丶"] = {"的"};
    // 地
    m_strokeMap["一丨一乛丨乛一"] = {"地"};
    // 得
    m_strokeMap["丿丨丨一一一一丨丶"] = {"得"};
    // 了
    m_strokeMap["乛丨"] = {"了"};
    // 着
    m_strokeMap["丶丿一一一丨乛一一"] = {"着"};
    // 过
    m_strokeMap["一丨乛丶丶乛丶"] = {"过"};
    // 把
    m_strokeMap["一丨一乛丨乛一一乛"] = {"把"};
    // 被
    m_strokeMap["丶乛丿一丨乛一一丿丶"] = {"被"};
    // 和
    m_strokeMap["丿一丨丿丶丨乛一"] = {"和"};
    // 有
    m_strokeMap["一丿乛一一丨乛一一"] = {"有"};
    // 在
    m_strokeMap["一丿丨一丨一"] = {"在"};
    // 是
    m_strokeMap["丨乛一一一一丨一乛丿丶"] = {"是"};
    // 就
    m_strokeMap["丶一丨乛一丨一丿乛丶丶"] = {"就"};
    // 可
    m_strokeMap["一丨乛一一丨"] = {"可"};
    // 以
    m_strokeMap["乛丶丿丶"] = {"以"};
    // 能
    m_strokeMap["乛丶丿乛一一丨丨一丿乛"] = {"能"};
    // 会
    m_strokeMap["丿丶一一一乛丶"] = {"会"};
    // 想
    m_strokeMap["一丨乛丨一丨一丶乛丶丶"] = {"想"};
    // 要
    m_strokeMap["一丨乛丨一丨一乛丿一"] = {"要"};
    // 还
    m_strokeMap["一丿丨丶丶乛丶"] = {"还"};
    // 都
    m_strokeMap["一丨丿丨乛一一丿乛丨"] = {"都"};
    // 对
    m_strokeMap["乛丶一丨丶"] = {"对"};
    // 于
    m_strokeMap["一一丨"] = {"于"};
    // 与
    m_strokeMap["一乛一一"] = {"与"};
    // 到
    m_strokeMap["一乛丶一丨一丨丨"] = {"到"};
    // 处
    m_strokeMap["丿乛丿丨丶"] = {"处"};
    // 理
    m_strokeMap["一一丨一一丨乛一一"] = {"理"};
    // 发
    m_strokeMap["乛丿丶乛丨丶"] = {"发"};
    // 发
    m_strokeMap["乛丿丶乛丨丶"] = {"发"};
    // 现
    m_strokeMap["一一一丨乛一一丿乛"] = {"现"};
    // 点
    m_strokeMap["丨一丨一丶丶丶丶"] = {"点"};
    // 面
    m_strokeMap["一丿丨乛丨丨一一一"] = {"面"};
    // 前
    m_strokeMap["丶丿一丨乛一一一丨丨"] = {"前"};
    // 后
    m_strokeMap["丿一一丿丨乛一"] = {"后"};
    // 新
    m_strokeMap["丶一丨丨一一丨丿丶一丨"] = {"新"};
    // 开
    m_strokeMap["一一丨丨"] = {"开"};
    // 关
    m_strokeMap["丶丿一一一丿丶"] = {"关"};
    // 并
    m_strokeMap["丶丿一一一丨丨"] = {"并"};
    // 进
    m_strokeMap["一一丨丿丶丶乛丶"] = {"进"};
    // 出
    m_strokeMap["乛丨丨乛丨"] = {"出"};
    // 入
    m_strokeMap["丿丶丨"] = {"入"};
    // 回
    m_strokeMap["丨乛一丨乛一一"] = {"回"};
    // 因
    m_strokeMap["丨乛一一大丶"] = {"因"};
    // 国
    m_strokeMap["丨乛一一丨丶一"] = {"国"};
    // 家
    m_strokeMap["丶丶乛一丿一一丨丿丶"] = {"家"};
    // 如
    m_strokeMap["乛丿一丨乛一"] = {"如"};
    // 果
    m_strokeMap["丨乛一一一丨一丿丶"] = {"果"};
    // 时
    m_strokeMap["丨乛一一一丨丶"] = {"时"};
    // 间
    m_strokeMap["丨乛一一一丨乛一一"] = {"间"};
    // 明
    m_strokeMap["丨乛一一丿乛一一"] = {"明"};
    // 星
    m_strokeMap["丨乛一一丿一一丨一"] = {"星"};
    // 昨
    m_strokeMap["丨乛一一丿一丨一"] = {"昨"};
    // 晚
    m_strokeMap["丨乛一一丨乛丿丿一乛"] = {"晚"};
    // 早
    m_strokeMap["丨乛一一一丨"] = {"早"};
    // 春
    m_strokeMap["一一一丿丶丨乛一一"] = {"春"};
    // 天
    m_strokeMap["一一一丿丶"] = {"天"};
    // 气
    m_strokeMap["丿一一乛"] = {"气"};
    // 空
    m_strokeMap["丶丶乛丶一丨乛一"] = {"空"};
    // 中
    m_strokeMap["丨乛一丨"] = {"中"};
    // 外
    m_strokeMap["丿乛丶丨丶"] = {"外"};
    // 内
    m_strokeMap["丨乛丿丶"] = {"内"};
    // 全
    m_strokeMap["丿丶一一丨一"] = {"全"};
    // 部
    m_strokeMap["丶一丨乛一一丿乛丨"] = {"部"};
    // 分
    m_strokeMap["丿丶乛丿"] = {"分"};
    // 高
    m_strokeMap["丶一丨乛一丨乛一一"] = {"高"};
    // 低
    m_strokeMap["丿丨一丿一丨一丶"] = {"低"};
    // 长
    m_strokeMap["丿一乛丶"] = {"长"};
    // 短
    m_strokeMap["丿一一一丨一丨丶丶"] = {"短"};
    // 多
    m_strokeMap["丿丶丿丶"] = {"多"};
    // 少
    m_strokeMap["丨丿丿丶"] = {"少"};
    // 好
    m_strokeMap["乛丿一乛一一"] = {"好"};
    // 坏
    m_strokeMap["一丨一丶丿丶丶"] = {"坏"};
    // 多
    m_strokeMap["丿丶丿丶"] = {"多"};
    // 少
    m_strokeMap["丨丿丿丶"] = {"少"};
    // 大
    m_strokeMap["一丿丶"] = {"大"};
    // 小
    m_strokeMap["丨丿丶"] = {"小"};
    // 上
    m_strokeMap["丨一一"] = {"上"};
    // 下
    m_strokeMap["一丨丶"] = {"下"};
    // 左
    m_strokeMap["一丿一丨一"] = {"左"};
    // 右
    m_strokeMap["一丿丨乛一"] = {"右"};
    // 前
    m_strokeMap["丶丿一丨乛一一一丨丨"] = {"前"};
    // 后
    m_strokeMap["丿一一丿丨乛一"] = {"后"};
    // 我
    m_strokeMap["丿一丨乛一一丨丿丶"] = {"我"};
    // 他
    m_strokeMap["丿丨一也"] = {"他"};
    // 她
    m_strokeMap["乛丿一也"] = {"她"};
    // 它
    m_strokeMap["丶丶乛丿乛"] = {"它"};
    // 们
    m_strokeMap["丿丨一口"] = {"们"};
    // 一
    m_strokeMap["一"] = {"一"};
    // 二
    m_strokeMap["一一"] = {"二"};
    // 三
    m_strokeMap["一一一"] = {"三"};
    // 四
    m_strokeMap["丨乛丿乛一"] = {"四"};
    // 五
    m_strokeMap["一丨乛一丨"] = {"五"};
    // 六
    m_strokeMap["丶一丿丶"] = {"六"};
    // 七
    m_strokeMap["一乛"] = {"七"};
    // 八
    m_strokeMap["丿丶"] = {"八"};
    // 九
    m_strokeMap["丿乛"] = {"九"};
    // 十
    m_strokeMap["一丨"] = {"十"};
    // 百
    m_strokeMap["一丿丨乛一"] = {"百"};
    // 千
    m_strokeMap["丿一十"] = {"千"};
    // 万
    m_strokeMap["一乛"] = {"万"};
    // 亿
    m_strokeMap["丿丨一乛"] = {"亿"};
    // 个
    m_strokeMap["丿丨丨"] = {"个"};
    // 几
    m_strokeMap["丿乛"] = {"几"};
    // 公
    m_strokeMap["丿丶乛丶"] = {"公"};
    // 私
    m_strokeMap["丿一丨丿丶"] = {"私"};
    // 共
    m_strokeMap["一丨丨一丿丶"] = {"共"};
    // 总
    m_strokeMap["丶丿丨乛一丶丶丶"] = {"总"};
    // 合
    m_strokeMap["丿丶一丨乛一"] = {"合"};
    // 同
    m_strokeMap["丨乛一丨乛一"] = {"同"};
    // 名
    m_strokeMap["丿乛丶丨乛一"] = {"名"};
    // 各
    m_strokeMap["丿乛丶丨乛一"] = {"各"};
    // 位
    m_strokeMap["丿丨一丶一立"] = {"位"};
    // 住
    m_strokeMap["丿丨一丶丶一"] = {"住"};
    // 做
    m_strokeMap["丿丨一丨乛一丨一丿丶"] = {"做"};
    // 当
    m_strokeMap["丨丿丶乛一一"] = {"当"};
    // 然
    m_strokeMap["丿丶丶丶丶丶丶丶丶丶"] = {"然"};
    // 样
    m_strokeMap["一丨丿丶丶丶一一丨"] = {"样"};
    // 种
    m_strokeMap["丿一丨丿丶丨乛一丨"] = {"种"};
    m_strokeMap["一一丶丿一丨丿丶"] = {"来"};
    // 去
    m_strokeMap["一丨一乛丶"] = {"去"};
    // 也
    m_strokeMap["乛丨丨乛"] = {"也"};
    // 又
    m_strokeMap["乛丶"] = {"又"};
    // 再
    m_strokeMap["一丨乛一一丨一"] = {"再"};
    // 更
    m_strokeMap["一丨乛一一丿乛丶"] = {"更"};
    // 而
    m_strokeMap["一丿丨丨乛丨丨"] = {"而"};
    // 且
    m_strokeMap["丨乛一一一"] = {"且"};
    // 或
    m_strokeMap["一丨乛一丨乛一一丿丶"] = {"或"};
    // 成
    m_strokeMap["一丿乛一一丿丶"] = {"成"};
    // 我
    m_strokeMap["丿一丨乛一一ノ丶"] = {"我"};
    m_strokeMap["丿丨丿乛丨丿丶"] = {"你"};
    m_strokeMap["丿丨一乛丨乛一"] = {"他"};
    m_strokeMap["乛丿一乛丨乛一"] = {"她"};
    m_strokeMap["丶丶乛丿乛"] = {"它"};
    m_strokeMap["丿丨一丨乛一"] = {"们"};
    // 那
    m_strokeMap["乛一一丿乛丨"] = {"那"};
    // 哪
    m_strokeMap["丨乛一乛一一丿乛丨"] = {"哪"};
    // 这
    m_strokeMap["丶一丿丶丶乛丶"] = {"这"};
    // 过
    m_strokeMap["一丨乛丶丶乛丶"] = {"过"};
    // 还
    m_strokeMap["一丿丨丶丶乛丶"] = {"还"};
    // 送
    m_strokeMap["丶丿一一一丿丶丶乛丶"] = {"送"};
    // 道
    m_strokeMap["丶丿一丨乛一一一丨丨丶乛丶"] = {"道"};
    // 说
    m_strokeMap["丶乛一丶丶丨乛一"] = {"说"};
    // 话
    m_strokeMap["丶乛一丿一丨乛一"] = {"话"};
    // 请
    m_strokeMap["丶乛一一一丨乛一一"] = {"请"};
    // 读
    m_strokeMap["丶乛一丶乛一一丨丿丶"] = {"读"};
    // 谢
    m_strokeMap["丶乛一丿乛一一丨一丨丶"] = {"谢"};
    // 老
    m_strokeMap["一丿一一丿乛"] = {"老"};
    // 师
    m_strokeMap["丨丿一丨乛一一丨"] = {"师"};
    // 教
    m_strokeMap["一丿一丨一丿乛丨一"] = {"教"};
    // 学
    m_strokeMap["丶丶丿丶丿乛乛一一丨"] = {"学"};
    // 习
    m_strokeMap["乛丶一"] = {"习"};
    // 问
    m_strokeMap["丨乛一丨乛一"] = {"问"};
    // 题
    m_strokeMap["丨乛一一一丨一丿丶一丿丶"] = {"题"};
    // 知
    m_strokeMap["丿一丨乛一丨乛一"] = {"知"};
    // 道
    m_strokeMap["丶丿一丨乛一一一丨丨丶乛丶"] = {"道"};
    // 路
    m_strokeMap["丨乛一丨一丿乛丶丶丨乛一"] = {"路"};
    // 走
    m_strokeMap["一丨一丨一乛丿丶"] = {"走"};
    // 跑
    m_strokeMap["丨乛一丨一丿乛丶丿乛一乛"] = {"跑"};
    // 跳
    m_strokeMap["丨乛一丨一丿乛丶丿丶一丿丶"] = {"跳"};
    // 做
    m_strokeMap["丿丨一丨乛一丨一丿丶"] = {"做"};
    // 用
    m_strokeMap["丿乛一一丨"] = {"用"};
    // 东
    m_strokeMap["一乛丨丿丶"] = {"东"};
    // 西
    m_strokeMap["一丨乛丿乛一一"] = {"西"};
    // 南
    m_strokeMap["一丨丨乛丨一一一"] = {"南"};
    // 北
    m_strokeMap["丨一一一丿乛"] = {"北"};
    // 中
    m_strokeMap["丨乛一丨"] = {"中"};
    // 外
    m_strokeMap["丿乛丶丨丶"] = {"外"};
    // 前
    m_strokeMap["丶丿一丨乛一一一丨丨"] = {"前"};
    // 后
    m_strokeMap["丿一一丿丨乛一"] = {"后"};
    // 左
    m_strokeMap["一丿一丨一"] = {"左"};
    // 右
    m_strokeMap["一丿丨乛一"] = {"右"};
    // 旁
    m_strokeMap["丶一丿丶丶一乛丿"] = {"旁"};
    // 边
    m_strokeMap["乛丿丶丶乛丶"] = {"边"};
    // 第
    m_strokeMap["丿一一丨一乛丿乛一一丿"] = {"第"};
    // 等
    m_strokeMap["丿一一丶一丨一丨一一丿丶"] = {"等"};
    // 起
    m_strokeMap["一丨一丨一乛丿乛"] = {"起"};
    // 跟
    m_strokeMap["丨乛一丨一丿乛丶一丨乛一一"] = {"跟"};
    // 关
    m_strokeMap["丶丿一一一丿丶"] = {"关"};
    // 系
    m_strokeMap["丿一一丨丿丶乛一"] = {"系"};
    // 心
    m_strokeMap["丶乛丶丶"] = {"心"};
    // 思
    m_strokeMap["丨乛一丨一丶乛丶丶"] = {"思"};
    // 想
    m_strokeMap["一丨乛丨一丨一丶乛丶丶"] = {"想"};
    // 念
    m_strokeMap["丿丶一乛丶丶丶"] = {"念"};
    // 意
    m_strokeMap["丶一丨乛一丨一丶乛丶丶"] = {"意"};
    // 感
    m_strokeMap["一丿一丨乛一丿丶丶乛丶丶"] = {"感"};
    // 情
    m_strokeMap["丶丶一丨一一丨乛一一"] = {"情"};
    // 爱
    m_strokeMap["丿丶丶丿丶乛丿乛丶"] = {"爱"};
    // 恨
    m_strokeMap["丶丶一丨一乛丿一"] = {"恨"};
    // 快
    m_strokeMap["丶丶一丨一乛丿丶"] = {"快"};
    // 慢
    m_strokeMap["丶丶一丨一丨乛一一丨乛丶"] = {"慢"};
    // 忙
    m_strokeMap["丶丶一丨一乛"] = {"忙"};
    // 怕
    m_strokeMap["丶丶一丨一丿乛一一"] = {"怕"};
    // 惊
    m_strokeMap["丶丶一丨一丶一丨乛一一丨"] = {"惊"};
    // 吃
    m_strokeMap["丨乛一丿一乛"] = {"吃"};
    // 喝
    m_strokeMap["丨乛一丨乛一丿乛丶丶"] = {"喝"};
    // 唱
    m_strokeMap["丨乛一丨乛一一丨乛一一"] = {"唱"};
    // 听
    m_strokeMap["丨乛一丿一丨"] = {"听"};
    // 叫
    m_strokeMap["丨乛一乛丨"] = {"叫"};
    // 喊
    m_strokeMap["丨乛一一丿一丨乛丿丶"] = {"喊"};
    // 看
    m_strokeMap["丿一一一丨一一一丨乛一一"] = {"看"};
    // 见
    m_strokeMap["丨乛一一丿乛"] = {"见"};
    // 听
    m_strokeMap["丨乛一丿一丨"] = {"听"};
    // 说
    m_strokeMap["丶乛一丶丶丨乛一"] = {"说"};
    // 读
    m_strokeMap["丶乛一丶乛一一丨丿丶"] = {"读"};
    // 写
    m_strokeMap["丶乛一一一"] = {"写"};
    // 认
    m_strokeMap["丶乛一人"] = {"认"};
    // 识
    m_strokeMap["丶乛一丨乛一丿丶"] = {"识"};
    // 让
    m_strokeMap["丶乛一丨一一"] = {"让"};
    // 记
    m_strokeMap["丶乛一乛一"] = {"记"};
    // 计
    m_strokeMap["丶乛一十"] = {"计"};
    // 订
    m_strokeMap["丶乛一一丨"] = {"订"};
    // 定
    m_strokeMap["丶丶乛一一丨乛丿丶"] = {"定"};
    // 放
    m_strokeMap["丶一丿丶一丿丶"] = {"放"};
    // 收
    m_strokeMap["乛丨一丿丶"] = {"收"};
    // 改
    m_strokeMap["乛一丨一丿丶"] = {"改"};
    // 变
    m_strokeMap["丶一丨丨一乛乛丶"] = {"变"};
    // 化
    m_strokeMap["丿丨一丿乛"] = {"化"};
    // 代
    m_strokeMap["丿丨一弋"] = {"代"};
    // 表
    m_strokeMap["一一丨一一一丿乛丶"] = {"表"};
    // 示
    m_strokeMap["一一一丨丿丶"] = {"示"};
    // 例
    m_strokeMap["丿丨一一夕丨丨"] = {"例"};
    // 件
    m_strokeMap["丿丨一丿一丨"] = {"件"};
    // 信
    m_strokeMap["丿丨一丶一丨乛一一"] = {"信"};
    // 证
    m_strokeMap["丶乛一丨一丨一"] = {"证"};
    // 明
    m_strokeMap["丨乛一一丿乛一一"] = {"明"};
    // 研
    m_strokeMap["一丿丨乛一一一丨丨"] = {"研"};
    // 究
    m_strokeMap["丶丶乛一丿丶丿乛"] = {"究"};
    // 发
    m_strokeMap["乛丿丶乛丨丶"] = {"发"};
    // 现
    m_strokeMap["一一一丨乛一一丿乛"] = {"现"};
    // 发
    m_strokeMap["乛丿丶乛丨丶"] = {"发"};
    // 现
    m_strokeMap["一一一丨乛一一丿乛"] = {"现"};
    // 生
    m_strokeMap["丿一一一丨一"] = {"生"};
    // 活
    m_strokeMap["丶丶一丿一丨乛一"] = {"活"};
    // 动
    m_strokeMap["一一一乛丨一乛丿"] = {"动"};
    // 工
    m_strokeMap["一丨一"] = {"工"};
    // 作
    m_strokeMap["丿丨一丿一丨一"] = {"作"};
    // 业
    m_strokeMap["丨丨一一"] = {"业"};
    // 公
    m_strokeMap["丿丶乛丶"] = {"公"};
    // 司
    m_strokeMap["乛一丨乛一一"] = {"司"};
    // 管
    m_strokeMap["丿一一丶丶乛一丨乛一一"] = {"管"};
    // 理
    m_strokeMap["一一丨一一丨乛一一"] = {"理"};
    // 经
    m_strokeMap["乛乛一一丨一丨一"] = {"经"};
    // 济
    m_strokeMap["丶丶一丶一丨丨"] = {"济"};
    // 世
    m_strokeMap["一丨丨一乛"] = {"世"};
    // 界
    m_strokeMap["丨乛一一丨一丿丶丨丨"] = {"界"};
    // 由
    m_strokeMap["丨乛一丨一"] = {"由"};
    // 于
    m_strokeMap["一一丨"] = {"于"};
    // 相
    m_strokeMap["一丨丿丶丨乛一一"] = {"相"};
    // 对
    m_strokeMap["乛丶一丨丶"] = {"对"};
    // 当
    m_strokeMap["丨丿丶乛一一"] = {"当"};
    // 然
    m_strokeMap["丿丶丶丶丶丶丶丶丶丶"] = {"然"};
    // 能
    m_strokeMap["乛丶丿乛一一丨丨一丿乛"] = {"能"};
    // 力
    m_strokeMap["乛丿"] = {"力"};
    // 机
    m_strokeMap["一丨丿丶几"] = {"机"};
    // 器
    m_strokeMap["丨乛一丨一丨乛一一丨丨一"] = {"器"};
    // 电
    m_strokeMap["丨乛一一乛"] = {"电"};
    // 话
    m_strokeMap["丶乛一丿一丨乛一"] = {"话"};
    // 手
    m_strokeMap["丿一一一乛"] = {"手"};
    // 机
    m_strokeMap["一丨丿丶几"] = {"机"};
    // 网
    m_strokeMap["丨乛丿丶丿丶"] = {"网"};
    // 上
    m_strokeMap["丨一一"] = {"上"};
    // 下
    m_strokeMap["一丨丶"] = {"下"};
    // 线
    m_strokeMap["乛乛一一一丿丶"] = {"线"};
    // 数
    m_strokeMap["丶一丨乛一丨一丨一丿丶"] = {"数"};
    // 学
    m_strokeMap["丶丶丿丶丿乛乛一一丨"] = {"学"};
    // 英
    m_strokeMap["一丨丨一乛丨乛一一丿丶"] = {"英"};
    // 语
    m_strokeMap["丶乛一丨乛一一丨乛一"] = {"语"};
    // 文
    m_strokeMap["丶一丿丶"] = {"文"};
    // 化
    m_strokeMap["丿丨一丿乛"] = {"化"};
    // 国
    m_strokeMap["丨乛一一丨丶一"] = {"国"};
    // 家
    m_strokeMap["丶丶乛一丿一一丨丿丶"] = {"家"};
    // 人
    m_strokeMap["丿丶"] = {"人"};
    // 民
    m_strokeMap["乛一丨一乛"] = {"民"};
    // 和
    m_strokeMap["丿一丨丿丶丨乛一"] = {"和"};
    // 平
    m_strokeMap["一丿丶一丨"] = {"平"};
    // 安
    m_strokeMap["丶丶乛乛丿一"] = {"安"};
    // 全
    m_strokeMap["丿丶一一丨一"] = {"全"};
    // 正
    m_strokeMap["一丨一丨一"] = {"正"};
    // 确
    m_strokeMap["一丿丨乛一丿乛一一丿丶"] = {"确"};
    // 实
    m_strokeMap["丶丶乛一一一丿丶"] = {"实"};
    // 践
    m_strokeMap["丨乛一丨一丨一一一丿丶"] = {"践"};
    // 真
    m_strokeMap["一丨丨一丨乛一一一丿丶"] = {"真"};
    // 假
    m_strokeMap["丿丨一丨乛一一丨一乛乛乛"] = {"假"};
    // 好
    m_strokeMap["乛丿一乛一一"] = {"好"};
    // 坏
    m_strokeMap["一丨一丶丿丶丶"] = {"坏"};
    // 美
    m_strokeMap["丶丿一一一丨一一一丿丶"] = {"美"};
    // 丽
    m_strokeMap["一丨乛丶丨乛一"] = {"丽"};
    // 漂
    m_strokeMap["丶丶一一丨乛一一一丨丿丶"] = {"漂"};
    // 亮
    m_strokeMap["丶一丨乛一丨乛一一丿乛"] = {"亮"};
    // 黑
    m_strokeMap["丨乛一丶丶丶丶丶丶丶"] = {"黑"};
    // 白
    m_strokeMap["丿丨乛一一"] = {"白"};
    // 红
    m_strokeMap["乛乛一一丨一"] = {"红"};
    // 黄
    m_strokeMap["一丨丨一丨乛一一一丿丶"] = {"黄"};
    // 绿
    m_strokeMap["乛乛一一乛一一丨丿丶"] = {"绿"};
    // 蓝
    m_strokeMap["一丨丨一丨丨一一丨一一"] = {"蓝"};
    // 紫
    m_strokeMap["丨一丨一丿乛一一丨丿丶"] = {"紫"};
    // 青
    m_strokeMap["一一一丨一丨乛一一"] = {"青"};
    // 年
    m_strokeMap["丿一一一丨丨"] = {"年"};
    // 岁
    m_strokeMap["丨乛一丨一丿乛丶"] = {"岁"};
    // 月
    m_strokeMap["丿乛一一"] = {"月"};
    // 日
    m_strokeMap["丨乛一一"] = {"日"};
    // 星
    m_strokeMap["丨乛一一丿一一丨一"] = {"星"};
    // 期
    m_strokeMap["一丨丨一一一丿乛一一"] = {"期"};
    // 时
    m_strokeMap["丨乛一一一丨丶"] = {"时"};
    // 间
    m_strokeMap["丨乛一一一丨乛一一"] = {"间"};
    // 今
    m_strokeMap["丿丶丶乛"] = {"今"};
    // 天
    m_strokeMap["一一一丿丶"] = {"天"};
    // 明
    m_strokeMap["丨乛一一丿乛一一"] = {"明"};
    // 昨
    m_strokeMap["丨乛一一丿一丨一"] = {"昨"};
    // 晚
    m_strokeMap["丨乛一一丨乛丿丿一乛"] = {"晚"};
    // 早
    m_strokeMap["丨乛一一一丨"] = {"早"};
    // 中
    m_strokeMap["丨乛一丨"] = {"中"};
    // 午
    m_strokeMap["丿一一十"] = {"午"};
    // 晚
    m_strokeMap["丨乛一一丨乛丿丿一乛"] = {"晚"};
    // 夜
    m_strokeMap["丶一丿丶丨乛丶丶"] = {"夜"};
    // 饭
    m_strokeMap["乛丶一丿一乛"] = {"饭"};
    // 吃
    m_strokeMap["丨乛一丿一乛"] = {"吃"};
    // 喝
    m_strokeMap["丨乛一丨乛一丿乛丶丶"] = {"喝"};
    // 酒
    m_strokeMap["丶丶一一丨乛一一丨乛一一"] = {"酒"};
    // 茶
    m_strokeMap["一丨丨一丿丶一丨丿丶"] = {"茶"};
    // 水
    m_strokeMap["丨乛丿丶"] = {"水"};
    // 果
    m_strokeMap["丨乛一一一丨一丿丶"] = {"果"};
    // 菜
    m_strokeMap["一丨丨一丿丶丶一丨丿丶"] = {"菜"};
    // 米
    m_strokeMap["丶丿一丨丿丶"] = {"米"};
    // 面
    m_strokeMap["一丿丨乛丨丨一一一"] = {"面"};
    // 肉
    m_strokeMap["丨乛丿丶丿丶"] = {"肉"};
    // 鱼
    m_strokeMap["丿乛丨乛一一"] = {"鱼"};
    // 虾
    m_strokeMap["丨乛一丨一丨丶"] = {"虾"};
    // 蛋
    m_strokeMap["乛丿丶丨乛一一丨一丨一"] = {"蛋"};
    // 奶
    m_strokeMap["乛丿一乛丿"] = {"奶"};
    // 牛
    m_strokeMap["丿一一一丨"] = {"牛"};
    // 羊
    m_strokeMap["丶丿一一一丨"] = {"羊"};
    // 猪
    m_strokeMap["丿一丿丿一丨乛一一"] = {"猪"};
    // 狗
    m_strokeMap["丿一丿丿一丨乛一"] = {"狗"};
    // 猫
    m_strokeMap["丿一丿丿一丨丨丨乛一一"] = {"猫"};
    // 鸡
    m_strokeMap["乛丶一丿乛丶乛一一"] = {"鸡"};
    // 鸭
    m_strokeMap["丨乛一一丨一丿乛丶乛一一"] = {"鸭"};
    // 马
    m_strokeMap["乛一一"] = {"马"};
    // 鸟
    m_strokeMap["丿乛丶乛一一"] = {"鸟"};
    // 虫
    m_strokeMap["丨乛一丨丶"] = {"虫"};
    // 花
    m_strokeMap["一丨丨一丿乛"] = {"花"};
    // 草
    m_strokeMap["一丨丨一丨乛一一一丨"] = {"草"};
    // 树
    m_strokeMap["一丨丿丶一丨乛一一丨丶"] = {"树"};
    // 叶
    m_strokeMap["丨乛一十"] = {"叶"};
    // 根
    m_strokeMap["一丨丿丶一丨乛一一"] = {"根"};
    // 本
    m_strokeMap["一丨丿丶一"] = {"本"};
    // 木
    m_strokeMap["一丨丿丶"] = {"木"};
    // 林
    m_strokeMap["一丨丿丶一丨丿丶"] = {"林"};
    // 森
    m_strokeMap["一丨丿丶一丨丿丶一丨丿丶"] = {"森"};
    // 土
    m_strokeMap["一丨一"] = {"土"};
    // 地
    m_strokeMap["一丨一乛丨乛一"] = {"地"};
    // 山
    m_strokeMap["丨乛丨"] = {"山"};
    // 水
    m_strokeMap["丨乛丿丶"] = {"水"};
    // 火
    m_strokeMap["丶丿丿丶"] = {"火"};
    // 风
    m_strokeMap["丿乛丿丶"] = {"风"};
    // 云
    m_strokeMap["一一乛丶"] = {"云"};
    // 雨
    m_strokeMap["一丨乛丨丶丶丶丶"] = {"雨"};
    // 雪
    m_strokeMap["一丨乛丨丶丶丶丶乛一一"] = {"雪"};
    // 天
    m_strokeMap["一一一丿丶"] = {"天"};
    // 气
    m_strokeMap["丿一一乛"] = {"气"};
    // 温
    m_strokeMap["丶丶一丨乛一一丨一一"] = {"温"};
    // 度
    m_strokeMap["丶一丿一丨乛一丨乛乛丶"] = {"度"};
    // 热
    m_strokeMap["一丨一一一丿丶丶丶丶丶"] = {"热"};
    // 冷
    m_strokeMap["丶一丿丶丶丶乛丶"] = {"冷"};
    // 春
    m_strokeMap["一一一丿丶丨乛一一"] = {"春"};
    // 夏
    m_strokeMap["一丿丨乛一一丿乛乛乛"] = {"夏"};
    // 秋
    m_strokeMap["丿一丨丿丶火"] = {"秋"};
    // 冬
    m_strokeMap["丿乛丶丶丶"] = {"冬"};
    // 医
    m_strokeMap["一丿一丨一乛一丿丶"] = {"医"};
    // 生
    m_strokeMap["丿一一一丨一"] = {"生"};
    // 院
    m_strokeMap["乛丨丶丶乛一一一丿乛"] = {"院"};
    // 病
    m_strokeMap["丶一丿丶丨乛一一丨丿丶"] = {"病"};
    // 药
    m_strokeMap["一丨丨一乛乛一一丶"] = {"药"};
    // 房
    m_strokeMap["丶一丿丶一丿一一"] = {"房"};
    // 间
    m_strokeMap["丨乛一一一丨乛一一"] = {"间"};
    // 门
    m_strokeMap["丶丨乛丨"] = {"门"};
    // 窗
    m_strokeMap["丶丶乛丿丶丿乛一"] = {"窗"};
    // 床
    m_strokeMap["丶一丿丶一丨丿丶"] = {"床"};
    // 桌
    m_strokeMap["丨一丨一一一丨丿丶"] = {"桌"};
    // 椅
    m_strokeMap["一丨丿丶大丨乛一一"] = {"椅"};
    // 书
    m_strokeMap["乛乛丨丶"] = {"书"};
    // 包
    m_strokeMap["丿乛一一乛乛"] = {"包"};
    // 笔
    m_strokeMap["丿一一丨一丿乛"] = {"笔"};
    // 本
    m_strokeMap["一丨丿丶一"] = {"本"};
    // 纸
    m_strokeMap["乛乛一一丿一乛"] = {"纸"};
    // 报
    m_strokeMap["一丨一丨乛乛丨乛丶"] = {"报"};
    // 名
    m_strokeMap["丿乛丶丨乛一"] = {"名"};
    // 字
    m_strokeMap["丶丶乛乛一"] = {"字"};
    // 词
    m_strokeMap["丶乛一丨乛一丨乛一"] = {"词"};
    // 句
    m_strokeMap["丿乛丨乛一"] = {"句"};
    // 文
    m_strokeMap["丶一丿丶"] = {"文"};
    // 章
    m_strokeMap["丶一丨乛一一丨丨一丨"] = {"章"};
    // 篇
    m_strokeMap["丿一一丨丶一丿丶丨乛一一"] = {"篇"};
    // 节
    m_strokeMap["一丨丨一乛丨"] = {"节"};
    // 目
    m_strokeMap["丨乛一一一"] = {"目"};
    // 录
    m_strokeMap["乛一一一丨丿丶"] = {"录"};
    // 影
    m_strokeMap["丨乛一一丶丨一一丨丿丶"] = {"影"};
    // 视
    m_strokeMap["丶丶一丨乛一一丿乛"] = {"视"};
    // 听
    m_strokeMap["丨乛一丿一丨"] = {"听"};
    // 声
    m_strokeMap["一丨一乛一一丨"] = {"声"};
    // 音
    m_strokeMap["丶一丨乛一一丨乛一一"] = {"音"};
    // 乐
    m_strokeMap["丿乛丨丿丶"] = {"乐"};
    // 歌
    m_strokeMap["一丨乛一丨一丨乛一一丨丿丶"] = {"歌"};
    // 舞
    m_strokeMap["丿一一丨一丨一一丨一丨一"] = {"舞"};
    // 体
    m_strokeMap["丿丨一一丨一"] = {"体"};
    // 育
    m_strokeMap["丶一乛丶丨乛一一"] = {"育"};
    // 运
    m_strokeMap["一一乛丶丶乛丶"] = {"运"};
    // 动
    m_strokeMap["一一一乛丨一乛丿"] = {"动"};
    // 跑
    m_strokeMap["丨乛一丨一丿乛丶丿乛一乛"] = {"跑"};
    // 步
    m_strokeMap["丨一丨一一丿丿丶"] = {"步"};
    // 行
    m_strokeMap["丿丿一一一丨"] = {"行"};
    // 走
    m_strokeMap["一丨一丨一乛丿丶"] = {"走"};
    // 停
    m_strokeMap["丿丨一丶一丨乛一一丨"] = {"停"};
    // 站
    m_strokeMap["丶一丿一一丨一丨乛一"] = {"站"};
    // 坐
    m_strokeMap["丿丶丶丿丶一丨一"] = {"坐"};
    // 躺
    m_strokeMap["丿丨乛一一丿丿丨乛一一"] = {"躺"};
    // 睡
    m_strokeMap["丨乛一一丿一一一丨一一"] = {"睡"};
    // 穿
    m_strokeMap["丶丶乛一丨一乛丿"] = {"穿"};
    // 衣
    m_strokeMap["丶一丿乛丿丶"] = {"衣"};
    // 服
    m_strokeMap["丿乛一一丨乛乛丶"] = {"服"};
    // 鞋
    m_strokeMap["一丨丨一丨一一丨一丨一"] = {"鞋"};
    // 帽
    m_strokeMap["丨乛一一丨乛一一丨乛一一"] = {"帽"};
    // 头
    m_strokeMap["丶丶一丿丶"] = {"头"};
    // 发
    m_strokeMap["乛丿丶乛丨丶"] = {"发"};
    // 脸
    m_strokeMap["丿乛一一一丨一一一丿丶"] = {"脸"};
    // 眼
    m_strokeMap["丨乛一一一丨乛一一一乛丶"] = {"眼"};
    // 睛
    m_strokeMap["丨乛一一一一一丨乛一一"] = {"睛"};
    // 鼻
    m_strokeMap["丿丨乛一一一丨乛一一一丨丨"] = {"鼻"};
    // 嘴
    m_strokeMap["丨乛一丨一乛一丨一一丨丨"] = {"嘴"};
    // 手
    m_strokeMap["丿一一一乛"] = {"手"};
    // 脚
    m_strokeMap["丿乛一一一丨一丿乛丶丨"] = {"脚"};
    // 指
    m_strokeMap["一丨一丿乛一一丨乛一一"] = {"指"};
    // 朋
    m_strokeMap["丿乛一一丿乛一一"] = {"朋"};
    // 友
    m_strokeMap["一丿乛乛丶"] = {"友"};
    // 同
    m_strokeMap["丨乛一丨乛一"] = {"同"};
    // 学
    m_strokeMap["丶丶丿丶丿乛乛一一丨"] = {"学"};
    // 校
    m_strokeMap["一丨丿丶丶一丿丶"] = {"校"};
    // 班
    m_strokeMap["一一一丶一丿一一一"] = {"班"};
    // 级
    m_strokeMap["乛乛一一丨一丿丶"] = {"级"};
    // 教
    m_strokeMap["一丿一丨一丿乛丨一"] = {"教"};
    // 师
    m_strokeMap["丨丿一丨乛一一丨"] = {"师"};
    // 问
    m_strokeMap["丨乛一丨乛一"] = {"问"};
    // 答
    m_strokeMap["丿丶一丿一丨乛一"] = {"答"};
    // 考
    m_strokeMap["一丿一一丨一乛"] = {"考"};
    // 试
    m_strokeMap["丶乛一一丨一丨一"] = {"试"};
    // 成
    m_strokeMap["一丿乛一一丿丶"] = {"成"};
    // 绩
    m_strokeMap["乛乛一一一一丨一丿丶"] = {"绩"};
    // 分
    m_strokeMap["丿丶乛丿"] = {"分"};
    // 数
    m_strokeMap["丶一丨乛一丨一丨一丿丶"] = {"数"};
    // 科
    m_strokeMap["丿一丨丿丶丶一十"] = {"科"};
    // 学
    m_strokeMap["丶丶丿丶丿乛乛一一丨"] = {"学"};
    // 语
    m_strokeMap["丶乛一丨乛一一丨乛一"] = {"语"};
    // 英
    m_strokeMap["一丨丨一乛丨乛一一丿丶"] = {"英"};
    // 国
    m_strokeMap["丨乛一一丨丶一"] = {"国"};
    // 家
    m_strokeMap["丶丶乛一丿一一丨丿丶"] = {"家"};
    // 市
    m_strokeMap["丶一丨乛丨"] = {"市"};
    // 省
    m_strokeMap["丨丿丶丿丨乛一一一"] = {"省"};
    // 区
    m_strokeMap["一丿丶乛"] = {"区"};
    // 县
    m_strokeMap["丨乛一一一乛丶"] = {"县"};
    // 镇
    m_strokeMap["丿一一丶一丨丨一一一丿丶"] = {"镇"};
    // 村
    m_strokeMap["一丨丿丶一丨丶"] = {"村"};
    // 城
    m_strokeMap["一丨一一丿乛一一丿丶"] = {"城"};
    // 路
    m_strokeMap["丨乛一丨一丿乛丶丶丨乛一"] = {"路"};
    // 街
    m_strokeMap["丿丿一一一丨一丨一丨一"] = {"街"};
    // 道
    m_strokeMap["丶丿一丨乛一一一丨丨丶乛丶"] = {"道"};
    // 车
    m_strokeMap["一乛一丨"] = {"车"};
    // 站
    m_strokeMap["丶一丿一一丨一丨乛一"] = {"站"};
    // 飞
    m_strokeMap["乛丿丶"] = {"飞"};
    // 机
    m_strokeMap["一丨丿丶几"] = {"机"};
    // 船
    m_strokeMap["丿乛一一丨丨乛一"] = {"船"};
    // 票
    m_strokeMap["丨乛一一一丨一一丨乛丶"] = {"票"};
    // 钱
    m_strokeMap["丿一一丶一丨一一一丿丶"] = {"钱"};
    // 银
    m_strokeMap["丿一一丶一丨乛一一"] = {"银"};
    //银
    m_strokeMap["丿一一丶一丨乛一一"] = {"银"};
    //金
    m_strokeMap["丿丶一一丨乛一"] = {"金"};
    //钱
    m_strokeMap["丿一一丶一丨一一一丿丶"] = {"钱"};
    //价
    m_strokeMap["丿丨一丨一丨丨"] = {"价"};
    //格
    m_strokeMap["一丨丿丶丿乛丶丨乛一"] = {"格"};
    //买
    m_strokeMap["乛乛丶一丿丶"] = {"买"};
    //卖
    m_strokeMap["一丨乛丶一丿丶"] = {"卖"};
    //花
    m_strokeMap["一丨丨一丿乛"] = {"花"};
    //费
    m_strokeMap["乛一一丨丿乛丿丶"] = {"费"};
    //贵
    m_strokeMap["丨乛一一丨一丨一丿丶"] = {"贵"};
    //便
    m_strokeMap["丿丨一一一丨乛一"] = {"便"};
    //宜
    m_strokeMap["丶丶乛丨乛一一一"] = {"宜"};
    //免
    m_strokeMap["丿乛丨乛一丿乛"] = {"免"};
    //店
    m_strokeMap["丶一丿丶丨一丨乛一"] = {"店"};
    //饭
    m_strokeMap["乛丶一丿一乛"] = {"饭"};
    //馆
    m_strokeMap["乛丶一丶丶丨乛一一"] = {"馆"};
    //宾
    m_strokeMap["丶丶乛丿一丨一丿丶"] = {"宾"};
    //客
    m_strokeMap["丶丶乛丿乛丶丨乛一"] = {"客"};
    //房
    m_strokeMap["丶一丿丶一丿一一"] = {"房"};
    //间
    m_strokeMap["丨乛一一一丨乛一一"] = {"间"};
    //门
    m_strokeMap["丶丨乛丨"] = {"门"};
    //窗
    m_strokeMap["丶丶乛丿丶丿乛一"] = {"窗"};
    //开
    m_strokeMap["一一丨丨"] = {"开"};
    //关
    m_strokeMap["丶丿一一一丿丶"] = {"关"};
    //灯
    m_strokeMap["火一丨"] = {"灯"};
    //开
    m_strokeMap["一一丨丨"] = {"开"};
    //始
    m_strokeMap["乛丿丶乛丶丨乛一"] = {"始"};
    //结
    m_strokeMap["乛乛一一丨一丨乛一"] = {"结"};
    //束
    m_strokeMap["一丨乛一一丨丿丶"] = {"束"};
    //完
    m_strokeMap["丶丶乛一一丿乛"] = {"完"};
    //成
    m_strokeMap["一丿乛一一丿丶"] = {"成"};
    //了
    m_strokeMap["乛丨"] = {"了"};

    // Additional common characters
}

QStringList StrokeRecognition::matchStroke(const QString &strokeSequence)
{
    if (strokeSequence.isEmpty()) {
        return {};
    }

    // exact match first
    if (m_strokeMap.contains(strokeSequence)) {
        return m_strokeMap[strokeSequence];
    }

    // prefix match
    QStringList results;
    for (const QString &key : m_strokeMap.keys()) {
        if (key.startsWith(strokeSequence)) {
            results.append(m_strokeMap[key]);
        }
    }

    // deduplicate and limit to 20 results
    QSet<QString> uniqueResults;
    for (const QString &r : results) {
        uniqueResults.insert(r);
    }
    QStringList sorted = uniqueResults.values();
    sorted.sort();
    return sorted.mid(0, 20);
}

void QmlStrokePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<StrokeRecognition>(uri, 1, 0, "StrokeRecognition");
}
