#ifndef LABELBUTTON_H
#define LABELBUTTON_H

#include <M5Unified.h>
#include <string>

namespace ui {

class LabelButton {
public:
    LabelButton(int x, int y, int w, int h, const std::string& label);

    void draw();
    bool isPressed(int touchX, int touchY);
    const std::string& getLabel() const;

private:
    int _x, _y, _w, _h;
    std::string _label;
};

}

#endif 