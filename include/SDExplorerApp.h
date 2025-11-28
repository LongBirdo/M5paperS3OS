#pragma once

#include "App.h"
#include "AppManager.h"
#include <M5Unified.h>
#include <stack>
#include <string>
#include <vector>
#include "FS.h"
#include <SD.h>

struct FileInfo {
    std::string name;
    bool isDirectory;
    bool isImage;
};

class SDExplorerApp : public App {
public:
    SDExplorerApp(AppManager* manager);

    void setup() override;
    void update() override;
    void draw() override;
    void handleTouch(int x, int y) override;
    bool handleBackPress() override;
    bool handleForwardPress() override;

private:
    void openDirectory(const std::string& path);
    void showImage(const std::string& imagePath);
    void drawFileBrowser();
    void drawImageViewer();

    AppManager* appManager;
    std::string currentPath;
    File currentDirectory;
    std::vector<FileInfo> fileList;

    std::stack<std::string> folderHistory;
    std::stack<std::string> folderHistoryForward;

    // Image viewer state
    std::string _currentImagePath;
    bool _needsFullRedraw = true;
};