#pragma once
#include <M5Unified.h>
#include <functional>
#include <string>
#include <vector>

namespace ui {

class Keyboard {
public:
    using Callback = std::function<void(const std::string&)>;
    bool drawAndHandleInput(const std::string& placeholder = "Type here...");

    Keyboard(int x = 0, int y = 0, int w = 960, int h = 540);

    void show(Callback onDone = nullptr);
    void hide();
    bool isVisible() const { return _visible; }
    void draw();
    void handleTouch(int x, int y);

private:
    void drawTextBox();
    void updateTextBox();
    void drawKeyboard();
    void drawKey(int x, int y, int w, int h, const String& label, uint16_t color = DARKGREY);

    void appendChar(char c);
    void backspace();
    void enter();
    void toggleShift();
    void toggleMode(); 

    int _x, _y, _w, _h;
    bool _visible = false;
    bool _needsRedraw = true;
    std::string _text;
    Callback _onDone;

    bool _shift = false; 
    bool _capsLock = false; 
    bool _numbersMode = false; 

    const std::vector<std::string> _letterRows = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };

    const std::vector<std::string> _numberRows = {
        "1234567890",
        "-/:;()$&@",
        ".,?!'\""
    };
};

}