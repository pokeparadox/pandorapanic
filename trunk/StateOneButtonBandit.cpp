#include "StateOneButtonBandit.h"
// Init our shapes object
OBBShapes::OBBShapes() {
    shapeCount = 0;
}

OBBShapes::~OBBShapes() {
    for (int i = 0; i < shapeCount; i++) {
        shapes[i]->clear();
        guides[i]->clear();
    }
}

// Add a shape
void OBBShapes::addShape(string s) {
    shapes[shapeCount] = new Sprite;
    shapes[shapeCount]->loadSprite("images/OneButtonBandit/" + s + ".png");
    shapes[shapeCount]->setTransparentColour(WHITE);
    shapes[shapeCount]->setUseHardware(true);
    guides[shapeCount] = new Sprite;
    guides[shapeCount]->loadSprite("images/OneButtonBandit/" + s + "_guide.png");
    guides[shapeCount]->setTransparentColour(WHITE);
    guides[shapeCount]->setUseHardware(true);
    shapeCount++;
}

// Return a reference to the requested sprite
Sprite &OBBShapes::getSprite(int n, bool guide) {
    if (guide) {
        return(*guides[n]);
    }
    else {
        return(*shapes[n]);
    }
}

OBBColumn::~OBBColumn() {
    /*if (click.isPlaying()) {
        click.stop();
    }
    click.freeAll();*/
    shapeNode = lastShapeNode = finalMatchNode = tempNode = NULL;
    while (shapeNode != NULL) {
        deleteLeadNode();
    }
}
//OBBColumn::OBBColumn() {}
void OBBColumn::initColumn(OBBShapes &s, OBBColumnConfig &c, float r, int x) {
    sprites = s;
    config = c;
    xPos = x;
    // alpha = 255;
    rateAdditive = r;
    active = false;

    // Just do stuff normally
    currentState = 0;

    // Set the click sound effect
    click.loadSound("sounds/OneButtonBandit/click.ogg");

    // And the button to push
    button.loadSprite("images/OneButtonBandit/button.png");
    button.setPosition(xPos + config.shapeX*0.5f - 15, 400);
    // button.setAlpha(200);

    // Populate our list with random shapes
    shapeNode = lastShapeNode = finalMatchNode = tempNode = NULL;
    for (int i = 0; i < 4; i++) {
        addShapeNode();

        // Change the shape of the second node to be the match shape
        if (i == 1) {
            lastShapeNode->shape = config.matchShape;
        }
    }
}

void OBBColumn::deleteLeadNode() {
    tempNode = shapeNode->next;
    delete shapeNode;
    shapeNode = tempNode;
}

void OBBColumn::addShapeNode() {
    // Build a new shape node
    tempNode = new OBBShapeNode;
    do {
        tempNode->shape = Random::nextInt(0, config.numberOfShapes);
    } while (lastShapeNode != NULL && tempNode->shape == lastShapeNode->shape);
    tempNode->next = NULL;

    // Determine the yPos for our new node
    if (lastShapeNode == NULL) {
        // We want to position the first one so that the second is bang in the middle of the screen
        tempNode->yPos = (config.screenY + config.shapeY)*0.5f + config.shapeGapY;
    }
    else {
        tempNode->yPos = lastShapeNode->yPos - config.shapeY - config.shapeGapY;
    }

    if (shapeNode == NULL) {
        shapeNode = tempNode;
    }
    else {
        lastShapeNode->next = tempNode;
    }

    // Set this as the last node
    lastShapeNode = tempNode;
}

void OBBColumn::stopOnMatch() {
    active = false;

    // Is the current matching node above the guide?
    if (!(finalMatchNode->yPos < config.guideYPos)) {
        // Add a new node with the matching shape
        if (lastShapeNode->shape != config.matchShape) {
            addShapeNode();
            lastShapeNode->shape = config.matchShape;
        }

        // Set the last node as our matching node
        finalMatchNode = lastShapeNode;
    }

    // Put the column into spin down mode
    currentState = 1;
}

void OBBColumn::instantStop() {
    active = false;
    currentState = 2;
    moveY = 0;
}

