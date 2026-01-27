// src/ui/Keyboard.cpp
#include "ui/Keyboard.h"

namespace ui {

Keyboard::Keyboard(int x, int y, int w, int h)
    : _x(x), _y(y), _w(w), _h(h) {}

bool Keyboard::drawAndHandleInput(const std::string& placeholder) {
    // If keyboard is open → take over everything
    if (_visible) {
        if (_needsRedraw) {
            M5.Display.setRotation(1);
            M5.Display.clear(WHITE);
            drawTextBox();           // shows current text or placeholder
            drawKeyboard();
            _needsRedraw = false;
        }
        draw();  // will only redraw if needed
        return true;  // "I handled drawing and touch"
    }

    // Keyboard not open → show placeholder text in your app
    M5.Display.setTextColor(DARKGREY);
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.drawCentreString(placeholder.c_str(), 480, 400);
    return false;
}

void Keyboard::show(Callback onDone) {
    _visible = true;
    _text.clear();
    _onDone = std::move(onDone);
    _shift = false;
    _capsLock = false;
    _numbersMode = false;
    _needsRedraw = true;

    M5.Display.setRotation(1);
    M5.Display.clear(WHITE);
    draw();
}

void Keyboard::hide() {
    _visible = false;
    M5.Display.setRotation(2);
    if (_onDone) _onDone(_text);
}

void Keyboard::draw() {
    if (!_visible || !_needsRedraw) return;

    M5.Display.startWrite();
    drawTextBox();
    drawKeyboard();
    M5.Display.endWrite();
    _needsRedraw = false;
}

void Keyboard::updateTextBox() {
    M5.Display.startWrite();
    drawTextBox();
    M5.Display.endWrite();
}

void Keyboard::drawTextBox() {
    M5.Display.fillRect(0, 0, 960, 140, WHITE);
    M5.Display.drawRect(0, 0, 960, 140, BLACK);
    M5.Display.setTextColor(BLACK);
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.setTextWrap(true);
    M5.Display.setCursor(30, 50);
    M5.Display.print(_text.empty() ? "Type here..." : _text.c_str());
}

void Keyboard::drawKeyboard() {
    const int keyH = 90;
    const int keyW = 88;
    const int gap  = 5;
    int y = 160;
    int leftx = 20; //was 30

    const auto& rows = _numbersMode ? _numberRows : _letterRows;

    // Row 1
    for (int i = 0; i < 10; ++i) {
        int x = leftx + i * (keyW + gap);
        String label = String(rows[0][i]);
        if (!_numbersMode && (_capsLock || _shift)) label.toUpperCase();
        drawKey(x, y, keyW, keyH, label);
    }

    // Row 2
    y += keyH + gap;
    for (int i = 0; i < (int)rows[1].length(); ++i) {
        int x = leftx + keyW/2 + i * (keyW + gap);
        String label = String(rows[1][i]);
        if (!_numbersMode && (_capsLock || _shift)) label.toUpperCase();
        drawKey(x, y, keyW, keyH, label);
    }

    // Row 3 — Shift + letters + Enter
    y += keyH + gap;

    drawKey(leftx, y, keyW + 10, keyH, "Shift", (_shift || _capsLock) ? WHITE : DARKGREY);

    int letterX = leftx + keyW + gap;
    for (int i = 0; i < (int)rows[2].length(); ++i) {
        int x = letterX + i * (keyW + gap);
        String label = String(rows[2][i]);
        if (!_numbersMode && (_capsLock || _shift)) label.toUpperCase();
        drawKey(x, y, keyW, keyH, label);
    }

    drawKey(830, y, keyW + leftx, keyH, "Enter", RED);

    // Bottom row — 123/ABC (left), Space (center), Del (right)
    y += keyH + gap;

    drawKey(leftx, y, keyW * 1.8, keyH, _numbersMode ? "ABC" : "123",
            _numbersMode ? WHITE : DARKGREY);

    drawKey(200, y, 560, keyH, "Space", DARKGREY);
    drawKey(780, y, 150, keyH, "Del", RED);
}

void Keyboard::drawKey(int x, int y, int w, int h, const String& label, uint16_t color) {
    M5.Display.fillRoundRect(x, y, w, h, 14, color);
    M5.Display.setTextColor(color == WHITE ? BLACK : WHITE);
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    if (label.length() > 4) M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.drawCentreString(label, x + w/2, y + h/2 - 12);
}

void Keyboard::handleTouch(int x, int y) {
    if (!_visible || y < 160) return;

    const int keyH = 90;
    const int keyW = 88;
    const int gap  = 5;
    const int rowHeight = keyH + gap;
    int leftx = 20; //was 30

    int row = (y - 160) / rowHeight;

    const auto& rows = _numbersMode ? _numberRows : _letterRows;

    if (row == 0) {
        int col = (x - leftx) / (keyW + gap);
        if (col >= 0 && col < 10) appendChar(rows[0][col]);
    }
    else if (row == 1) {
        int col = (x - leftx - keyW/2) / (keyW + gap);
        if (col >= 0 && col < (int)rows[1].length()) appendChar(rows[1][col]);
    }
    else if (row == 2) {
        if (x < 120) toggleShift();           // Shift
        else if (x > 830) enter();            // Enter
        else {
            int col = (x - (leftx + keyW + gap)) / (keyW + gap);
            if (col >= 0 && col < (int)rows[2].length()) appendChar(rows[2][col]);
        }
    }
    else if (row >= 3) {
        if (x < 200) toggleMode();            // 123/ABC toggle — bottom-left
        else if (x > 750) backspace();        // Del
        else if (x > 180 && x < 760) appendChar(' '); // Space
    }
}

void Keyboard::appendChar(char c) {
    if (!_numbersMode && isalpha(c)) {
        c = (_capsLock || _shift) ? toupper(c) : tolower(c);
        if (_shift && !_capsLock) _shift = false;
    }
    _text += c;
    updateTextBox();
}

void Keyboard::backspace() {
    if (!_text.empty()) _text.pop_back();
    updateTextBox();
}

void Keyboard::enter() {
    hide();
}

void Keyboard::toggleShift() {
    _shift = !_shift;
    // double-tap = caps lock
    //if (_shift) _capsLock = !_capsLock;  DOESNT WORK
    _needsRedraw = true;
    draw();
}

void Keyboard::toggleMode() {
    M5.Display.clear();
    _numbersMode = !_numbersMode;
    _needsRedraw = true;
    draw();
}

}