#include "StatePong.h"
StatePong::StatePong()
{
    // Background
    background.loadBackground("images/Pong/background.png");

    // Sprites
    ball.loadSprite(            "images/Pong/ball.png");
    ball.setTransparentColour(MAGENTA);
    playerPaddle.loadSprite(    "images/Pong/paddle.png");
    playerPaddle.setTransparentColour(MAGENTA);
    cpuPaddle.loadSprite(       "images/Pong/paddle.png");
    cpuPaddle.setTransparentColour(MAGENTA);

    if(variables.size()<SUBSTATE_TRIGGER)
    {
        // Sound effects
        bounceWallSound.loadSound(  "sounds/Pong/wall.ogg");
        bouncePaddleSound.loadSound("sounds/Pong/paddle.ogg");
        bounceOutSound.loadSound(   "sounds/Pong/out.ogg");
    }
}


StatePong::~StatePong()
{

}

void StatePong::init()
{
    // End game settings
    playerWin = 0;
    gameOver  = false;
    bounceOut = false;
    bounceWall = false;
    bouncePaddle = false;
    endGame.setMode(SECONDS);
    endGame.stop();

    // Set the various widths here rather that throughout the code
    ballWidth    = ballHeight = 16;
    paddleWidth               = 16;
    paddleHeight              = 60;
    screenHeight              = GFX::getYResolution();
    screenWidth               = GFX::getXResolution();
    collision                 = 0;
    collisionDeflection       = paddleHeight*0.5f + ballHeight*0.33f;

    // Give the ball a random location (on the right hand side of the screen)
    int ballPosX = Random::nextInt(screenWidth / 2 + ballWidth, screenWidth - 120 - ballWidth * 2);
    int ballPosY = Random::nextInt(ballHeight, screenHeight - ballHeight * 2);
    ball.setPosition(ballPosX, ballPosY);

    // Make sure the ball is travelling to the left
    ballRight    = -1;
    ballUp       =  Random::nextInt(4, 9) / (Random::nextBool() ? 10.0 : -10.0);

    // Set our difficulty settings
    switch(variables[2].getInt()) {
        case 0 ... 5:
            paddleSpeed     = 10;
            cpuPaddleSpeed  =  7.5f;
            ballVelocity    =  2.0f;
            maxBallVelocity =  6.0f;
            cpuPaddleZone   = paddleHeight / 2.5f;
            break;

        case 6 ... 15:
            paddleSpeed     = 10;
            cpuPaddleSpeed  = 9;
            ballVelocity    =  3.0f;
            maxBallVelocity =  8.0f;
            cpuPaddleZone   = paddleHeight / 5;
            break;

        default:
            paddleSpeed     = 10;
            cpuPaddleSpeed  = 10;
            ballVelocity    =  2.5f + variables[2].getInt() / 10;
            maxBallVelocity =  8.0f + variables[2].getInt() / 10;
            cpuPaddleZone   =  paddleHeight / variables[2].getInt();
            break;
    }

    // Put the player paddle in the middle
    playerPaddle.setPosition(120, (screenHeight - paddleHeight) * 0.5f);
    playerPaddleVelocity = 0;

    // Put the opponents paddle somewhere
    cpuPaddle.setPosition(screenWidth - 120 - paddleWidth, (screenHeight - paddleHeight) *0.5f);

    pauseText.loadFont("font/bip.ttf", 32);
    pauseText.setColour(WHITE);
}

void StatePong::userInput()
{

    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit()) {
            nullifyState();
        }
    #endif

    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }


    // User is moving their paddle?
    if (input->isUp()) {
        playerPaddleVelocity = paddleSpeed * -1;
    }
    else if (input->isDown()) {
        playerPaddleVelocity = paddleSpeed;
    }
    else {
        playerPaddleVelocity = 0;
    }
}

#ifdef PENJIN_SDL
    void StatePong::render(SDL_Surface *screen)
    {
        // Render the background
        background.render(screen);

        // Add the user paddle
        playerPaddle.render(screen);

        // Add the cpu paddle
        cpuPaddle.render(screen);

        // Add the ball
        ball.render(screen);
    }

    void StatePong::pauseScreen(SDL_Surface* screen)
    {
        // Pause screen
        if(!isSubState())
            pauseSymbol(screen);
        pauseText.setPosition(50,180);
        pauseText.print(screen, "Win a good, old game of Pong!");
        pauseText.setPosition(50,220);
        pauseText.print(screen, "Caution: It's sudden death!");
        buttonsOverlay(screen);
    }
#else
    void StatePong::render()
    {
        // Render the background
        background.render();

        // Add the user paddle
        playerPaddle.render();

        // Add the cpu paddle
        cpuPaddle.render();

        // Add the ball
        ball.render();
    }

    void StatePong::pauseScreen()
    {
        // Pause screen
        pauseSymbol();
        pauseText.setPosition(50,180);
        pauseText.print("Win a good, old game of Pong!");
        pauseText.setPosition(50,220);
        pauseText.print("Caution: It's sudden death!");
    }
