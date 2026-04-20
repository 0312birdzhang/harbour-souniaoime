#include "stroke.h"
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
    // 初始化笔画映射表
    // 格式：笔画序列 -> 汉字列表
    
    // 横（一）
    m_strokeMap["一"] = {"一", "乙"};
    
    // 竖（丨）
    m_strokeMap["丨"] = {"丨", "十", "丁"};
    
    // 撇（丿）
    m_strokeMap["丿"] = {"丿", "八", "人"};
    
    // 捺（丶）
    m_strokeMap["丶"] = {"丶", "卜", "又"};
    
    // 折（乛）
    m_strokeMap["乛"] = {"乛", "了", "子"};
    
    // 两笔画
    m_strokeMap["一一"] = {"二", "工", "土"};
    m_strokeMap["一丨"] = {"十", "干", "千"};
    m_strokeMap["一丿"] = {"厂", "广", "户"};
    m_strokeMap["一丶"] = {"卜", "又", "丈"};
    m_strokeMap["一乛"] = {"七", "刀", "力"};
    
    m_strokeMap["丨一"] = {"丁", "下", "万"};
    m_strokeMap["丨丨"] = {"川", "州", "巡"};
    m_strokeMap["丨丿"] = {"儿", "几", "九"};
    m_strokeMap["丨丶"] = {"卜", "又", "丈"};
    m_strokeMap["丨乛"] = {"了", "子", "孔"};
    
    // 三笔画
    m_strokeMap["一一一"] = {"三", "王", "丰"};
    m_strokeMap["一丨一"] = {"土", "士", "干"};
    m_strokeMap["一丿一"] = {"厂", "广", "户"};
    m_strokeMap["一丶一"] = {"下", "万", "丈"};
    m_strokeMap["一乛一"] = {"七", "刀", "力"};
    
    m_strokeMap["丨一一"] = {"丁", "下", "万"};
    m_strokeMap["丨丨一"] = {"川", "州", "巡"};
    m_strokeMap["丨丿一"] = {"儿", "几", "九"};
    m_strokeMap["丨丶一"] = {"卜", "又", "丈"};
    m_strokeMap["丨乛一"] = {"了", "子", "孔"};
    
    // 四笔画
    m_strokeMap["一一一一"] = {"四", "西", "酉"};
    m_strokeMap["一丨一丨"] = {"日", "目", "田"};
    m_strokeMap["一丿一丿"] = {"从", "林", "森"};
    m_strokeMap["一丶一丶"] = {"双", "轰", "聂"};
    m_strokeMap["一乛一乛"] = {"习", "羽", "翌"};
    
    // 常用汉字的笔画序列
    m_strokeMap["一丨一丨"] = {"日"};
    m_strokeMap["丨フ一一"] = {"口"};
    m_strokeMap["ノ丶一一丨一"] = {"我"};
    m_strokeMap["一丨丨一一丨フ一一"] = {"国"};
    m_strokeMap["ノ丨一一一丨一一"] = {"你"};
    m_strokeMap["一丨一フ丨一一"] = {"中"};
    m_strokeMap["丶一ノ丶一一丨"] = {"文"};
    m_strokeMap["一丨丨一一フ丶"] = {"天"};
    m_strokeMap["一丨ノ丶"] = {"木"};
    m_strokeMap["丶フ一丨ノ丶"] = {"鸟"};
}

QStringList StrokeRecognition::matchStroke(const QString &strokeSequence)
{
    if (strokeSequence.isEmpty()) {
        return {};
    }
    
    // 直接匹配
    if (m_strokeMap.contains(strokeSequence)) {
        return m_strokeMap[strokeSequence];
    }
    
    // 部分匹配（前缀匹配）
    QStringList results;
    for (const QString &key : m_strokeMap.keys()) {
        if (key.startsWith(strokeSequence)) {
            results.append(m_strokeMap[key]);
        }
    }
    
    // 去重并限制数量
    QSet<QString> uniqueResults(results.begin(), results.end());
    return uniqueResults.values().mid(0, 10); // 最多返回10个结果
}

void QmlStrokePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<StrokeRecognition>(uri, 1, 0, "StrokeRecognition");
}
