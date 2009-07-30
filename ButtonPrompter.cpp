#include "ButtonPrompter.h"
/*
    ButtonPrompter utility for Pandora Panic
    Release: 6 Dec 2008
    Author: Todd Foster
    License: GPLv3
    Images by TheBrainSquid based on model by Miggles
    discussion at: http://www.gp32x.com/board/index.php?showtopic=44652&st=255&gopid=676340&#entry676340

    Ideas for future enhancements:
        - setter for alternate button sheets
        - flashing buttons
        - create individual default positions for each image
*/


ButtonPrompter::ButtonPrompter() {
    setImages("images/ButtonPrompter/ButtonsSheet.png", 10);
    initDefaults();
}

ButtonPrompter::ButtonPrompter(string filename, int cols) {
    setImages(filename, cols);
    initDefaults();
    };


void ButtonPrompter::initDefaults() {
    timer.setMode(MILLI_SECONDS);
    timer.start();

    // Default settings should be reasonable for generic use in all games
    defaultUseFlash = true;
    defaultFlashDelay = 250;
    defaultFlashQuantity = 3;
    defaultScreen = NULL;
    defaultX = 400;
    defaultY = 400;
    defaultButton = BUTTON_A;
    }

ButtonPrompter& ButtonPrompter::setImages(string filename, int cols) {
    clear();
    columns = cols;
    loadImageSheet(filename, cols, 2);
    return *this;
    }
ButtonPrompter& ButtonPrompter::setFlash(bool on) { defaultUseFlash=on; return *this; }
ButtonPrompter& ButtonPrompter::setFlashDelay(int delay) { defaultFlashDelay = delay; return *this; }
ButtonPrompter& ButtonPrompter::setFlashQuantity(int quantity) { defaultFlashQuantity = quantity; return *this; }
ButtonPrompter& ButtonPrompter::setScreen(SDL_Surface *screen) { defaultScreen = screen; return *this; }
ButtonPrompter& ButtonPrompter::setDefaultX(int x) { defaultX = x; return *this; }
ButtonPrompter& ButtonPrompter::setDefaultY(int y) { defaultY = y; return *this; }
ButtonPrompter& ButtonPrompter::setDefaultButton(int button) { defaultButton = button; return *this; }

void ButtonPrompter::display(int button, int x, int y) { display(button, x, y, defaultUseFlash, defaultFlashDelay, defaultFlashQuantity); }
void ButtonPrompter::display(int x, int y) { display(defaultButton, x, y); }
void ButtonPrompter::display(int button) { display(button, defaultX, defaultY); }
void ButtonPrompter::display() { display(defaultButton); }

void ButtonPrompter::display(int button, int x, int y, bool useFlash, int flashDelay, int flashQuantity) {
    //TODO: calculate center and adjust x/y? Currently just using upper
    //      left corner, I assume.

    // Put two events on queue for each flash requested
    int t = timer.getTicks();
    for (int i=0; i<flashQuantity; i++) {
        // Lighter prompt
        int tt = t + flashDelay;
        queue.push_back(ButtonEvent(button, x, y, t, tt));
        t = tt;

        // Darker prompt
        tt += flashDelay;
        queue.push_back(ButtonEvent(button + (useFlash? columns : 0), x, y, t, tt));
        t = tt;
    }
}

void ButtonPrompter::render() {
    // Clean out expired events
    int currentTime = timer.getTicks();
    size_t i=0;
    while(i<queue.size()) {
        if(queue[i].endTime() < currentTime)
            queue.erase(queue.begin() + i);
        i++;
    }

    // Display remaining events
    for(std::vector<ButtonEvent>::iterator queueIt=queue.begin(); queueIt!=queue.end(); queueIt++) {
        ButtonEvent e = *(queueIt);
        if (e.startTime() < currentTime)
            renderImage(e.button(), e.x(), e.y());
    }
}

