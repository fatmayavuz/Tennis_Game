#include "icb_gui.h"
#include <stdlib.h>
#include <time.h>
#include <windows.h>

int FRM1;
HANDLE hTimerQueue = NULL;
ICBYTES m;

int player1PositionY = 200; // Player 1's starting vertical position
int player2PositionY = 200; // Player 2's starting vertical position
int ballPositionX = 200;
int ballPositionY = 150;
int ballSpeedX = 4;
int ballSpeedY = 3;
int playerSpeed = 3; // Speed at which players move vertically

void ICGUI_Create()
{
    ICG_MWTitle("Tennis Game");
    ICG_MWSize(450, 500);
}

// Function to move players vertically towards the ball
void MovePlayers()
{
    // Move player 1 towards the ball vertically
    if (ballPositionY < player1PositionY + 25)
    {
        player1PositionY -= playerSpeed;
    }
    else if (ballPositionY > player1PositionY + 25)
    {
        player1PositionY += playerSpeed;
    }

    // Move player 2 towards the ball vertically
    if (ballPositionY < player2PositionY + 25)
    {
        player2PositionY -= playerSpeed;
    }
    else if (ballPositionY > player2PositionY + 25)
    {
        player2PositionY += playerSpeed;
    }
}

// Thread called by the timer
VOID CALLBACK UpdateDisplay(PVOID lpParam, BOOLEAN TimerOrWaitFired)
{
    // Clear background
    FillRect(m, 0, 0, 400, 400, 0xFFFFFF); // White background

    // Move players
    MovePlayers();

    // Draw player 1
    FillRect(m, 50, player1PositionY, 20, 50, 0x00FF00); // Green rectangle for Player 1

    // Draw player 2
    FillRect(m, 350, player2PositionY, 20, 50, 0x0000FF); // Blue rectangle for Player 2

    // Draw the ball
    FillCircle(m, ballPositionX, ballPositionY, 10, 0xFF0000); // Red ball

    // Update the ball position
    ballPositionX += ballSpeedX;
    ballPositionY += ballSpeedY;

    // Ball reflection logic
    // If the ball hits player 1
    if (ballPositionX < 70 && ballPositionY > player1PositionY && ballPositionY < player1PositionY + 50)
    {
        ballSpeedX = -ballSpeedX; // Reverse the ball's horizontal direction
    }
    // If the ball hits player 2
    if (ballPositionX > 330 && ballPositionY > player2PositionY && ballPositionY < player2PositionY + 50)
    {
        ballSpeedX = -ballSpeedX; // Reverse the ball's horizontal direction
    }

    // Reflect ball if it hits the top or bottom of the court
    if (ballPositionY <= 0 || ballPositionY >= 400)
    {
        ballSpeedY = -ballSpeedY; // Reverse vertical direction
    }

    // Reflect ball off left or right walls when it doesn't hit players
    if (ballPositionX <= 0 || ballPositionX >= 400)
    {
        ballSpeedX = -ballSpeedX; // Reverse horizontal direction when hitting the wall
    }

    // Display the updated frame
    DisplayImage(FRM1, m);
    Sleep(30); // Animation delay
}

void StartGame()
{
    HANDLE hTimer = NULL;
    CreateImage(m, 400, 400, ICB_UINT);
    hTimerQueue = CreateTimerQueue();
    if (NULL == hTimerQueue)
    {
        return;
    }
    CreateTimerQueueTimer(&hTimer, hTimerQueue, (WAITORTIMERCALLBACK)UpdateDisplay, NULL, 1000, 30, 0);
    SetFocus(ICG_GetMainWindow());
}

void ICGUI_main()
{
    ICG_Button(5, 5, 120, 25, "Start Game", StartGame);
    FRM1 = ICG_FrameMedium(5, 40, 400, 400);
}
