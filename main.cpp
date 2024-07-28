#include "include/raylib.h"
#include "iostream"

using namespace std;

int main()
{
	InitWindow(800,800,"Arkoid -Remake");
	
	while(!WindowShouldClose())
	{
		BeginDrawing();
			
			ClearBackground(WHITE);
				
		EndDrawing();
	}
	
	CloseWindow();
	return 0;
}

