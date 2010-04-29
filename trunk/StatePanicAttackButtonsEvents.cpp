#include "StatePanicAttackButtonsEvents.h"
/*
    StatePanicAttackButtonEvents
    Release: 20 December 2008
    Author: Todd Foster
    License: GPLv3
*/

StatePanicAttackButtonEvents::StatePanicAttackButtonEvents() {
    clear();
    loadImageSheet("images/PanicAttack/paButtons.png",  10, 3);
    location.resize(PA_NUM_BUTTONS);
    location[PA_BUTTON_UP] = Vector2di(240, 278);
    location[PA_BUTTON_LEFT] = Vector2di(210, 298);
    location[PA_BUTTON_RIGHT] = Vector2di(270, 298);
    location[PA_BUTTON_DOWN] = Vector2di(240, 318);
    location[PA_BUTTON_Y] = Vector2di(600, 285);
    location[PA_BUTTON_B] = Vector2di(570, 305);
    location[PA_BUTTON_A] = Vector2di(630, 305);
    location[PA_BUTTON_X] = Vector2di(600, 325);
    location[PA_BUTTON_SHOULDERLEFT] = Vector2di(230, 235);
    location[PA_BUTTON_SHOULDERRIGHT] = Vector2di(600, 240);


    bigRedX.clear();
    bigRedX.loadImageNoKey("images/PanicAttack/bigRedX.png");

    timer.setMode(MILLI_SECONDS);
    timer.start();
    responseTime = 1500;

    loserButton = PA_NUM_BUTTONS;
    preview = -1;
    }

void StatePanicAttackButtonEvents::expect(PA_BUTTON button) {
    queue.push_back(PAButtonEvent(
        button,
        location[button].x,
        location[button].y,
        timer.getTicks() + responseTime));
}

bool StatePanicAttackButtonEvents::rm(PA_BUTTON button) {
    bool foundIt = false;
    for(int i=0; i<(int)queue.size(); i++)
        if(queue[i].button() == button) {
            foundIt = true;
            queue.erase(queue.begin() + i);
            i--;
        }
    if (!foundIt)
        loserButton = button;
    return foundIt;
}

int StatePanicAttackButtonEvents::buttonCount(bool countShoulderButtons) {
    int count = queue.size();
    if (!countShoulderButtons)
        for (int i=0; i<count; i++)
            if(queue[i].button() == PA_BUTTON_SHOULDERLEFT)
                count--;
    return count;
}

#ifdef PENJIN_SDL
void StatePanicAttackButtonEvents::render(SDL_Surface *screenArgument) {
    screen = screenArgument;
    int currentTime = timer.getTicks();

    if (preview >= 0) {
        draw(PA_BUTTON_SHOULDERLEFT, PA_YELLOW_ICON);
        if(preview < PA_BUTTON_SHOULDERLEFT)
            for(int i=0; i<=preview; i++)
                draw((PA_BUTTON) i, PA_YELLOW_ICON);
    }

    // Check for missed buttons
    for (int i=0; i<(int)queue.size(); i++)
        if(queue[i].time() < currentTime) {
            //rm(queue[i].button()); break; // Uncomment this to watch the game play without a person
            loserButton = queue[i].button();
            timer.stop();
            break;
        }


    //Draw loser button or pending buttons
    if (lost()) {
            draw(loserButton, PA_RED_ICON);
            bigRedX.renderImage(screen, location[loserButton].x - 5, location[loserButton].y - 20);
            if (loserButton == PA_BUTTON_SHOULDERLEFT)
                bigRedX.renderImage(screen, location[PA_BUTTON_SHOULDERRIGHT].x - 5, location[PA_BUTTON_SHOULDERRIGHT].y - 20);
        }
    else
        for (int i=0; i<(int)queue.size(); i++)
            draw(queue[i].button(), whichButton(currentTime, queue[i]));
}
#else
void StatePanicAttackButtonEvents::render()
{
    int currentTime = timer.getTicks();

    if (preview >= 0) {
        draw(PA_BUTTON_SHOULDERLEFT, PA_YELLOW_ICON);
        if(preview < PA_BUTTON_SHOULDERLEFT)
            for(int i=0; i<=preview; ++i)
                draw((PA_BUTTON) i, PA_YELLOW_ICON);
    }

    // Check for missed buttons
    for (int i=0; i<(int)queue.size(); ++i)
        if(queue[i].time() < currentTime) {
            //rm(queue[i].button()); break; // Uncomment this to watch the game play without a person
            loserButton = queue[i].button();
            timer.stop();
            break;
        }


    //Draw loser button or pending buttons
    if (lost()) {
            draw(loserButton, PA_RED_ICON);
            bigRedX.renderImage(location[loserButton].x - 5, location[loserButton].y - 20);
            if (loserButton == PA_BUTTON_SHOULDERLEFT)
                bigRedX.renderImage(location[PA_BUTTON_SHOULDERRIGHT].x - 5, location[PA_BUTTON_SHOULDERRIGHT].y - 20);
        }
    else
        for (int i=0; i<(int)queue.size(); ++i)
            draw(queue[i].button(), whichButton(currentTime, queue[i]));
}
#endif

void StatePanicAttackButtonEvents::draw(PA_BUTTON button, PA_ICON_COLOR color) {
    // Left & Right buttons should always occur together.
    if(button == PA_BUTTON_SHOULDERLEFT)
      draw(PA_BUTTON_SHOULDERRIGHT, color);

    #ifdef PENJIN_SDL
    renderImage(
        button + PA_NUM_BUTTONS * color, screen,
        location[button % PA_NUM_BUTTONS].x,
        location[button % PA_NUM_BUTTONS].y);
    #else
    renderImage(
        (uint)(button + PA_NUM_BUTTONS * color),
        location[button % PA_NUM_BUTTONS].x,
        location[button % PA_NUM_BUTTONS].y);
    #endif
}

PA_ICON_COLOR StatePanicAttackButtonEvents::whichButton(int currentTime, PAButtonEvent e) {
    int remaining = e.time() - currentTime;

    // Check for green button
    int halfLife = responseTime * 0.5f;
    if (remaining > halfLife)
        return PA_GREEN_ICON;

    int sixthLife = halfLife / 6;
    for (int i=0; i<=halfLife; i+=sixthLife) // three full flashes
        if (remaining < i)
            return (i/sixthLife)%2==0? PA_YELLOW_ICON : PA_RED_ICON;
    return PA_RED_ICON; // last resort
}
