#ifndef INCREMENTER_H
#define INCREMENTER_H

#include <M5Unified.h>

namespace ui {

class Incrementer {
public:
    Incrementer(int x, int y, int width, int height);

    void draw(int value);
    void drawValue(int value); 
    bool isIncrementPressed(int touchX, int touchY);
    bool isDecrementPressed(int touchX, int touchY);

    int getX() const { return _x; }
    int getY() const { return _y; }
    int getWidth() const { return _width; }

private:
    int _x, _y, _width, _height;
    void drawTriangleButton(int btnX, int btnY, bool isUp);
};

} 

#endif 