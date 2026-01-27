#include "SettingsApp.h"
#include "AppManager.h"
#include "ui/LabelButton.h"
#include "ui/Incrementer.h"
#include <ArduinoJson.h>
#include <SD.h>
#include <WiFi.h>
#include "ui/ListSelector.h"

// Initialize all UI elements in the constructor initializer list
SettingsApp::SettingsApp(AppManager* manager) : appManager(manager),
    yearInc(70, 150, 80, 40),
    monthInc(220, 150, 80, 40),
    dateInc(370, 150, 80, 40),
    hourInc(70, 400, 80, 40),
    minuteInc(220, 400, 80, 40),
    secondInc(370, 400, 80, 40),
    saveButton(190, 800, 160, 60, "Save"),
    networkSelector(70, 150, 400, 400, 60) // x, y, width, height, rowHeight
{}

void SettingsApp::setup(){
    Serial.println("Loading settings app");
    currentState = SettingsState::MAIN_SETTINGS;
    _needsRedraw = true;
    
    mainSettingsButtons.clear();
    mainSettingsButtons.push_back(ui::LabelButton(120, 200, 300, 80, "Date and Time"));
    mainSettingsButtons.push_back(ui::LabelButton(120, 320, 300, 80, "Network"));
}

void SettingsApp::update() {
    if(_isScanning) {
        int n = WiFi.scanComplete();
        if (n >= 0) {
            scannedNetworksSSID.clear();
            for (int i = 0; i < n; i++) {
                scannedNetworksSSID.push_back(WiFi.SSID(i));
            }
            // Pass the scanned names to our new selector
            networkSelector.setOptions(scannedNetworksSSID); 
            _isScanning = false;
            _needsRedraw = true;
        }
    }
}

void SettingsApp::draw() {
    if (!_needsRedraw) return;
    
    M5.Display.clear();
    
    switch (currentState) {
        case SettingsState::MAIN_SETTINGS:
            drawMainSettings();
            break;
        case SettingsState::DATE_TIME:
            drawDateTimeScreen();
            break;
        case SettingsState::NETWORK:
            drawNetworkScreen();
            break;
    }
    _needsRedraw = false;
}

void SettingsApp::handleTouch(int x, int y) {
    switch (currentState) {
        case SettingsState::MAIN_SETTINGS:
            handleMainSettingsTouch(x, y);
            break;
        case SettingsState::DATE_TIME:
            handleDateTimeTouch(x, y);
            break;
        case SettingsState::NETWORK:
            handleNetworkTouch(x, y);
            break;
    }
}

void SettingsApp::drawMainSettings() {
    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.drawCentreString("Settings", 270, 100);
    for (auto& button : mainSettingsButtons) {
        button.draw();
    }
}

void SettingsApp::handleMainSettingsTouch(int x, int y){
    for (auto& button : mainSettingsButtons) {
        if (button.isPressed(x, y)) {
            if (button.getLabel() == "Date and Time") {
                currentState = SettingsState::DATE_TIME;
                M5.Rtc.getTime(&_time);
                M5.Rtc.getDate(&_date);
                _needsRedraw = true;
            } else if (button.getLabel() == "Network") {
                WiFi.scanNetworks(true);
                _isScanning = true;
                currentState = SettingsState::NETWORK;
                _needsRedraw = true;
            }
        }
    }
}

void SettingsApp::drawDateTimeScreen(){
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.drawCentreString("Set Date & Time", 270, 60);

    M5.Display.setFont(&fonts::FreeSansBold9pt7b);
    M5.Display.setTextColor(BLACK);

    M5.Display.drawCentreString("Year", 110, 130);
    M5.Display.drawCentreString("Month", 260, 130);
    M5.Display.drawCentreString("Date", 410, 130);
    
    yearInc.draw(_date.year);
    monthInc.draw(_date.month);
    dateInc.draw(_date.date);
    hourInc.draw(_time.hours);
    minuteInc.draw(_time.minutes);
    secondInc.draw(_time.seconds);
    
    saveButton.draw();
}

void SettingsApp::handleDateTimeTouch(int x, int y){
    // ... (Incrementer logic remains same)
    if(yearInc.isIncrementPressed(x, y)) { _date.year++; _needsRedraw = true; }
    else if (yearInc.isDecrementPressed(x, y)) { _date.year--; _needsRedraw = true; }
    // ... (Rest of logic)
    else if(saveButton.isPressed(x, y)) {
        m5::rtc_datetime_t dt; dt.date = _date; dt.time = _time;
        M5.Rtc.setDateTime(dt);
        currentState = SettingsState::MAIN_SETTINGS;
        _needsRedraw = true;
    }
}

void SettingsApp::drawNetworkScreen(){
    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.drawCentreString("Choose Network", 270, 60);

    if (_isScanning) {
        M5.Display.setFont(&fonts::FreeSansBold18pt7b);
        M5.Display.drawCenterString("Searching...", 270, 300);
    } else {
        // Draw the reusable list selector
        networkSelector.draw(); 

        // Draw the Connect button at the bottom
        M5.Display.fillRect(170, 800, 200, 60, BLACK);
        M5.Display.setTextColor(WHITE);
        M5.Display.setFont(&fonts::FreeSansBold12pt7b);
        M5.Display.drawCentreString("CONNECT", 270, 818);
    }
}

void SettingsApp::handleNetworkTouch(int x, int y){
    if (_isScanning) return;

    // Check if user interacted with the list (arrows or rows)
    // We removed _needsRedraw = true because the networkSelector::draw() 
    // should only redraw its own clipping area/rect to prevent flickering.
    if (networkSelector.handleTouch(x, y)) {
        networkSelector.draw(); 
    }

    // Check if user pressed "Connect"
    if (x >= 170 && x <= 370 && y >= 800 && y <= 860) {
        String selectedSSID = networkSelector.getSelectedString();
        if (selectedSSID != "") {
            Serial.print("Connecting to: ");
            Serial.println(selectedSSID);
        }
    }
}