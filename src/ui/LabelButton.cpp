#include "ui/LabelButton.h"

namespace ui {

LabelButton::LabelButton(int x, int y, int w, int h, const std::string& label)
    : _x(x), _y(y), _w(w), _h(h), _label(label) {}

void LabelButton::draw() {
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.setTextColor(BLACK);

    M5.Display.fillRect(_x, _y, _w, _h, WHITE);
    
    M5.Display.drawRect(_x, _y, _w, _h, BLACK);

    int centerX = _x + (_w / 2);
    int centerY = _y + (_h / 2) - 8; 

    M5.Display.drawCentreString(_label.c_str(), centerX, centerY);
}

bool LabelButton::isPressed(int touchX, int touchY) {
    return (touchX >= _x && touchX <= _x + _w && touchY >= _y && touchY <= _y + _h);
}

const std::string& LabelButton::getLabel() const {
    return _label;
}

} 