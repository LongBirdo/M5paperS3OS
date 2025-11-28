#include "AppManager.h"
#include <WiFi.h>
#include "BloomScreenSaver.h"

static int lastBatteryLevel = -1;
static std::string lastWiFiStatus = "";
static bool touchHandled = false;

//batt protection
unsigned long lastBatteryCheck = 0;
const int BATTERY_CHECK_INTERVAL = 10000;  
const int LOW_BATTERY_THRESHOLD = 10; 
const int WARNING_BATTERY_THRESHOLD = 20; 
unsigned long lastActivityTime = millis();
const unsigned long IDLE_TIMEOUT = 300000;  // 300000 =5 minutes

void AppManager::setup() {
    M5.Display.clear();
}

void AppManager::update() {
    M5.update();
    if (currentApp) {
        currentApp->update();
    }

    if (M5.Touch.getCount() > 0) {
        lastActivityTime = millis();  // Reset on touch
        if (!touchActive) {
            touchActive = true;
            touchHandled = false;
            auto touch = M5.Touch.getDetail();
            handleNavigationTouch(touch.x, touch.y);
            if (!touchHandled && currentApp) {
                currentApp->handleTouch(touch.x, touch.y);
            }
            needsRedraw = true;
        }
    } else {
        touchActive = false;
    }

    if (needsRedraw) {
        M5.Speaker.setVolume(255);
        M5.Speaker.tone(800, 50);
        if (currentApp) {
            currentApp->draw();
        }
        drawFooter();
        M5.Display.display();
        needsRedraw = false;
    }
    //bat protection

    if (millis() - lastActivityTime > IDLE_TIMEOUT) {
        //slow down
        if (M5.Display.isEPD()) {
            M5.Display.setEpdMode(epd_mode_t::epd_quality);
        }
   
        showScreensaver();
        M5.Display.display();
        delay(1000);

        if (M5.Display.isEPD()) {
            M5.Display.setEpdMode(epd_mode_t::epd_fast);
        }

    // Enter light sleep, wake on touch not working
    //M5.Power.lightSleep(0, true);
    M5.Power.deepSleep();  //Deep sleep cuz light sleep still causes death after a few days
    }

    if (millis() - lastBatteryCheck > BATTERY_CHECK_INTERVAL) {
        lastBatteryCheck = millis();
        int batteryLevel = M5.Power.getBatteryLevel();
        float batteryVoltage = M5.Power.getBatteryVoltage() / 1000.0;  // In volts
        
        if (batteryLevel < LOW_BATTERY_THRESHOLD || batteryVoltage < 3.0) {
            M5.Display.display();
            delay(2000);
            M5.Power.deepSleep();  
        } else if (batteryLevel < WARNING_BATTERY_THRESHOLD) {
        }
    }
}

void AppManager::registerApp(const std::string& name, App* app) {
    appRegistry.push_back({name, app});
}

AppRecord* AppManager::findApp(const std::string& name) {
    for (auto& record : appRegistry) {
        if (record.name == name) {
            return &record;
        }
    }
    return nullptr;
}

void AppManager::switchApp(const std::string& name, bool isNav) {

    Serial.print("Switching app to: ");
    Serial.print(name.c_str());
    Serial.print("\n");

    AppRecord* record = findApp(name);
    if (!record) return;

    if (currentApp) {
        currentApp->exit();
        if (!isNav) {
            navHistoryBack.push_back({currentAppName, ""});
            navHistoryForward.clear();
        }
    }
    
    currentApp = record->appInstance;
    currentAppName = record->name;
    currentApp->setup();
    needsRedraw = true;
}

void AppManager::drawFooter() {
    int currentBatteryLevel = M5.Power.getBatteryLevel();
    std::string currentWiFiStatus = (WiFi.status() == WL_CONNECTED) ? "Connected" : "Off";
    
    m5::rtc_time_t time;
    M5.Rtc.getTime(&time);
    m5::rtc_date_t date;
    M5.Rtc.getDate(&date);
    
    static int lastSecond = -1;
    bool secondChanged = (time.seconds != lastSecond);
    lastSecond = time.seconds;
    
    // Only redraw the footer if something has changed
    if (currentBatteryLevel != lastBatteryLevel || currentWiFiStatus != lastWiFiStatus || secondChanged || needsRedraw) {
        M5.Display.fillRect(0, 910, 540, 50, WHITE);
        M5.Display.setFont(&fonts::FreeSansBold12pt7b);
        M5.Display.setTextColor(BLACK);
        M5.Display.setTextSize(1);
        
        // Date and time on the right
        char dateBuffer[16];
        sprintf(dateBuffer, "%04d/%02d/%02d", date.year, date.month, date.date);
        std::string dateString = dateBuffer;
        
        char timeBuffer[16];
        sprintf(timeBuffer, "%02d:%02d", time.hours, time.minutes);
        std::string timeString = timeBuffer;

        //M5.Display.drawString(dateString.c_str(), 250, 935); 
        //M5.Display.drawString(timeString.c_str(), 400, 935); 

        std::string status = "Bat: " + std::to_string(currentBatteryLevel) + "% | " + currentWiFiStatus +
        " | " + dateString + " | " + timeString;
        M5.Display.drawString(status.c_str(), 60, 935);
        
        
        M5.Display.drawString("<", 20, 933);
        M5.Display.drawString(">", 500, 933); 
        
        lastBatteryLevel = currentBatteryLevel;
        lastWiFiStatus = currentWiFiStatus;
    }
}

void AppManager::handleNavigationTouch(int x, int y) {
    // Back button (<) press
    if (x >= 0 && x <= 50 && y >= 910) {
        bool handledByApp = false;
        if (currentApp) {
            handledByApp = currentApp->handleBackPress();
        }

        if (!handledByApp && !navHistoryBack.empty()) {
            NavigationState previousState = navHistoryBack.back();
            navHistoryBack.pop_back();

            navHistoryForward.push_back({currentAppName, ""});
            switchApp(previousState.appName, true);
        }
        touchHandled = true;
        needsRedraw = true;
    }

    // Forward button (>) press
    if (x >= 490 && x <= 540 && y >= 910) {
        bool handledByApp = false;
        if (currentApp) {
            handledByApp = currentApp->handleForwardPress();
        }

        if (!handledByApp && !navHistoryForward.empty()) {
            NavigationState nextState = navHistoryForward.back();
            navHistoryForward.pop_back();
            
            navHistoryBack.push_back({currentAppName, ""});
            switchApp(nextState.appName, true);
        }
        touchHandled = true;
        needsRedraw = true;
    }
}