#ifdef PENJIN_SDL
void OBBColumn::render(SDL_Surface * screen) {
    // Render the guide
    sprites.getSprite(config.matchShape, true).setPosition(xPos, config.guideYPos);
    sprites.getSprite(config.matchShape, true).render(screen);

    // If the column is really moving, set the transparency as well
    // This may get to be a bit much for the Pandora to do in software
    // and might have to wait until I'm using a 3d surface.
    // Possibly telling SDL exactly what the sprites are instead of letting
    // it guess would make this quick enough.
    // if (moveY > 12) {
    //     alpha = 255 - moveY + 6;
    // }
    // else {
    //     alpha = 255;
    // }

    // Render ourselves some sprites
    tempNode = shapeNode;
    do {
        // Don't render anything that isn't on the screen
        if (tempNode->yPos + config.shapeY > 0) {
            sprites.getSprite(tempNode->shape, false).setPosition(xPos, tempNode->yPos);
            // sprites.getSprite(tempNode->shape, false).setAlpha(alpha);
            sprites.getSprite(tempNode->shape, false).render(screen);
        }
        tempNode = tempNode->next;
    } while(tempNode != NULL);

    if (active) {
        button.render(screen);
    }
}
#else
void OBBColumn::render()
{
    // Render the guide
    sprites.getSprite(config.matchShape, true).setPosition(xPos, config.guideYPos);
    sprites.getSprite(config.matchShape, true).render();

    // If the column is really moving, set the transparency as well
    // This may get to be a bit much for the Pandora to do in software
    // and might have to wait until I'm using a 3d surface.
    // Possibly telling SDL exactly what the sprites are instead of letting
    // it guess would make this quick enough.
    // if (moveY > 12) {
    //     alpha = 255 - moveY + 6;
    // }
    // else {
    //     alpha = 255;
    // }

    // Render ourselves some sprites
    tempNode = shapeNode;
    do {
        // Don't render anything that isn't on the screen
        if (tempNode->yPos + config.shapeY > 0) {
            sprites.getSprite(tempNode->shape, false).setPosition(xPos, tempNode->yPos);
            // sprites.getSprite(tempNode->shape, false).setAlpha(alpha);
            sprites.getSprite(tempNode->shape, false).render();
        }
        tempNode = tempNode->next;
    } while(tempNode != NULL);

    if (active) {
        button.render();
    }
}
#endif

void OBBColumn::update(float &rate) {

    switch(currentState) {
        // Regular update mode
        case 0:
            // Calculate how much we're moving by
            #ifdef PENJIN_FIXED
                moveY = config.moveY * (rate * rateAdditive).intValue<<16;
            #else
                moveY = config.moveY * (rate * rateAdditive);
            #endif
            break;

        // Matching the final shape
        case 1:
            if (nodeIsInValidPosition(finalMatchNode)) {
                if (finalMatchNode->yPos == config.guideYPos) {
                    moveY = 0;
                    currentState = 2;
                    click.play();
                }
                else if (finalMatchNode->yPos > config.guideYPos - 3) {
                    moveY = 1;
                }
                else if (finalMatchNode->yPos > config.guideYPos - config.shapeY/5) {
                    moveY = moveY/3;
                    if (moveY < 3) {
                        moveY = 3;
                    }
                }
            }
            break;

        // Do nothing
        case 2:
            break;

    }

    if (moveY != 0) {
        // Now move the first node in the list
        // If it's gone beyond the screen, trash it and add another at the end
        tempNode = shapeNode;
        tempNode->yPos += moveY;
        if (tempNode->yPos > config.screenY) {
            deleteLeadNode();
            addShapeNode(); // Now add a new node to the list
            tempNode = shapeNode;
            tempNode->yPos += moveY;
        }

        // Move all remaining nodes
        while (tempNode->next != NULL) {
            tempNode = tempNode->next;
            tempNode->yPos += moveY;
        }
    }
}

bool OBBColumn::nodeIsInValidPosition(OBBShapeNode *node) {
    // Is this shape in the right place?
    if (   node->yPos >= (config.guideYPos - config.shapeY*0.5f)
        && node->yPos <= (config.guideYPos + config.shapeY*0.5f)) {
        return(true);
    }
    return(false);
}

// Looks at the current shapes in the list and checks to see if we have any that
// are in the correct position to be marked as a hit.
bool OBBColumn::isValid() {
    tempNode = shapeNode;

    // It's never going to be the first shape in the list, but it
    // could be the second or the third so just check them all.
    while (tempNode->next != NULL) {
        tempNode = tempNode->next;

        // Is this node the same shape as the match shape?
        if (tempNode->shape == config.matchShape && nodeIsInValidPosition(tempNode)) {
            finalMatchNode = tempNode;
            return(true);
        }
    }

    return(false);
}

bool OBBColumn::isStopped() {
    return(moveY == 0);
}

