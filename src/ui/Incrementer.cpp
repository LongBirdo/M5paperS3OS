#include "ui/Incrementer.h"

namespace ui {

Incrementer::Incrementer(int x, int y, int width, int height)
    : _x(x), _y(y), _width(width), _height(height) {}

void Incrementer::drawTriangleButton(int btnX, int btnY, bool isUp) {
    if (isUp) {
        M5.Display.fillTriangle(btnX, btnY + _height, btnX + (_width / 2), btnY, btnX + _width, btnY + _height, BLACK);
    } else {
        M5.Display.fillTriangle(btnX, btnY, btnX + (_width / 2), btnY + _height, btnX + _width, btnY, BLACK);
    }
}

void Incrementer::draw(int value) {
    M5.Display.fillRect(_x, _y, _width, _height * 3, WHITE);

    drawTriangleButton(_x, _y, true);

    M5.Display.fillRect(_x, _y + _height + 10, _width, _height, DARKGREY);
    
    drawTriangleButton(_x, _y + _height * 2 + 20, false);
    
    drawValue(value);
}

// Draws only the value and its background
void Incrementer::drawValue(int value) {
    M5.Display.fillRect(_x, _y + _height + 10, _width, _height, DARKGREY);

    M5.Display.setTextColor(WHITE);
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    
    int textX = _x + (_width / 2);
    int textY = _y + _height + 20;
    M5.Display.drawCentreString(String(value), textX, textY);
}

bool Incrementer::isIncrementPressed(int touchX, int touchY) {
    return (touchX >= _x && touchX <= _x + _width && touchY >= _y && touchY <= _y + _height);
}

bool Incrementer::isDecrementPressed(int touchX, int touchY) {
    return (touchX >= _x && touchX <= _x + _width && touchY >= _y + _height * 2 + 20 && touchY <= _y + _height * 3 + 20);
}

} 