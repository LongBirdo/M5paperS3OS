#pragma once
#include "App.h"
#include <string>
#include <vector>
#include "ui/LabelButton.h"
#include "ui/Incrementer.h"
#include "ui/ListSelector.h"
#include "ui/Keyboard.h" 

class AppManager;

enum class SettingsState {
    MAIN_SETTINGS,
    DATE_TIME,
    NETWORK
};

class SettingsApp : public App {
public:
    SettingsApp(AppManager* manager);
    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;
    bool needsRedraw() override;
private:
    AppManager* appManager;
    SettingsState currentState = SettingsState::MAIN_SETTINGS;
    std::vector<ui::LabelButton> mainSettingsButtons;
    m5::rtc_time_t _time;
    m5::rtc_date_t _date;
    
    bool _needsRedraw = true; 
    
    ui::Incrementer yearInc, monthInc, dateInc, hourInc, minuteInc, secondInc;
    ui::LabelButton saveButton;

    bool _isScanning = false;
    String _pendingSSID = "";
    std::vector<String> scannedNetworksSSID;
    std::vector<int32_t> scannedNetworksRSSI;
    ui::ListSelector networkSelector;
    ui::Keyboard _keyboard{0, 0, 960, 540}; 

    void saveNetwork(String ssid, String password);

    void drawMainSettings();
    void drawDateTimeScreen();
    void drawNetworkScreen();

    void handleMainSettingsTouch(int x, int y);
    void handleDateTimeTouch(int x, int y);
    void handleNetworkTouch(int x, int y);
};