//StateOneButtonBandit::StateOneButtonBandit() {}
void StateOneButtonBandit::init() {

    // set inputtimer needed for the pause functionality
    inputlimiter.setMode(MILLI_SECONDS);
    inputlimiter.start();

    // Define some useful variables
    config.screenX = GFX::getXResolution();
    config.screenY = GFX::getYResolution();
    config.shapeX = 192;
    config.shapeY = 192;
    config.shapeGapX = 20;
    config.shapeGapY = 10;
    config.moveY = 10;
    config.guideYPos = (config.screenY - config.shapeY)*0.5f;

    // How many objects are we showing?
    // This will be dependent on the level of the current game
    // so we can increase the number of sprites to increase
    // the difficulty.
    // 1 = 2 shapes (0,1)

    // How fast are the sprites going to move?
    // These should change based on the difficulty level as well as the
    // number of sprites used.
    currentGlobalRate = 0.0;

    // Get the difficulty setting
    float scale = 0.0f;
    switch(variables[2].getInt()) {
        case 0 ... 10:
            config.numberOfShapes = 3;
            startGlobalRate = 1.0f;
            maxGlobalRate = 1.6f;
            break;

        case 11 ... 25:
            scale = (variables[2].getInt() - 11)/50.0f;
            config.numberOfShapes = 4;
            startGlobalRate = 1.3f + scale;
            maxGlobalRate = 2.0f + scale;
            break;

        default:
            scale = (variables[2].getInt() - 25)/100.0f;
            config.numberOfShapes = 5;
            startGlobalRate = 1.6f + scale;
            maxGlobalRate = 2.5f + scale;
            break;

    }

    // Load up each of the images to be displayed
    sprites.addShape("grapes");
    sprites.addShape("horseshoe");
    sprites.addShape("lemon");
    sprites.addShape("heart");
    if (config.numberOfShapes > 3) {
        sprites.addShape("diamond");
    }
    if (config.numberOfShapes > 4) {
        sprites.addShape("seven");
        config.matchShape = config.numberOfShapes;
    }
    else {
        config.matchShape = Random::nextInt(0, config.numberOfShapes);
    }

    // Get our timer running right
    gameTimer.setMode(DECI_SECONDS);
    gameTimer.start();

    // Set the startup state
    win = waitForButtonLift = false;
    currentState = 0;
    currentColumn = 0;

    // Create some column objects
    int centre = (config.screenX - config.shapeX)*0.5f;
    leftColumn.initColumn(  sprites, config, 1.0f,  centre - config.shapeGapX - config.shapeX);
    centreColumn.initColumn(sprites, config, 1.15f, centre);
    rightColumn.initColumn( sprites, config, 1.3f,  centre + config.shapeX + config.shapeGapX);

    // Load the background graphic
    background.loadBackground("images/OneButtonBandit/background.png");
    background.setUseHardware(true);
    shading.loadImageNoKey("images/OneButtonBandit/shading.png");

    // Load the sound effects
    if(variables.size() < SUBSTATE_TRIGGER) {
        coin.loadSound("sounds/OneButtonBandit/coin.ogg");
        click.loadSound("sounds/OneButtonBandit/click.ogg");
        chime.loadSound("sounds/OneButtonBandit/chime.ogg");
        error.loadSound("sounds/OneButtonBandit/error.ogg");
        tumbler.loadSound("sounds/OneButtonBandit/tumbler.ogg");
        tumbler.setLoops(100);

        // Background music
        music.loadMusic("music/OneButtonBandit/OneArm.ogg");
        music.setLooping(false);
        music.play();

        // Start with the coin effect
        coin.play();
    }

    // System debug text
    text.loadFont("font/foo.ttf", 28);
    text.setColour(Colour(BLUE));
}

void StateOneButtonBandit::userInput() {
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit()) {
            nullifyState();
        }
    #endif
    // The pausecheck
    if (input->isStart())
    {
            pauseToggle();
            input->resetKeys();
    }

    // Only check for user input when the game has become playable
    if (currentState == 3) {
        if (waitForButtonLift) {
            if (!input->isA()) {
                waitForButtonLift = false;
            }
        }
        else if (input->isA()) {
            waitForButtonLift = true;
            // Set the left column to stop mode
            // Advance to the next column
            if (currentColumn == 0 && leftColumn.isValid()) {
                leftColumn.stopOnMatch();
                currentColumn = 1;
                centreColumn.active = true;
            }
            else if (currentColumn == 1 && centreColumn.isValid()) {
                centreColumn.stopOnMatch();
                currentColumn = 2;
                rightColumn.active = true;
            }
            else if (currentColumn == 2 && rightColumn.isValid()) {
                win = true;
                rightColumn.stopOnMatch();
                currentState = 4;
            }
            else {
                // Play the buzz

                // Go straight to endgame mode
                switch (currentColumn) {
                    case 0: leftColumn.instantStop(); break;
                    case 1: centreColumn.instantStop(); break;
                    case 2: rightColumn.instantStop(); break;
                }
                win = false;
                currentState = 4;
            }
        }
    }
}

