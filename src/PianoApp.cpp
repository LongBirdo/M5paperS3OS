#include "AppManager.h"
#include "PianoApp.h"
#include <SD.h>
#include <ArduinoJson.h> 
#include <iostream>

PianoApp::PianoApp(AppManager* manager) : appManager(manager) {
    // Add any initialization here if needed (or leave empty)
}

void PianoApp::setup() {
    // Add setup logic here if needed (or leave empty)
}

void PianoApp::update() {
    // Add update logic here if needed (or leave empty)
}

void PianoApp::draw() {
    M5.Display.clear();
    M5.Display.fillRect(20, 40, 40, 40, WHITE);
    M5.Display.drawRect(20, 40, 40, BLACK);
}

void PianoApp::handleTouch(int x, int y) {
    // Add touch handling logic here if needed (or leave empty)
}