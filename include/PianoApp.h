#pragma once
#include "App.h"
#include "ui/Keyboard.h" 
#include <vector>
#include <string>

class AppManager;

class PianoApp : public App
{
public:
    PianoApp(AppManager *manager);
    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;

private:
    AppManager *appManager;
    ui::Keyboard _keyboard{0, 0, 960, 540}; 
    std::string _currentText;
    bool _needsRedraw = true;
};