#include "WidgetApp.h"
#include "AppManager.h"

// Configuration - Use your own details here
const char* ha_url = "http://YOUR_HA_IP:8123/api/states/sensor.living_room_temperature";
const char* ha_token = "API HERE";

WidgetApp::WidgetApp(AppManager* manager) : appManager(manager) {}

void WidgetApp::setup() {
    _needsRedraw = true;
    _lastFetchTime = 0; // Force immediate fetch on start
}

void WidgetApp::update() {
    // Check if it's time to refresh data from Home Assistant
    if (millis() - _lastFetchTime >= FETCH_INTERVAL || _lastFetchTime == 0) {
        _lastFetchTime = millis();
        fetchHAData();
    }
}

void WidgetApp::fetchHAData() {
    if (WiFi.status() != WL_CONNECTED) return;

    _isFetching = true;
    HTTPClient http;
    
    // 1. Set the URL
    http.begin(ha_url);
    
    // 2. Add the Authorization Header (Crucial for HA)
    http.addHeader("Authorization", ha_token);
    http.addHeader("Content-Type", "application/json");

    // 3. Send GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        String payload = http.getString();
        
        // 4. Parse JSON
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            // Extract the 'state' value from HA response
            _livingRoomTemp = doc["state"].as<String>();
            _needsRedraw = true;
        }
    } else {
        Serial.printf("Error on HTTP request: %d\n", httpResponseCode);
    }

    http.end();
    _isFetching = false;
}

void WidgetApp::draw() {
    if (!_needsRedraw) return;

    M5.Display.clear();
    
    // Header
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.setTextColor(BLACK);
    M5.Display.drawCentreString("Home Assistant", 270, 40);
    M5.Display.drawLine(0, 80, 540, 80, BLACK);

    // Display Temperature Widget
    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.drawString("Living Room:", 30, 150);
    
    if (_isFetching) {
        M5.Display.drawString("...", 350, 150);
    } else {
        M5.Display.drawString(_livingRoomTemp + " C", 350, 150);
    }

    // Refresh Info
    M5.Display.setFont(&fonts::FreeSans9pt7b);
    M5.Display.setTextColor(DARKGREY);
    M5.Display.drawCentreString("Auto-refreshing every 60s", 270, 900);

    _needsRedraw = false;
}

void WidgetApp::handleTouch(int x, int y) {
    // Example: Manual refresh on tap anywhere
    if (y < 800) {
        _lastFetchTime = 0; // This triggers a refresh in the next update() loop
    }
}

bool WidgetApp::handleBackPress() {
    return false; // Let AppManager return to menu
}

bool WidgetApp::handleForwardPress() {
    return false;
}