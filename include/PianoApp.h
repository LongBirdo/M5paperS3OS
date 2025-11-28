#pragma once
#include "App.h"
#include <vector>
#include <string>

class AppManager; 


class PianoApp : public App {
public:
    PianoApp(AppManager* manager);
    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;
private:
    AppManager* appManager;
};