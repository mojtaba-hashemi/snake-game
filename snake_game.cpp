// Project Name: 	Snake game
// Project Author:	Mojtaba Hashemi
// Modified Date:	Tuseday - 2023 26 September

#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <fstream>


using namespace std;
#define WIDTH 20
#define HEIGHT 20
char Level[HEIGHT][WIDTH];

// FUNCTION DECLARATIONS
void Initialize(int size, int snakeX[], int snakeY[]);
void ClearScreen();
void BuildLevel();
void ReDraw();
void Update(int &size, int snakeX[], int snakeY[], int tempX[], int tempY[], int &item_count, int &points);
void SpeedUpdate();
void Items(vector<int> &itemX, vector<int> &itemY, int &item_count, int snakeX[], int snakeY[], int &size, int tailX, int tailY, int &points);
bool IsGameOver(int snakeX[], int snakeY[], int size);
void HighScore(int points);


// MAIN
int main() {
	int size = 3;
	int item_count = 0;
	int points = 0;
	int snakeX[100], snakeY[100], tempX[100], tempY[100];

	Initialize(size, snakeX, snakeY);
	BuildLevel();
	Update(size, snakeX, snakeY, tempX, tempY, item_count, points);
	HighScore(points);

	_getch();
	return 0;
}

// INITIALIZE SNAKE
void Initialize(int size, int snakeX[], int snakeY[]) {
	snakeX[0] = WIDTH/2;
	snakeY[0] = 3;
	snakeX[1] = WIDTH/2;
	snakeY[1] = 2;
	snakeX[2] = WIDTH/2;
	snakeY[2] = 1;
	for (int i=3; i<100; i++) {
		snakeX[i] = NULL;
		snakeY[i] = NULL;
	}


	for(int i=0; i<size; i++)
		Level[snakeY[i]][snakeX[i]] = 'o';

}



// BUILD LEVEL
void BuildLevel() {
	for (int i=0; i<HEIGHT; i++) {
		for(int j=0; j<WIDTH; j++) {
			Level[0][j] = '*';
			Level[i][0] = '*';
			Level[i][WIDTH-1] = '*';
			Level[HEIGHT-1][j] = '*';
		}
	}
}



// DISPLAY LEVEL
void ReDraw() {
	for (int i=0; i<HEIGHT; i++) {
		cout << endl;
		for(int j=0; j<WIDTH; j++)
			cout << " " << Level[i][j];
	}
}



// CLEAR SCREEN
void ClearScreen() {
	HANDLE hOut;
	COORD Position;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	Position.X = 0;
	Position.Y = 0;
	SetConsoleCursorPosition(hOut,Position);
}





// UPDATE
void Update(int &size, int snakeX[], int snakeY[], int tempX[], int tempY[], int &item_count, int &points) {
	int count = 0;
	char input = ' ';
	char previnput = 's';
	int tailX, tailY;
	bool gameOver = false;
	vector<int> itemX, itemY;

	while(!gameOver) {
		SpeedUpdate();

		tailY = snakeY[size-1];
		tailX = snakeX[size-1];

		Level[tailY][tailX] = ' ';
		Items(itemX, itemY, item_count, snakeX, snakeY, size, tailX, tailY, points);




		for (int i=0; i<size-1; i++) {
			tempX[i+1] = snakeX[i];
			tempY[i+1] = snakeY[i];
		}



		tempX[0] = snakeX[0];
		tempY[0] = snakeY[0];




		for (int i=0; i<size; i++) {
			snakeX[i] = tempX[i];
			snakeY[i] = tempY[i];
		}



		if(_kbhit())
			input = _getch();


		if(previnput == 's')
		{
			snakeY[0] += 1;
			if(input == 'a' || input == 'A')
			previnput = 'a';
			else if (input == 'd' || input == 'D')
			previnput = 'd';
		}
		
		else if (previnput == 'w')
		{
			snakeY[0] -=1;
			if(input == 'a' || input =='A')
			previnput = 'a';
			else if (input == 'd' || input =='D')
			previnput = 'd';
		}
		
		// WHILE MOVING RIGHT
		else if (previnput =='d')
		{
			snakeX[0] += 1;
			if(input == 'w' || input == 'W')
				previnput = 'w';
			else if (input == 's' || input == 'S')
				previnput ='s';
		}
		
		// WHILE MOVING LEFT
		else if (previnput == 'a')
		{
			snakeX[0] -= 1;
			if(input == 'w' || input == 'W')
				previnput = 'w';
			else if (input == 's' || input == 'S')
				previnput = 's';
		}
		
		// Set snake new position
		for(int i =0; i<size; i++)
			Level[snakeY[i]][snakeX[i]] = 'o';
			
		// Check if game is over
		gameOver = IsGameOver(tempX, tempY, size);
		if(gameOver == true)
		{
			cout << "\a\a";
			break;
		}
		
		// Redraw
		ClearScreen();
		ReDraw();
		
	}
}



// ITEMS
void Items(vector<int> &itemX, vector<int> &itemY, int &item_count, int snakeX[], int snakeY[], int &size, int tailX, int tailY, int &points)
{
	clock_t interval = clock() % 3000;
	cout << "\t\t	points: " << points << endl;
	
	
	// Show items at certain intervals
	if(interval > 2800)
	{
		item_count++;
		srand((unsigned)time(NULL));
		// Item is placed at random locations
		itemX.push_back( rand() % (WIDTH-2)+1);
		itemY.push_back( rand() % (HEIGHT-2)+1);
		
		Level[itemY.back()][itemX.back()] = '@';
	}
	
	
	// Check collision with item
	for (int i=0; i<item_count; i++)
	{
		if(	(snakeX[0] == itemX.at(i)) && (snakeY[0] == itemY.at(i)) ) 
		{
			points += 100;
			// Delete item
			item_count--;
			itemY.erase(itemY.begin()+i);
			itemX.erase(itemX.begin()+i);
			// Shake grows
			size++;
			snakeX[size-1] = tailX;
			snakeY[size-1] = tailY;
			cout << "\a";
		}
	}
}


// SPEED
void SpeedUpdate()
{
	if(clock() <= 9000)
	{
		Sleep(100);
		cout << endl << " Level 1";
	}
	else if ((clock() > 9000) && (clock() < 18000))
	{
		Sleep(50);
		cout << endl << " Level 2";
	}
	else if ((clock() > 18000) && (clock() < 36000))
	{
		Sleep(25);
		cout << endl << " Level 3";
	}
	else
	cout << endl << " Level 4";

}




// IS GAME OVER
bool IsGameOver(int snakeX[], int snakeY[], int size)
{
	// if snake collides with tail
	for(int i=2; i<size; i++)
	{
		if(	(snakeX[0] == snakeX[i]) && (snakeY[0] == snakeY[i]) )
		{
			cout << endl << "\t\t	YOU LOSE!";
			_getch();
			return true;
		}
	}
	if((snakeX[0] == WIDTH-1) || (snakeY[0] == HEIGHT-1) || (snakeX[0] == 0) || (snakeY[0] == 0))
	{
		cout << endl << "\t\t	YOU LOSE!";
		_getch();
		return true;
	}
	else
		return false;
}



// HIGH SCORES
void HighScore(int points)
{
	// To do 
	// high score development 
	
	//char fileName[] = "H"
}





























































