#include <SDL.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Screen dimensions
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Player dimensions
const int PLAYER_WIDTH = 20;
const int PLAYER_HEIGHT = 80;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;

bool quit = false;

bool initSDL()
{
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		// Create window
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (renderer == NULL)
			{
				printf("Renderer could not be create! Error: %s\n", SDL_GetError());
				return false;
			}			
		}
	}
	
	return true;
}

void closeSDL()
{
	// Free resources
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}

void drawPlayers(int y1, int y2)
{
	SDL_Rect player1 = { 20, y1, 20, 80 };
	SDL_Rect player2 = { SCREEN_WIDTH - PLAYER_WIDTH - 20, y2, 20, 80 };

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &player1);
	SDL_RenderFillRect(renderer, &player2);
}

void drawBall(int x, int y)
{
	SDL_Rect ball = { x, y, 10, 10 };

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &ball);
}

float dist(float x, float y)
{
	return sqrt(x*x + y*y);
}

float random01()
{
	return (float)rand() / (float)RAND_MAX;
}

int main(int argc, char* args[])
{	
	// Ball vars
	int ballX, ballY, speed;
	float moveX, moveY, mDist;

	// Players vars
	int moveP1, moveP2;

	// Keyboard
	const Uint8* keyState;
	
	// Init players positions
	moveP1 = moveP2 = (SCREEN_HEIGHT / 2) - PLAYER_HEIGHT;

	// Init ball position and speed
	ballX = SCREEN_WIDTH / 2;
	ballY = SCREEN_HEIGHT / 2;
	speed = 6;

	// Init random
	srand((unsigned)time(NULL));
	
	// Random new ball position
	moveX = random01();
	moveY = random01();

	// Normalized new ball position
	mDist = dist(moveX, moveY);
	moveX = (moveX / mDist) * speed;
	moveY = (moveY / mDist) * speed;

	if (initSDL())
	{
		// Main loop
		while (!quit)
		{
			// Event loop
			while (SDL_PollEvent(&e) != 0)
			{
				// User presses the close button
				if (e.type == SDL_QUIT)
					quit = true;
			}

			keyState = SDL_GetKeyboardState(NULL);
			if (keyState[SDL_SCANCODE_A])
			{
				if ((moveP1 + PLAYER_HEIGHT) < SCREEN_HEIGHT - 10)
				{
					moveP1 += 10;
					//printf("Player1: %d\n", moveP1 + PLAYER_HEIGHT);
				}
			}
			if (keyState[SDL_SCANCODE_S])
			{
				if (moveP1 > 10)
				{
					moveP1 -= 10;
					//printf("Player1: %d\n", moveP1);
				}
			}
			if (keyState[SDL_SCANCODE_J])
			{
				if ((moveP2 + PLAYER_HEIGHT) < SCREEN_HEIGHT - 10)
				{
					moveP2 += 10;
					//printf("Player2: %d\n", moveP2 + PLAYER_HEIGHT);
				}
			}
			if (keyState[SDL_SCANCODE_K])
			{
				if (moveP2 > 10)
				{
					moveP2 -= 10;
					//printf("Player2: %d\n", moveP2);
				}
			}

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);

			drawPlayers(moveP1, moveP2);

			ballX += moveX;
			ballY += moveY;
			printf("Ball: %d, %d\n", ballX, ballY);
			drawBall(ballX, ballY);

			// Update screen
			SDL_RenderPresent(renderer);
		}

		closeSDL();
	}

	return 0;
}