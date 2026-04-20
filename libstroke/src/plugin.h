#ifndef PLUGIN_H
#define PLUGIN_H

#pragma once

#include <maliit/abstractinputmethod.h>

class StrokeInputMethod : public Maliit::AbstractInputMethod
{
    Q_OBJECT

public:
    StrokeInputMethod(QObject *parent = nullptr);

    void keyClicked(int key, Qt::KeyboardModifiers modifiers) override;
    void reset() override;
};

#endif // PLUGIN_H
