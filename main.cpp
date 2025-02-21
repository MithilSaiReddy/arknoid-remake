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
static const int PLAYER_MAX_LIFE = 4;
static const int BRICKS_PER_LINE = 20;
static const int LINES_OF_BRICKS = 5;

static bool pause = false;
static bool gameOver = false;

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

typedef struct Ball{
	Vector2 position;
	Vector2 speed;
	bool active;
	int radius;
}Ball;

//InitClasses
Player player;	
Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE];
Vector2 brickSize;
Ball ball;

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
	
	//ball
	ball.position = (Vector2){screenWidth/2,screenHeight * 7/8 - 30};
	ball.speed = (Vector2){0,0};
	ball.radius = 7;
	ball.active = false;
	
	//bircks init
	brickSize = (Vector2){(float)GetScreenWidth()/(float)BRICKS_PER_LINE, 40 };
	
     int initialDownPosition = 30;

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
	if(!gameOver)
    {
    	if(IsKeyPressed(KEY_P)) pause = !pause;
    	
    	if(!pause){
	//player
	if(IsKeyDown(KEY_LEFT)) player.position.x -= 5;
	if ((player.position.x - player.size.x/2) <= 0) player.position.x = player.size.x/2;
	if(IsKeyDown(KEY_RIGHT)) player.position.x += 5;
	if((player.position.x - player.size.x/2) >= screenWidth) player.position.x = screenWidth - player.size.x/2;
	
	//BALL
	//ball launch 
	if(!ball.active)
	{
		if(IsKeyPressed(KEY_SPACE))
		{
			ball.active = true;
			ball.speed = (Vector2){0,-5};
		}
	}
	//ball movement
	if (ball.active)
	{
		ball.position.x += ball.speed.x;
 		ball.position.y += ball.speed.y;
	}
	else
	{
		ball.position = (Vector2){screenWidth/2,screenHeight * 7/8 - 30};
	}
	//ball and wall collision
	if (((ball.position.x + ball.radius) >= screenWidth) || ((ball.position.x - ball.radius) <= 0 )) ball.speed.x *= -1;
	if((ball.position.y - ball.radius) <= 0) ball.speed.y *= -1;
	if((ball.position.y + ball.radius) >= screenHeight)
	{
		ball.speed = (Vector2){0,0};
		ball.active = false;
		player.life--;
	}
	//ball and player collision 
	if (CheckCollisionCircleRec(ball.position, ball.radius,
	(Rectangle){player.position.x - player.size.x/2, player.position.y - player.size.y/2,player.size.x,player.size.y}))
	{
		if(ball.speed.y > 0)
		{
			ball.speed.y *= -1;
			ball.speed.x = (ball.position.x - ball.position.y)/ (player.size.x/2 * 2);
		}
	}
	//ball and brick collsiion 
            for (int i = 0; i < LINES_OF_BRICKS; i++)
            {
                for (int j = 0; j < BRICKS_PER_LINE; j++)
                {
                    if (brick[i][j].active)
                    {
                        // Hit below
                        if (((ball.position.y - ball.radius) <= (brick[i][j].position.y + brickSize.y/2)) &&
                            ((ball.position.y - ball.radius) > (brick[i][j].position.y + brickSize.y/2 + ball.speed.y)) &&
                            ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x/2 + ball.radius*2/3)) && (ball.speed.y < 0))
                        {
                            brick[i][j].active = false;
                            ball.speed.y *= -1;
                        }
                        // Hit above
                        else if (((ball.position.y + ball.radius) >= (brick[i][j].position.y - brickSize.y/2)) &&
                                ((ball.position.y + ball.radius) < (brick[i][j].position.y - brickSize.y/2 + ball.speed.y)) &&
                                ((fabs(ball.position.x - brick[i][j].position.x)) < (brickSize.x/2 + ball.radius*2/3)) && (ball.speed.y > 0))
                        {
                            brick[i][j].active = false;
                            ball.speed.y *= -1;
                        }
                        // Hit left
                        else if (((ball.position.x + ball.radius) >= (brick[i][j].position.x - brickSize.x/2)) &&
                                ((ball.position.x + ball.radius) < (brick[i][j].position.x - brickSize.x/2 + ball.speed.x)) &&
                                ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y/2 + ball.radius*2/3)) && (ball.speed.x > 0))
                        {
                            brick[i][j].active = false;
                            ball.speed.x *= -1;
                        }
                        // Hit right
                        else if (((ball.position.x - ball.radius) <= (brick[i][j].position.x + brickSize.x/2)) &&
                                ((ball.position.x - ball.radius) > (brick[i][j].position.x + brickSize.x/2 + ball.speed.x)) &&
                                ((fabs(ball.position.y - brick[i][j].position.y)) < (brickSize.y/2 + ball.radius*2/3)) && (ball.speed.x < 0))
                        {
                            brick[i][j].active = false;
                            ball.speed.x *= -1;
                        }
                    }
                }
            }
		if(player.life <= 0) gameOver = true;
		else//game end 
		{
			gameOver = true;
			for(int i = 0; i < LINES_OF_BRICKS;i++)
			{
				for(int j = 0; j < BRICKS_PER_LINE; j++)
				{
					if(brick[i][j].active) gameOver = false;	
				}
			}
		}
	   }
	}
	else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            Init();
            gameOver = false;
        }
    }	
}

void Draw(void)
{
	BeginDrawing();
		//globals
		ClearBackground(WHITE);
		DrawFPS(600,400);
	if(!gameOver){
		//player
		DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, BLACK);
		
		//player health
		 for (int i = 0; i < player.life; i++) DrawCircle(20 + 40*i, screenHeight - 30, 15, RED);
		
		//ball
		DrawCircleV(ball.position,ball.radius,YELLOW);
		
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
            
            //User Interface
            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40,BLACK);
        }
        else DrawText("GAME OVER!! PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("GAME OVER!! PRESS [ENTER] TO PLAY AGAIN", 30)/2, GetScreenHeight()/2 - 50,30, BLACK);		
		
	EndDrawing();


}


