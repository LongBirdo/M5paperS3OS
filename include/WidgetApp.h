#pragma once

#include "App.h"
#include <M5Unified.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

class AppManager;

class WidgetApp : public App {
public:
    WidgetApp(AppManager* manager);

    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;
    bool handleBackPress() override;
    bool handleForwardPress() override;

private:
    void fetchHAData(); // Your logic to talk to Home Assistant
    
    AppManager* appManager;
    bool _needsRedraw = true;
    
    // Timer logic for periodic updates
    unsigned long _lastFetchTime = 0;
    const unsigned long FETCH_INTERVAL = 60000; // Refresh every 60 seconds

    // Variables to store HA data
    String _livingRoomTemp = "--";
    String _lightStatus = "Unknown";
    bool _isFetching = false;
};