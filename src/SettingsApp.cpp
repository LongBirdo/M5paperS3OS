#include "SettingsApp.h"
#include "AppManager.h"
#include "ui/LabelButton.h"
#include "ui/Incrementer.h"

SettingsApp::SettingsApp(AppManager* manager) : appManager(manager),
    // Initialize the Incrementer objects here
    yearInc(70, 150, 80, 40),
    monthInc(220, 150, 80, 40),
    dateInc(370, 150, 80, 40),
    hourInc(70, 400, 80, 40),
    minuteInc(220, 400, 80, 40),
    secondInc(370, 400, 80, 40),
    saveButton(190, 800, 160, 60, "Save") {}

void SettingsApp::setup(){
    Serial.println("Loading settings app");
    
    // Reset the state to the main screen every time the app is loaded
    currentState = SettingsState::MAIN_SETTINGS;
    
    // Tell the app it needs to do a full redraw on first load
    _needsRedraw = true;
    
    // Create the main settings buttons only once in setup
    mainSettingsButtons.clear();
    mainSettingsButtons.push_back(ui::LabelButton(120, 200, 300, 80, "Date and Time"));
    mainSettingsButtons.push_back(ui::LabelButton(120, 320, 300, 80, "Network"));
}

void SettingsApp::update() {}

void SettingsApp::draw() {
    // Only redraw the screen if something has changed
    if (!_needsRedraw) {
        return;
    }
    
    M5.Display.clear();
    
    switch (currentState) {
        case SettingsState::MAIN_SETTINGS:
            drawMainSettings();
            break;
        case SettingsState::DATE_TIME:
            drawDateTimeScreen();
            break;
    }
    
    // Reset the flag after a full redraw is complete
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
            }
        }
    }
}

void SettingsApp::drawDateTimeScreen(){
    M5.Display.setFont(&fonts::FreeSansBold18pt7b);
    M5.Display.drawCentreString("Set Date & Time", 270, 60);

    M5.Display.setFont(&fonts::FreeSansBold9pt7b);
    M5.Display.setTextColor(BLACK);

    M5.Display.drawCentreString("Year", 70 + 40, 130);
    M5.Display.drawCentreString("Month", 220 + 40, 130);
    M5.Display.drawCentreString("Date", 370 + 40, 130);

    M5.Display.drawCentreString("Hour", 70 + 40, 380);
    M5.Display.drawCentreString("Minute", 220 + 40, 380);
    M5.Display.drawCentreString("Second", 370 + 40, 380);
    
    yearInc.draw(_date.year);
    monthInc.draw(_date.month);
    dateInc.draw(_date.date);
    hourInc.draw(_time.hours);
    minuteInc.draw(_time.minutes);
    secondInc.draw(_time.seconds);
    
    saveButton.draw();
}

void SettingsApp::handleDateTimeTouch(int x, int y){
    if(yearInc.isIncrementPressed(x, y)) {
        _date.year++;
        yearInc.drawValue(_date.year); 
    } else if (yearInc.isDecrementPressed(x, y)) {
        _date.year--;
        yearInc.drawValue(_date.year);
    }
    else if(monthInc.isIncrementPressed(x, y)) {
        _date.month++;
        monthInc.drawValue(_date.month);
    } else if (monthInc.isDecrementPressed(x, y)) {
        _date.month--;
        monthInc.drawValue(_date.month);
    }
    else if(dateInc.isIncrementPressed(x, y)) {
        _date.date++;
        dateInc.drawValue(_date.date);
    } else if (dateInc.isDecrementPressed(x, y)) {
        _date.date--;
        dateInc.drawValue(_date.date);
    }
    else if(hourInc.isIncrementPressed(x, y)) {
        _time.hours++;
        hourInc.drawValue(_time.hours);
    } else if (hourInc.isDecrementPressed(x, y)) {
        _time.hours--;
        hourInc.drawValue(_time.hours);
    }
    else if(minuteInc.isIncrementPressed(x, y)) {
        _time.minutes++;
        minuteInc.drawValue(_time.minutes);
    } else if (minuteInc.isDecrementPressed(x, y)) {
        _time.minutes--;
        minuteInc.drawValue(_time.minutes);
    }
    else if(secondInc.isIncrementPressed(x, y)) {
        _time.seconds++;
        secondInc.drawValue(_time.seconds);
    } else if (secondInc.isDecrementPressed(x, y)) {
        _time.seconds--;
        secondInc.drawValue(_time.seconds);
    }
    else if(saveButton.isPressed(x, y)) {
        m5::rtc_datetime_t newDateTime;
        newDateTime.date = _date;
        newDateTime.time = _time;
        M5.Rtc.setDateTime(newDateTime);
        
        currentState = SettingsState::MAIN_SETTINGS;
        _needsRedraw = true;
    }

    if (_date.month > 12) _date.month = 1;
    if (_date.month < 1) _date.month = 12;
    if (_date.date > 31) _date.date = 1;
    if (_date.date < 1) _date.date = 31;
    if (_time.hours > 23) _time.hours = 0;
    if (_time.hours < 0) _time.hours = 23;
    if (_time.minutes > 59) _time.minutes = 0;
    if (_time.minutes < 0) _time.minutes = 59;
    if (_time.seconds > 59) _time.seconds = 0;
    if (_time.seconds < 0) _time.seconds = 59;
}