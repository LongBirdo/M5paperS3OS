#include "ui/ListSelector.h"

namespace ui {

ListSelector::ListSelector(int x, int y, int w, int h, int rowHeight)
    : _x(x), _y(y), _w(w), _h(h), _rowHeight(rowHeight) {}

void ListSelector::setOptions(const std::vector<String>& options) {
    _options = options;
    _selectedIndex = 0;
}

void ListSelector::draw() {
    // 1. Draw Container Box
    M5.Display.drawRect(_x, _y, _w, _h, BLACK);

    // 2. Draw Arrows on the right side
    int arrowAreaWidth = 60;
    int arrowX = _x + _w - arrowAreaWidth;
    
    // Up Arrow Box
    M5.Display.drawRect(arrowX, _y, arrowAreaWidth, _h / 2, BLACK);
    drawArrow(arrowX + 15, _y + (_h / 4) - 10, true);
    
    // Down Arrow Box
    M5.Display.drawRect(arrowX, _y + (_h / 2), arrowAreaWidth, _h / 2, BLACK);
    drawArrow(arrowX + 15, _y + (3 * _h / 4) - 10, false);

    // 3. Draw Options
    int visibleRows = _h / _rowHeight;
    int textX = _x + 10;
    
    for (int i = 0; i < visibleRows && i < _options.size(); i++) {
        int rowY = _y + (i * _rowHeight);
        bool isSelected = (i == _selectedIndex);

        if (isSelected) {
            // Invert colors for the selection
            M5.Display.fillRect(_x + 1, rowY + 1, _w - arrowAreaWidth - 2, _rowHeight - 2, BLACK);
            M5.Display.setTextColor(WHITE);
        } else {
            M5.Display.fillRect(_x + 1, rowY + 1, _w - arrowAreaWidth - 2, _rowHeight - 2, WHITE);
            M5.Display.setTextColor(BLACK);
        }

        M5.Display.setFont(&fonts::FreeSansBold12pt7b);
        // Truncate string if too long for box
        String disp = _options[i];
        if(disp.length() > 15) disp = disp.substring(0, 12) + "...";
        
        M5.Display.drawString(disp, textX, rowY + (_rowHeight / 2) - 10);
    }
}

void ListSelector::drawArrow(int x, int y, bool up) {
    if (up) {
        M5.Display.fillTriangle(x, y + 20, x + 15, y, x + 30, y + 20, BLACK);
    } else {
        M5.Display.fillTriangle(x, y, x + 15, y + 20, x + 30, y, BLACK);
    }
}

void ListSelector::scrollUp() {
    if (_selectedIndex > 0) {
        _selectedIndex--;
    } else {
        _selectedIndex = _options.size() - 1; // Wrap around
    }
}

void ListSelector::scrollDown() {
    if (_selectedIndex < _options.size() - 1) {
        _selectedIndex++;
    } else {
        _selectedIndex = 0; // Wrap around
    }
}

String ListSelector::getSelectedString() const {
    if (_options.empty()) return "";
    return _options[_selectedIndex];
}

bool ListSelector::handleTouch(int x, int y) {
    if (x < _x || x > _x + _w || y < _y || y > _y + _h) return false;

    int arrowX = _x + _w - 60;
    if (x >= arrowX) {
        if (y < _y + (_h / 2)) scrollUp();
        else scrollDown();
        return true;
    }
    
    // Check if user clicked a specific row directly
    int relativeY = y - _y;
    int rowClicked = relativeY / _rowHeight;
    if (rowClicked < _options.size()) {
        _selectedIndex = rowClicked;
        return true;
    }

    return false;
}

}