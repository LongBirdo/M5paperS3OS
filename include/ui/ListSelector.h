#pragma once
#include <M5Unified.h>
#include <vector>

namespace ui {

class ListSelector {
public:
    ListSelector(int x, int y, int w, int h, int rowHeight = 60);
    
    // Core logic
    void setOptions(const std::vector<String>& options);
    void draw();
    void scrollUp();
    void scrollDown();
    
    // Interaction
    int getSelectedIndex() const { return _selectedIndex; }
    String getSelectedString() const;
    bool handleTouch(int x, int y); // Returns true if selection changed

private:
    int _x, _y, _w, _h;
    int _rowHeight;
    int _selectedIndex = 0;
    int _scrollOffset = 0; // For future-proofing scrolling long lists
    std::vector<String> _options;

    void drawArrow(int x, int y, bool up);
};

}