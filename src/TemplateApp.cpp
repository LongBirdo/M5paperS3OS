#include "TemplateApp.h"
#include "AppManager.h"
// Include other headers as needed (e.g., <SD.h> for file ops, <ArduinoJson.h> for JSON)

TemplateApp::TemplateApp(AppManager* manager) : appManager(manager) {
    // Initialize app-specific members here if needed
}

void TemplateApp::setup() {
    // Run once when app is switched to (e.g., load data, reset state)
    // Example: counter = 0;
    // If using redraw flags: _needsRedraw = true;
}

void TemplateApp::update() {
    // Called every loop; handle ongoing logic (e.g., timers, sensor reads)
    // Keep lightweight to avoid blocking
    // Example: if (someCondition) { needsRedraw = true; }
}

void TemplateApp::draw() {
    // Render the UI; only called when needsRedraw is true in AppManager
    // Use M5.Display methods (clear, drawString, etc.)
    // Example:
    M5.Display.clear();
    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.setTextColor(BLACK);
    M5.Display.drawCentreString("Template App", 270, 100);
    // If using redraw flags: _needsRedraw = false;
}

void TemplateApp::handleTouch(int x, int y) {
    // Handle touch events (e.g., button presses)
    // Example: if (x > 100 && y > 100) { counter++; needsRedraw = true; }
}

bool TemplateApp::handleBackPress() {
    // Optional: Custom back logic (e.g., navigate sub-screens)
    // Return true if handled (prevents AppManager from popping history)
    return false;  // Let AppManager handle it by default
}

bool TemplateApp::handleForwardPress() {
    // Optional: Custom forward logic
    return false;
}