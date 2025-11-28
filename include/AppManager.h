#pragma once
#include <vector>
#include <string>
#include "App.h"

struct NavigationState {
    std::string appName;
    std::string statePayload; 
};

struct AppRecord {
    std::string name;
    App* appInstance;
};

class AppManager {
public:
    void setup();
    void update();
    void registerApp(const std::string& name, App* app);
    void switchApp(const std::string& name, bool isNav = false);

private:
    std::vector<AppRecord> appRegistry;
    App* currentApp = nullptr;
    std::string currentAppName;
    bool needsRedraw = true;
    bool touchActive = false;

    std::vector<NavigationState> navHistoryBack;
    std::vector<NavigationState> navHistoryForward;

    void drawFooter();
    void handleNavigationTouch(int x, int y);
    AppRecord* findApp(const std::string& name);
};