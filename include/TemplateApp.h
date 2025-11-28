#pragma once
#include "App.h"
#include <vector>
#include <string>

class AppManager;

class TemplateApp : public App {
public:
    TemplateApp(AppManager* manager);

    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;
    bool handleBackPress() override;     // Optional: For custom back navigation
    bool handleForwardPress() override;  // Optional: For custom forward navigation

private:
    AppManager* appManager;
    // Add your app-specific members here (e.g., int counter; bool _needsRedraw = true;)
};