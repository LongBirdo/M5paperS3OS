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
    // Let the keyboard draw itself + handle input if it's open
    if (_keyboard.drawAndHandleInput("Touch to name your song")) {
        return;  // Keyboard is active → it drew everything, we're done
    }

    // Keyboard is closed → draw normal Piano UI
    M5.Display.clear(WHITE);

    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.setTextColor(BLACK);
    M5.Display.drawCentreString("PIANO APP", 480, 100);

    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    if (_currentText.empty()) {
        M5.Display.setTextColor(DARKGREY);
        M5.Display.drawCentreString("No name yet", 480, 300);
    } else {
        M5.Display.setTextColor(BLACK);
        M5.Display.drawCentreString(_currentText.c_str(), 480, 300);
    }

    // Your piano keys or whatever
    M5.Display.fillRoundRect(100, 500, 340, 300, 20, WHITE);
    M5.Display.drawRoundRect(100, 500, 340, 300, 20, BLACK);
}

void PianoApp::handleTouch(int x, int y) {
    if (_keyboard.isVisible()) {
        _keyboard.handleTouch(x, y);
        return;
    }

    // Tap anywhere → open keyboard
    _keyboard.show([this](const std::string& text) {
        _currentText = text;
        _needsRedraw = true;
    });
}

void PianoApp::update() {
    // MUST exist
}