#endif
void StatePong::unlimitedUpdate()
{
    // Are we in the end game?
    if (bounceOut && !gameOver) {
        bounceOut = false;
        bounceOutSound.play();
        gameOver = true;
        endGame.start();
    }
    else if (gameOver && endGame.getScaledTicks() > 0.3) {
        variables[0].setInt(playerWin);
        setNextState(STATE_MAIN);
    }

    // Do we have other sound effects to play?
    if (!gameOver) {
        if (bounceWall) {
            bounceWall = false;
            bounceWallSound.play();
        }
        if (bouncePaddle) {
            bouncePaddle = false;
            bouncePaddleSound.play();
        }
    }
}

void StatePong::update()
{
    // High speed balls
    ballVelocity = ballVelocity * 1.005;
    if (ballVelocity > maxBallVelocity)
        ballVelocity = maxBallVelocity;

    // Bounce the ball off the edges of the playfield
    // If it goes off the left, we lost
    // If it goes off the right, we won
    // We being me, the player, not me the Pandora
    if (ball.getX() <= 0) {
        bounceOut = true;
        playerWin = 0;
    }
    else if (ball.getX() >= screenWidth) {
        bounceOut = true;
        playerWin = 1;
    }

    // Bouncing off the ceiling? Here, have some downers.
    // Bouncing off the floor? Here, have some uppers.
    // What, the pills don't work anymore?
    if (   ((ball.getY() <= 0) && (ballUp < 0))
        || ((ball.getY() >= (screenHeight - ballHeight)) && (ballUp > 0))) {
        bounceWall = true;
        ballUp *= -1;
    }

    // Stop playing with your balls
    // TODO: Do this properly (with trig)
    ball.setY(ball.getY() + 2 * ballVelocity * ballUp);
    ball.setX(ball.getX() + 2 * ballVelocity * ballRight);

    // Move the user paddle
    playerPaddle.setY(playerPaddle.getY() + playerPaddleVelocity);
    if (playerPaddle.getY() < 0)
        playerPaddle.setY(0);
    if (playerPaddle.getY() > (screenHeight - paddleHeight))
        playerPaddle.setY(screenHeight - paddleHeight);

    // Is the player paddle hitting the ball?
    if (ball.getX() <= (playerPaddle.getX() + paddleWidth) && (ball.getX() + ballWidth) >= playerPaddle.getX()) {
        if ((ball.getY() + ballHeight) >= playerPaddle.getY() && ball.getY() <= (playerPaddle.getY() + paddleHeight)) {
            ball.setX(playerPaddle.getX() + paddleWidth);
            bouncePaddle = true;
            ballRight *= -1;

            // Determine the vertical distance between the centre of the paddle and the centre of the ball
            // Use this to modify the vertical factor
            collision = (ball.getY() + ballHeight*0.5f) - (playerPaddle.getY() + paddleHeight*0.5f);

            // Use this value to modify the ballUp value
            ballUp = collision / (collisionDeflection * 1.0f);

            ACHIEVEMENTS->logEvent("PONG_HIT");
        }
    }


    // Move the cpu paddle to appear as though it's going to hit the ball
    if (gameOver) {
        cpuPaddleVelocity = 0;
    }
    else if ((ball.getY() + ballHeight) < (cpuPaddle.getY() + cpuPaddleZone)) {
        cpuPaddleVelocity = cpuPaddleSpeed * -1;
    }
    else if (ball.getY() > (cpuPaddle.getY() + paddleHeight - cpuPaddleZone)) {
        cpuPaddleVelocity = cpuPaddleSpeed;
    }
    else {
        cpuPaddleVelocity = 0;
    }
    cpuPaddle.setY(cpuPaddle.getY() + cpuPaddleVelocity);

    // Is the cpu paddle hitting the ball?
    if ((ball.getX() + ballWidth) >= cpuPaddle.getX() && ball.getX() < (cpuPaddle.getX() + paddleWidth)) {
        if ((ball.getY() + ballHeight) >= cpuPaddle.getY() && ball.getY() <= (cpuPaddle.getY() + paddleHeight)) {
            ball.setX(cpuPaddle.getX() - ballWidth);
            bouncePaddle = true;
            ballRight *= -1;

            // Determine the vertical distance between the centre of the paddle and the centre of the ball
            // Use this to modify the vertical factor
            collision = (ball.getY() + ballHeight*0.5f) - (cpuPaddle.getY() + paddleHeight*0.5f);

            // Use this value to modify the ballUp value
            ballUp = collision / (collisionDeflection * 1.0);
        }
    }
}