#ifdef PENJIN_SDL
void StateOneButtonBandit::render(SDL_Surface *screen)
{
    background.render(screen);
    leftColumn.render(screen);
    centreColumn.render(screen);
    rightColumn.render(screen);
    shading.renderImage(screen, 0, 0);
}

void StateOneButtonBandit::pauseScreen(SDL_Surface* screen)
{
    if(variables.size()<SUBSTATE_TRIGGER)
        pauseSymbol(screen);
    text.setColour(YELLOW);
    text.setPosition(50,200);
    text.print(screen, "Stop the rollers on the outlined symbols!");
    text.setColour(BLACK);
    text.setPosition(52,202);
    text.print(screen, "Stop the rollers on the outlined symbols!");
//    text.setColour(BLACK);
//    text.setPosition(50,240);
//    text.print(screen, "(outlines give an indication too)");
//    text.setColour(WHITE);
//    text.setPosition(52,242);
//    text.print(screen, "(outlines give an indication too)");
}
#else
    void StateOneButtonBandit::render()
    {
        background.render();
        leftColumn.render();
        centreColumn.render();
        rightColumn.render();
        shading.renderImage(0, 0);
    }

    void StateOneButtonBandit::pauseScreen()
    {
        pauseSymbol();
        text.setColour(YELLOW);
        text.setPosition(50,200);
        text.print("Stop the rollers on the symbols you started with");
        text.setColour(BLACK);
        text.setPosition(52,202);
        text.print("Stop the rollers on the symbols you started with");
        text.setColour(BLACK);
        text.setPosition(50,240);
        text.print("(outlines give an indication too)");
        text.setColour(WHITE);
        text.setPosition(52,242);
        text.print("(outlines give an indication too)");
    }
#endif

void StateOneButtonBandit::onPause()
{
    gameTimer.pause();
}

void StateOneButtonBandit::onResume()
{
    gameTimer.unpause();
}

void StateOneButtonBandit::unlimitedUpdate() {
        switch (currentState) {
            // Standing still
            case 0:
                if (gameTimer.getScaledTicks() >= 10) {
                    currentState = 1;
                    gameTimer.start();

                    // Next step is to go backwards
                    currentGlobalRate = -0.2f;
                }
                break;

            // Small climb backwards
            case 1:
                if (gameTimer.getScaledTicks() >= 3) {
                    click.play();
                    currentState = 2;
                    gameTimer.start();

                    // Next step is start spinning
                    currentGlobalRate = 0.0f;
                }
                break;

            // Beginning to spin
            case 2:
                if (gameTimer.getScaledTicks() >= 15) {
                    tumbler.play();
                    currentState = 3;
                    gameTimer.stop();
                    leftColumn.active = true;

                    // Next step is spin at full speed
                    currentGlobalRate = startGlobalRate;
                }
                break;

            // Playable
            case 3:
                break;

            // Start the end timer
            case 4:
                if (rightColumn.isStopped()) {
                    tumbler.stop();
                }
                if ((win && rightColumn.isStopped()) || !win) {
                    gameTimer.start();
                    currentState = 5;
                    if (win) {
                        // Play victory music
                        chime.play();
                    }
                    else {
                        // Play defeat music
                        error.play();
                    }
                }
                break;

            // End the game
            case 5:
                if (gameTimer.getScaledTicks() >= 15) {
                    variables[0].setInt(win);
                    setNextState(STATE_MAIN);
                }

        }
}

void StateOneButtonBandit::update() {
        switch (currentState) {
            // Standing still
            case 0:
                break;

            // Small climb backwards
            case 1:
                break;

            // Beginning to spin
            // Have to have this phase to get the currently lined up
            // items off the guides
            case 2:
                currentGlobalRate = (gameTimer.getScaledTicks() * 1.0f)/15 * startGlobalRate;
                break;

            // Playable
            case 3:
                currentGlobalRate *= 1.005f;
                if (currentGlobalRate > maxGlobalRate) {
                    currentGlobalRate = maxGlobalRate;
                }
                break;

        }

        // Move the columns
        leftColumn.update(currentGlobalRate);
        centreColumn.update(currentGlobalRate);
        rightColumn.update(currentGlobalRate);
}

void StateOneButtonBandit::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
}
