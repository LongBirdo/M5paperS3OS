#include "PianoApp.h"
#include "AppManager.h"
#include "ui/Keyboard.h"
#include <SD.h>
#include <ArduinoJson.h>
#include <iostream>

PianoApp::PianoApp(AppManager *manager) 
    : appManager(manager), _keyboard(0, 0, 960, 540) {}

void PianoApp::setup() {
    M5.Display.setRotation(2);
    _currentText = "Touch anywhere to open keyboard";
}

void PianoApp::draw() {
    if (_needsRedraw || _keyboard.isVisible()) {
        if (!_keyboard.isVisible()) {
            M5.Display.clear(WHITE); 
            M5.Display.setFont(&fonts::FreeSansBold18pt7b);
            M5.Display.setTextColor(BLACK);
            M5.Display.drawCentreString("PIANO APP", 270, 100);

            M5.Display.setFont(&fonts::FreeSansBold12pt7b);
            M5.Display.drawCentreString("Touch screen to type", 270, 300);
            M5.Display.drawCentreString(_currentText.c_str(), 270, 400);

            M5.Display.fillRoundRect(100, 500, 340, 300, 20, WHITE);
            M5.Display.drawRoundRect(100, 500, 340, 300, 20, BLACK);
        }

        _keyboard.draw();
        _needsRedraw = false;
    }
}
void PianoApp::handleTouch(int x, int y) {
    if (_keyboard.isVisible()) {
        _keyboard.handleTouch(x, y);
        return;
    }

    _keyboard.show([this](const std::string& text) {
        _currentText = text.empty() ? "You typed nothing" : "You typed: " + text;
        _needsRedraw = true;
    });
}
void PianoApp::update() {
    // MUST exist
}