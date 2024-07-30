#include "include/raylib.h"
#include "iostream"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

using namespace std;


//GlobalVariables
static const int screenWidth = 800;
static const int screenHeight = 450;
static const int PLAYER_MAX_LIFE = 3;
static const int BRICKS_PER_LINE = 10;
static const int LINES_OF_BRICKS = 3;

//Functions
static void Update(void);
static void Draw(void);
static void Init(void);


//Classes
typedef struct Player{
	Vector2 position;
	Vector2 size;
	int life;
}Player;

typedef struct Brick{
	Vector2 position;
	bool active;
}Brick;

//InitClasses
Player player;	
Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE];
Vector2 brickSize;

int main()
{
	InitWindow(screenWidth,screenHeight,"Arkoid -Remake");
	
	Init();
	
	while(!WindowShouldClose())
	{
		Update();
		Draw();
	}
	
	CloseWindow();
	return 0;
}

void Init(void)
{
	//globals 
	SetTargetFPS(60);
	
	//player
	player.position = (Vector2){ screenWidth/2, screenHeight*7/8 };
	player.size = (Vector2){ screenWidth/10, 20 };
	player.life = PLAYER_MAX_LIFE;
	
	//bircks init
	brickSize = (Vector2){(float)GetScreenWidth()/(float)BRICKS_PER_LINE, 40 };
	
     int initialDownPosition = 50;

     for (int i = 0; i < LINES_OF_BRICKS; i++)
     {
        for (int j = 0; j < BRICKS_PER_LINE; j++)
        {
            brick[i][j].position = (Vector2){ j*brickSize.x + brickSize.x/2, i*brickSize.y + initialDownPosition };
            brick[i][j].active = true;
        }
     }
}

void Update(void)
{
	//player
	if(IsKeyDown(KEY_LEFT)) player.position.x -= 5;
	if ((player.position.x - player.size.x/2) <= 0) player.position.x = player.size.x/2;
	if(IsKeyDown(KEY_RIGHT)) player.position.x += 5;
	if((player.position.x - player.size.x/2) >= screenWidth) player.position.x = screenWidth - player.size.x/2;
}

void Draw(void)
{
	BeginDrawing();
		//globals
		ClearBackground(WHITE);
		DrawFPS(10,10);
		
		//player
		DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, BLACK);
		
		//player health
		 for (int i = 0; i < player.life; i++) DrawCircle(20 + 40*i, screenHeight - 30, 15, RED);
		
		//Bricks
 for (int i = 0; i < LINES_OF_BRICKS; i++)
            {
                for (int j = 0; j < BRICKS_PER_LINE; j++)
                {
                    if (brick[i][j].active)
                    {
                        if ((i + j) % 2 == 0) DrawRectangle(brick[i][j].position.x - brickSize.x/2, brick[i][j].position.y - brickSize.y/2, brickSize.x, brickSize.y, BLUE);
                        else DrawRectangle(brick[i][j].position.x - brickSize.x/2, brick[i][j].position.y - brickSize.y/2, brickSize.x, brickSize.y, SKYBLUE);
                    }
                }
            }		
		
	EndDrawing();


}


