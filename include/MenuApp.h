#pragma once
#include "App.h"
#include <vector>
#include <string>

class AppManager; 

struct MenuButton {
    int x, y, w, h;
    std::string label;
    std::string targetAppName;
};

class MenuApp : public App {
public:
    MenuApp(AppManager* manager);
    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;
private:
    AppManager* appManager;
    std::vector<MenuButton> buttons;
    void drawButton(const MenuButton& button);
};