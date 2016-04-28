#define _CRT_SECURE_NO_WARNINGS

#include <SDL.h>
#include <SDL_ttf.h>
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
const int PLAYER_SPEED = 10;

// Vector
struct Vector {
	float x;
	float y;
};

// Texture
struct BTexture {
	SDL_Texture *texture;
	int w;
	int h;
};

// Ball
struct Ball {
	int x;
	int y;
	int w;
	int h;
	int speed;
};

// Player
struct Player {
	int x;
	int y;
	int speed;
	int score;
};

typedef struct BTexture BTexture;
typedef struct Player Player;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Renderer* renderer = NULL;
SDL_Event e;

TTF_Font* font = NULL;
BTexture scorePlayer1;
BTexture scorePlayer2;

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
			else
			{
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					return false;
				}
			}
		}
	}
	
	return true;
}

void closeSDL()
{
	// Free resources
	SDL_FreeSurface(screenSurface);
	screenSurface = NULL;

	SDL_DestroyTexture(scorePlayer1.texture);
	SDL_DestroyTexture(scorePlayer2.texture);

	TTF_CloseFont(font);
	font = NULL;

	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	TTF_Quit();
	SDL_Quit();
}

void drawPlayers(int y1, int y2)
{
	SDL_Rect player1 = { 20, y1, PLAYER_WIDTH, PLAYER_HEIGHT };
	SDL_Rect player2 = { SCREEN_WIDTH - PLAYER_WIDTH - 20, y2, PLAYER_WIDTH, PLAYER_HEIGHT };

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &player1);
	SDL_RenderFillRect(renderer, &player2);
}

void drawBall(struct Ball ball)
{
	SDL_Rect rect = { ball.x, ball.y, ball.w, ball.h };

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
}

void resetBall(struct Ball *ball, char *msg)
{
	printf("%s", msg);
	ball->x = SCREEN_WIDTH / 2 - ball->w / 2;
	ball->y = SCREEN_HEIGHT / 2 - ball->h / 2;
}

void loadFont()
{
	font = TTF_OpenFont("arial.ttf", 72);
	if (font == NULL)
		printf("Failed to load font! Error: %s\n", TTF_GetError());
}

BTexture loadTextTexture(char *text, SDL_Color color)
{
	// Convert *text to string before calling this function
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
	BTexture textTexture;

	if (textSurface == NULL)
		printf("Unable to render text surface! Error: %s\n", TTF_GetError());
	else
	{
		textTexture.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (textTexture.texture == NULL)
			printf("Unable to create texture from renderer text! Error: %s\n", SDL_GetError());
		else
		{
			textTexture.w = textSurface->w;
			textTexture.h = textSurface->h;
		}

		SDL_FreeSurface(textSurface);
	}

	return textTexture;
}

void renderTextTexture(BTexture textTexture, int x, int y)
{
	SDL_Rect rect = { x, y, textTexture.w, textTexture.h };
	SDL_RenderCopy(renderer, textTexture.texture, NULL, &rect);
}

int main(int argc, char* args[])
{
	Player player1;
	Player player2;

	struct Ball ball;
	struct Vector vel;

	// Players vars
	int moveP1, moveP2;
	player1.score = player2.score = 0;

	// Keyboard
	const Uint8* keyState;

	char buffer[65];
	int r = 10;

	// Color

	SDL_Color textColor = { 255, 255, 255, 255 };
	
	// Init players positions
	moveP1 = moveP2 = SCREEN_HEIGHT / 2 - PLAYER_HEIGHT / 2;

	// Init ball
	ball.w = ball.h = 10;
	ball.x = SCREEN_WIDTH / 2 - ball.w / 2;
	ball.y = SCREEN_HEIGHT / 2 - ball.w / 2;	
	ball.speed = 4;

	// Init ball velocity
	vel.x = 1;
	vel.y = 1;

	if (initSDL());
	{
		loadFont();

		scorePlayer1 = loadTextTexture(_itoa(player1.score, buffer, r), textColor);
		scorePlayer2 = loadTextTexture(_itoa(player2.score, buffer, r), textColor);
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
				if ((moveP1 + PLAYER_HEIGHT) < SCREEN_HEIGHT)
				{
					moveP1 += PLAYER_SPEED;
					//printf("Player1: %d\n", moveP1 + PLAYER_HEIGHT);
				}
			}
			if (keyState[SDL_SCANCODE_S])
			{
				if (moveP1 > 10)
				{
					moveP1 -= PLAYER_SPEED;
					//printf("Player1: %d\n", moveP1);
				}
			}
			if (keyState[SDL_SCANCODE_J])
			{
				if ((moveP2 + PLAYER_HEIGHT) < SCREEN_HEIGHT)
				{
					moveP2 += PLAYER_SPEED;
					//printf("Player2: %d\n", moveP2 + PLAYER_HEIGHT);
				}
			}
			if (keyState[SDL_SCANCODE_K])
			{
				if (moveP2 > 10)
				{
					moveP2 -= PLAYER_SPEED;
					//printf("Player2: %d\n", moveP2);
				}
			}

			// Bounce of player1
			if ( (ball.x <= (20 + PLAYER_WIDTH)) && ((ball.y >= moveP1) && (ball.y <= moveP1 + PLAYER_HEIGHT)) )
			{
				vel.x *= -1;
			}

			// Bounce of player2
			else if ( (ball.x + 10 >= (SCREEN_WIDTH - PLAYER_WIDTH - 20)) && ((ball.y >= moveP2) && (ball.y <= moveP2 + PLAYER_HEIGHT)) )
			{
				vel.x *= -1;
			}

			// Bounce from top and bottom edges
			if ( (ball.y + 10 >= SCREEN_HEIGHT) || (ball.y <= 0) )
			{
				vel.y *= -1;
			}

			// Player 1 won
			if (ball.x + ball.w >= SCREEN_WIDTH)
			{
				player1.score += 1;
				scorePlayer1 = loadTextTexture(_itoa(player1.score, buffer, r), textColor);
				resetBall(&ball, "Player 1 won!\n");
			}

			// Player 2 won
			if (ball.x + ball.w <= 0)
			{
				player2.score += 1;
				scorePlayer2 = loadTextTexture(_itoa(player2.score, buffer, r), textColor);
				resetBall(&ball, "Player 2 won!\n");
			}

			ball.x += (int)vel.x * ball.speed;
			ball.y += (int)vel.y * ball.speed;

			// Clear screen
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(renderer);

			// Draw objects
			drawPlayers(moveP1, moveP2);
			drawBall(ball);

			// Draw text
			renderTextTexture(scorePlayer1, SCREEN_WIDTH / 4 - scorePlayer1.w / 2, 20);
			renderTextTexture(scorePlayer2, SCREEN_WIDTH * 3 / 4 - scorePlayer2.w / 2, 20);

			// Update screen
			SDL_RenderPresent(renderer);
		}

		closeSDL();
	}

	return 0;
}