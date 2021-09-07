#include "SnakeGame.h"


int maxheight, maxwidth, map;
void PlayGame();
int IsUserReady();
int AskUserToPlayAgain();
void ClearCentre();
int UserInput();


int main ()
{	
	
	if (IsUserReady() == 'y')
	do {
		{	
			
			SnakeGame NewSnake;
			
			NewSnake.PlayGame();
			
						
		}
	}
	while (AskUserToPlayAgain() == 'y');
	return 0;
}


//화면초기화, 커서 중앙 배치
void ClearCentre(float x, float y)
{
	//처음은 상관없지만 두번째이후부터는 현재화면을 치워야하니 clear()
	clear();	
	initscr(); 
	noecho();
	getmaxyx(stdscr, maxheight, maxwidth);
	move((maxheight/y), (maxwidth/x));
}


//유저입력받기
int UserInput()
{
	int UserInput = getch();
	refresh();
	endwin();
	clear();

	return UserInput;	
}

// print start menu
int IsUserReady() 
{	//메시지 위치 배치먼저
	ClearCentre(3, 2.5);
	printw("Snake Game, Are you ready? press Y or N");
	return UserInput();
}

//종료시 출력
int AskUserToPlayAgain()
{
	ClearCentre(2.5, 2.5);
	printw("Are you play again? press Y or N");
	return UserInput();
}
