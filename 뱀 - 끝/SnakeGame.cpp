#include "SnakeGame.h"
#include <unistd.h>


using namespace std;


CharPosition::CharPosition(int col, int row)
{
	x = col;
	y = row;
}

CharPosition::CharPosition()
{
	x = 0;
	y = 0;
}


SnakeGame::SnakeGame()
{
	// 변수 초기화
	headchar = (char)229;
	partchar = 'x';
	edgechar = (char)64;
	fruitchar = 'x';
	fruit.x = 0;
	fruit.y = 0;
	score = 0;
	map=0;
	//------------------
	gate1.x=0;
	gate1.y=0;
	gate2.x=0;
	gate2.y=0;
	gateadd=true;
	gate=false;
	//--------------
	fruitnum=0;
	poisonnum=0;
	gatenum=0;
	poison.x=0;
	poison.y=0;
	bad='!';
	get_poison=0;
	mission=0;
	mission1=0;		//달성된 미션의 갯수, 4가 되어야 다음 스테이지
	mission2=0;
	mission3=0;		//어케할지 몰라서 그냥 무작정 4개를 두었음
	mission4=0;
	foodtime=0;
	poisontime=0;

	//추가된부
	///////////////////////////////////////////////
	random.x = 0;
	random.y = 0;
	get_random = 0;
	randomtime = 0;
	check = 0;
	randnum = 0;
	///////////////////////////////////////////////

	//del은 딜레이 1000000이 1초 50만은 0.5초
	del = 100000;
	delay = 5000000;
	bool bEatsFruit = 0;
	start_color();
	init_pair(1, COLOR_GREEN, COLOR_GREEN); // fruit 색 지정
	init_pair(2, COLOR_RED, COLOR_RED); 	// poison 색 지정
	init_pair(3, COLOR_YELLOW, COLOR_YELLOW); //snake 몸 색
	init_pair(4, COLOR_BLUE, COLOR_BLUE); 	//snake 머리 색
	init_pair(5, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(6, COLOR_CYAN, COLOR_CYAN); // 5는 벽, 6은 Immune Wall 색지정
	init_pair(7, COLOR_RED, COLOR_WHITE); // 미션판 색 지정
	init_pair(8, COLOR_WHITE, COLOR_WHITE);	//게이트 색 지정
	direction = 'l';			//초기 이동방향
	srand(time(NULL));
	InitGameWindow();
	//maxwidth -20한 이유는 점수를 오른쪽에 출력해야하는데 그 자리만들라고
	maxwidth-=20;
	PositionFruit();
	putPoison();
	putRandom();
	DrawWindow();
	DrawSnake();
	PrintScore();
	refresh();
}

SnakeGame::~SnakeGame()
{
	nodelay(stdscr, false);
	getch();
	endwin();
}

//다음 스테이지 넘어갈 시 변수들의 초기화 실행부분
//score와 snake의 속도 뱀의 이동방향을 초기화시킴 그 외에 fruit,poison,gate,mission등 점수판관련도 초기화
void SnakeGame::reset(){
	gate1.x=0;
	gate1.y=0;
	gate2.x=0;
	gate2.y=0;
	fruitnum=0;
	poisonnum=0;
	gatenum=0;
	mission=0;
	mission1=0;
	mission2=0;
	mission3=0;
	mission4=0;
	foodtime=0;
	poisontime=0;
	randomtime=0;
	score=0;
	del=100000;
	direction='l';
	gateadd=true;


}
//목표조건 달성시 실행, 다음 스테이지(맵)으로 넘어가고 변수들 초기화
void SnakeGame::nextStage(){
	clear();
	refresh();
	reset();
	InitGameWindow();
	maxwidth-=20;
	PositionFruit();
	putPoison();
	putRandom();
	DrawWindow();
	DrawSnake();
	snake.assign(snake2.begin(), snake2.end());	//스테이지 종료후 snake벡터를 초기화 길이를
	PrintScore();
	refresh();
}

//게이트 생성  일단 생성 조건은 mission이 3개클리어시로 지정
void SnakeGame::makeGate(){
	if(gateadd == false){
		return;
	}else{
		while(1){
			int tmx = rand()%maxwidth;
			int tmy = rand()%maxheight;

			if(map==0){
				//immune wall 위에 있으면 재시도
				if((tmy==0 and (tmx==0 or tmx==maxwidth-1)) or (tmy==maxheight-1 and (tmx==0 or
							       	tmx==maxwidth-1))){
					continue;

				}else if((tmy==0 and (tmx>=0 and tmx<=maxwidth-1)) or (tmy==maxheight-1 and (tmx>=0
								and tmx<=maxwidth-1)) or (tmx==0 and(
									tmy>=0 and tmy<=maxheight-1)) or (tmx==maxwidth-1
									and (tmy>=0 and tmy<=maxheight-1))){

					gate1.x=tmx;
					gate1.y=tmy;
					break;
				}else{
					continue;
				}
			}else if(map==1){
				if((tmy==0 and (tmx==0 or tmx==maxwidth-1 or maxwidth/2)) or (tmy==maxheight-1 and
							(tmx==0 or tmx==maxwidth-1 or maxwidth/2)) or (tmx==maxwidth/2
								and (tmy==0 or tmy==maxheight-1))){
					continue;
				}
				else if((tmy==0 and (tmx>=0 and tmx<=maxwidth-1)) or (tmy==maxheight-1 and (tmx>=0
								and tmx<=maxwidth-1)) or (tmx==0 and(
									tmy>=0 and tmy<=maxheight-1)) or (tmx==maxwidth-1
									and (tmy>=0 and tmy<=maxheight-1)) or
				      (tmx==maxwidth/2 and ((tmy>=1 and tmy<maxheight/2-2) or (tmy>maxheight/2+2 and
											     tmy<=maxheight-2)))){
					gate1.x=tmx;
					gate1.y=tmy;
					break;
				}else{
					continue;
				}
			}else if(map==2){
				if((tmy==0 and (tmx==0 or tmx==maxwidth-1)) or (tmy==maxheight-1 and (tmx==0 or
								tmx==maxwidth-1))){
					continue;
				}else if((tmy==0 and (tmx>=0 and tmx<=maxwidth-1)) or (tmy==maxheight-1 and (tmx>=0
								and tmx<=maxwidth-1)) or (tmx==0 and (
									tmy>=0 and tmy<=maxheight-1)) or (tmx==maxwidth-1
									and (tmy>=0 and tmy<=maxheight-1))){
					gate1.x=tmx;
					gate1.y=tmy;
					break;
				}else if((tmy==(maxheight/4+2)) and (tmx>=(maxwidth/4+5) and tmx<=((maxwidth*3)/4-5))){
					gate1.x=tmx;
					gate1.y=tmy;
					break;			//map2의 가운데의 위쪽

				}else if((tmy==((maxheight*3)/4-3)) and (tmx>=(maxwidth/4+5) and tmx<=((maxwidth*3)/4-5))){
					gate1.x=tmx;
					gate1.y=tmy;
					break;			//map2의 가운데의 아래쪽
				}else if(tmx==(maxwidth/4+5) and (tmy>maxheight/4+2 and tmy<((maxheight*3)/4-2))){
					gate1.x=tmx;
					gate1.y=tmy;
					break;			//map2의 가운데의 왼쪽
				}else if(tmx==((maxwidth*3)/4-6) and (tmy>maxheight/4+2 and tmy<((maxheight*3)/4-2))){
					gate1.x=tmx;
					gate1.y=tmy;
					break;			//map2의 가운데의 오른쪽
				}else{
					continue;
				}
			}else if(map==3){
				if((tmy==0 and (tmx==0 or tmx==maxwidth-1)) or (tmy==maxheight-1 and (tmx==0 or
								tmx==maxwidth-1))){
					continue;
				}else if((tmy==0 and (tmx>=0 and tmx<=maxwidth-1)) or (tmy==maxheight-1 and (tmx>=0
								and tmx<=maxwidth-1)) or (tmx==0 and (tmy>=0
									and tmy<=maxheight-1)) or (tmx==maxwidth-1
									and (tmy>=0 and tmy<=maxheight-1))){
					
					if((tmx==maxwidth-1 and tmy==maxheight/4) or (tmx==0 and tmy==(maxheight*3)/4)){
						continue;
					}else{
					
					gate1.x=tmx;
					gate1.y=tmy;
					break;
					}
				}else if((tmy==(maxheight*3)/4 and (tmx>0 and tmx<(maxwidth*4)/5)) or (tmx==(maxwidth*4)/5
							and (tmy>maxheight/2 and tmy<(maxheight*3)/4)) or (tmx==maxwidth/5
							and (tmy>maxheight/4 and tmy<maxheight/2)) or (tmy==maxheight/4 
							and (tmx>maxwidth/5 and tmx<maxwidth-1))){
					
					//map3의 내부 선 부분
					gate1.x=tmx;
					gate1.y=tmy;
					break;
				}else{
					continue;
				}

			}
		}
	}
	attron(COLOR_PAIR(8));
	move(gate1.y, gate1.x);
	addch((char)33);	

}

void SnakeGame::makeGate2(){
	if(gateadd==false){
		return;
	}else{
		while(1){
			int tmpx=rand()%maxwidth;
			int tmpy=rand()%maxheight;

			if(map==0){
				if((tmpy==0 and (tmpx==0 or tmpx==maxwidth-1)) or (tmpy==maxheight-1 and (tmpx==0 or
							       tmpx==maxwidth-1))){
				continue;
				}else if((tmpy==0 and (tmpx>=0 and tmpx<=maxwidth-1)) or (tmpy==maxheight-1 and (tmpx
							>=0 and tmpx<=maxwidth-1)) or (tmpx==0 and (tmpy>=0 and
							tmpy<=maxheight-1)) or (tmpx==maxwidth-1 and (tmpy>=0 and
							tmpy<=maxheight-1))){
					if(tmpx==gate1.x and tmpy==gate1.y){
						continue;
					}else{
					gate2.x=tmpx;
					gate2.y=tmpy;
					break;
					}

				}else{
					continue;
				}
			}else if(map==1){
				if((tmpy==0 and (tmpx==0 or tmpx==maxwidth-1)) or (tmpy==maxheight-1 and
							(tmpx==0 or tmpx == maxwidth-1)) or
						(tmpx==maxwidth/2 and (tmpy==0 or tmpy==maxheight-1))){
					continue;
				}else if((tmpy==0 and (tmpx>=0 and tmpx<=maxwidth-1)) or (tmpy==maxheight-1 and
							(tmpx>=0 and tmpx<=maxwidth-1)) or (tmpx==0 and (tmpy>=0
									and tmpy<=maxheight-1)) or (tmpx==maxwidth-1
									and (tmpy>=0 and tmpy<=maxheight-1))){
					gate2.x=tmpx;
					gate2.y=tmpy;
					break;
				}else{
					continue;
				}
			}else if(map==2){
				if((tmpy==0 and (tmpx==0 or tmpx==maxwidth-1)) or (tmpy==maxheight-1 and (tmpx==0 or
								tmpx==maxwidth-1))){
					continue;
				}else if((tmpy==0 and (tmpx>=0 and tmpx<=maxwidth-1)) or (tmpy==maxheight-1 and (tmpx
							       >=0 and tmpx<=maxwidth-1)) or (tmpx==0 and (tmpy>=0 and
							       tmpy<=maxheight-1)) or (tmpx==maxwidth-1 and (tmpy>=0 and
								tmpy<=maxheight-1))){
			 		if(tmpx==gate1.x and tmpy==gate1.y){
						continue;
					}else{
					gate2.x=tmpx;
					gate2.y=tmpy;
					break;
					}
				}else if((tmpy==(maxheight/4+2)) and (tmpx>=(maxwidth/4+5) and tmpx<=((maxwidth*3)/4-5))){
					if(tmpx==gate1.x and tmpy==gate1.y){
						continue;
					}else{
						gate2.x=tmpx;
						gate2.y=tmpy;
						break;	//map2의 가운데의 위쪽
					}
				}else if((tmpy==((maxheight*3)/4-3)) and (tmpx>=(maxwidth/4+5) and tmpx<=((maxwidth*3)/4-5))){
					if(tmpx==gate1.x and tmpy==gate1.y){
						continue;
					}else{
						gate2.x=tmpx;
						gate2.y=tmpy;
						break;		//map2의 가운데의 아래쪽
					}
				}else if(tmpx==(maxwidth/4+5) and (tmpy>maxheight/4+2 and tmpy<((maxheight*3)/4-2))){
					if(tmpx==gate1.x and tmpy==gate1.y){
						continue;
					}else{
						gate2.x=tmpx;
						gate2.y=tmpy;
						break;		//map2의 가운데의 왼쪽
					}
				}else if(tmpx==((maxwidth*3)/4-6) and (tmpy>maxheight/4+2 and tmpy<((maxheight*3)/4-2))){
					if(tmpx==gate1.x and tmpy==gate1.y){
						continue;
					}else{
						gate2.x=tmpx;
						gate2.y=tmpy;
						break;		//map2의 가운데의 오른쪽
					}
				}else{
					continue;
				}
			}else if(map==3){
				if((tmpy==0 and (tmpx==0 or tmpx==maxwidth-1)) or (tmpy==maxheight-1 and (tmpx==0 or
								tmpx==maxwidth-1))){
					continue;
				}else if((tmpy==0 and (tmpx>=0 and tmpx<=maxwidth-1)) or (tmpy==maxheight-1 and (tmpx
								>=0 and tmpx<=maxwidth-1)) or (tmpx==0 and (tmpy>=0 and
									tmpy<=maxheight-1)) or (tmpx==maxwidth-1
									and (tmpy>=0 and tmpy<=maxheight-1))){
					if((tmpx==gate1.x and tmpy==gate1.y) or (tmpx==maxwidth-1 and tmpy==maxheight/4)
							or (tmpx==0 and tmpy==(maxheight*3)/4)){
						continue;		// 찾은 좌표가 gate1의 좌표와 동일하다면 다시 찾아 
					}else{
						
						gate2.x=tmpx;
						gate2.y=tmpy;
						break;
					}
				}else if((tmpy==(maxheight*3)/4 and (tmpx>0 and tmpx<(maxwidth*4)/5)) or
						(tmpx==(maxwidth*4)/5 and (tmpy>maxheight/2 and tmpy<(maxheight*3)/4)) or
						(tmpx==maxwidth/5 and (tmpy>maxheight/4 and tmpy<maxheight/2)) or
						(tmpy==maxheight/4 and (tmpx>maxwidth/5 and tmpx<maxwidth-1))){
					if(tmpx==gate1.x and tmpy==gate1.y){
							continue;
							}else{
							gate2.x=tmpx;
							gate2.y=tmpy;
							break;
							}
							

				}else{
					continue;
				}
			}
		}
	}
		move(gate2.y, gate2.x);
		addch((char)33);
		attroff(COLOR_PAIR(8));
		refresh();

}

// initialise the game window
void SnakeGame::InitGameWindow()
{

	initscr(); // initialise the screen
	nodelay(stdscr,TRUE); //
	keypad(stdscr, true); // 방향키를 통해 뱀 이동을 조정할거니 keypad(*win,bool)의 bool true로
	noecho(); // 입출력모드 지정 반향기능을 정지  입력한 값을 화면에 안보이게
	curs_set(0); // 화면에 보이는 커서를 설정, 0은 안보이게, 1은 일반, 2는 큰 커서
	refresh();
	getch();

	//현재 화면의 가로 세로의 크기를 구한다 getmaxyx(win,max_y,max_x) y값의크기와 x의크기를구해 각각 max_y,max_x에 넣음
	//win에 해당하는 stdscr은 표준화면을 의미함
	getmaxyx(stdscr, maxheight, maxwidth);
	return;
}

//반복되는 코드라 그냥 올려버렸음
void SnakeGame::ChangeColor()
{
	attroff(COLOR_PAIR(5));
	attron(COLOR_PAIR(6));
	addch((char)33);
	attroff(COLOR_PAIR(6));
	attron(COLOR_PAIR(5));
}
// 맵 만들기map 변수 값에 따라서 다른 맵을 그리게끔, map 변수는 미션조건 달성시 1씩 늘어난다
void SnakeGame::DrawWindow()
{

	attron(COLOR_PAIR(5));
	if(map==0){					//stage 1
		//뱀새기가 활동할 부분을 테두리로 표기, 이때 Immune Wall은 다른문자로 표기
		for (int i = 0; i < maxwidth; i++) // top
		{
			move(0, i);
			addch(edgechar);
		}
		for (int i = 0; i < maxwidth; i++) // bottom
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		for (int i = 0; i < maxheight; i++) // left side
		{
			move(i, 0);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}
		for (int i = 0; i < maxheight; i++) // right side
		{
			move(i, maxwidth-1);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}
		return;
	}else if(map==1){					//stage 2
		for(int i=0; i< maxwidth; i++) // top
		{
			move(0, i);
			addch(edgechar);
		}
		move(0,maxwidth/2);
		ChangeColor();

		for(int i=0; i<maxwidth; i++) //bottom
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		move(maxheight-1, maxwidth/2);
		ChangeColor();

		//top center와 bottom center 잇는 선 중앙 일부는 뚫려서 지나가는길
		for(int i=1; i<maxheight-1; i++)
		{
			if(i>=maxheight/2-2 and i<=maxheight/2+2){
				continue;
			}
			
			move(i,maxwidth/2);
			addch(edgechar);
		}

		for(int i=0; i<maxheight; i++) // left side
		{
			move(i,0);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}

		for(int i=0; i<maxheight; i++) // right side
		{
			move(i, maxwidth-1);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}

		return;
	}else if(map==2){					//stage 3
		for(int i=0; i<maxwidth; i++) //top
		{
			move(0, i);
			addch(edgechar);
		}
		for(int i=0; i<maxwidth; i++) // bottom
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		for(int i=0; i<maxheight; i++) //left
		{
			move(i,0);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}
		for(int i=0; i<maxheight; i++) // right
		{
			move(i, maxwidth-1);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}
		//맵 중앙을 차지하는 네모 벽, 게이트가 생기는 경우, 네모의 내부에는 생기지않게 immune으로
		for(int i=maxwidth/4+5; i<(maxwidth*3)/4-5; i++)
		{
			for(int j=maxheight/4+2; j<(maxheight*3)/4-2; j++)
			{
				if(i==maxwidth/4+5 or i==(maxwidth*3)/4-6){
					move(j,i);
					addch(edgechar);
				}else if(j==maxheight/4+2 or j==(maxheight*3)/4-3){
					move(j,i);
					addch(edgechar);
				}
				else{
					move(j,i);
					ChangeColor();
				}
			}

		}

		return;
	}else if(map==3){				//stage 4
		for(int i=0; i<maxwidth; i++)
		{
			move(0,i);
			addch(edgechar);
		}
		for(int i=0; i<maxwidth; i++)
		{
			move(maxheight-1, i);
			addch(edgechar);
		}
		for(int i=0; i<maxheight; i++)
		{
			move(i,0);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}
		for(int i=0; i<maxheight; i++)
		{
			move(i,maxwidth-1);
			if(i==maxheight-1 or i==0){
				ChangeColor();
			}else{
				addch(edgechar);
			}
		}
		//회전 하듯이 맵을 나누었다.
		for(int i=1; i<(maxwidth*4)/5; i++)
		{
			move((maxheight*3)/4,i);
			addch(edgechar);
		}
		move((maxheight*3)/4,0);
		ChangeColor();
		for(int i=maxheight/2; i<=(maxheight*3)/4; i++){
			move(i, (maxwidth*4)/5);
			addch(edgechar);
		}
		move((maxheight*3)/4,(maxwidth*4)/5);
		ChangeColor();
		move(maxheight/2, (maxwidth*4)/5);	// immune
		ChangeColor();

		for(int i=maxwidth/5; i<maxwidth; i++)
		{
			move(maxheight/4, i);
			addch(edgechar);
		}
		move(maxheight/4,maxwidth-1);
		ChangeColor();

		for(int i=maxheight/4; i<=maxheight/2; i++)
		{
			move(i,maxwidth/5);
			addch(edgechar);
		}
		move(maxheight/4, maxwidth/5);
		ChangeColor();
		move(maxheight/2, maxwidth/5);		// immune
		ChangeColor();
		return;
	}
	attroff(COLOR_PAIR(5));

}



// draw snake's body
void SnakeGame::DrawSnake()
{

	
	attron(COLOR_PAIR(3));
	if(map==0){
	for (int i = 0; i < 5; i++)
	{
		snake.push_back(CharPosition(20+i, 5));
	}

	//길이가 몇이든 길이만큼 표시를해야함
	for (int i = 0; i < snake.size(); i++)
	{
		move(snake[i].y, snake[i].x);
		addch(partchar);


	}
	attroff(COLOR_PAIR(3));
	return;
	}else if(map==1){
		for(int i=0; i<5; i++){
			snake2.push_back(CharPosition(20+i, 5));
		}
		for(int i=0; i<snake2.size(); i++){
			move(snake2[i].y, snake2[i].x);
			addch(partchar);
		}
		return;
	}else if(map==2){
		for(int i=0; i<5; i++){
			snake3.push_back(CharPosition(20+i, 5));
		}
		for(int i=0; i<snake3.size(); i++){
			move(snake3[i].y, snake3[i].x);
			addch(partchar);
		}
		return;
	}else if(map==3){
		for(int i=0; i<5; i++){
			snake4.push_back(CharPosition(20+i, 5));
		}
		for(int i=0; i<snake4.size(); i++){
			move(snake4[i].y, snake4[i].x);
			addch(partchar);
		}
		return;
	}

}

// 점수표시 우측상단에할거
void SnakeGame::PrintScore()
{
	move(0, maxwidth+5);
	
	attron(COLOR_PAIR(7));
	printw("Score Board");
	attroff(COLOR_PAIR(7));
	move(1, maxwidth+1);
	printw("B: %d", snake.size()/5);
	move(2, maxwidth+1);
	printw("+: %d Items", fruitnum);
	move(3, maxwidth+1);
	printw("-: %d Items", poisonnum);
	move(4, maxwidth+1);
	printw("G: %d Use", gatenum);
	move(5, maxwidth+1);
	printw("-------------------");
	move(6, maxwidth+1);
	printw("Score : %d", score);
	move(7, maxwidth+1);
	printw("-------------------");
	move(9, maxwidth+1);
	printw("----*-Mission-*----");
	move(9, maxwidth+5);
	attron(COLOR_PAIR(7));
	printw("*-Mission-*");
	attroff(COLOR_PAIR(7));
	PrintMission();
	move(15, maxwidth+1);
	printw("end  mission: %d",mission);
	move(maxheight-1, maxwidth+1);
	printw("stage : %d", map+1);
	move(0,0);
	attron(COLOR_PAIR(6));
	addch((char)33);
	attroff(COLOR_PAIR(6));
	return;
}

// score Board 와 Mission 판을 표시, fruit or poison을 먹거나 gate 사용시 변수가 바뀌고 미션조건 달성시
// Mission 판에 달성 여부가 v 표시로 체크된다, 그리고 Mission 4가지가 전부 v 되어야 다음 스테이지로 넘어감
void SnakeGame::PrintMission()
{
	if(map==0){
		if(snake.size()/5>=1){			//
			move(10,maxwidth+1);
			printw("B:   1 (v)");
			mission1=1;
		}
		else{
			move(10, maxwidth+1);
			printw("B:   1 ( )");		//()좌표, 10,maxwidth+9
		}
		if(fruitnum>=2){
			move(11, maxwidth+1);
			printw("+:   2 (v)");
			mission2=1;

		}
		else{
			move(11, maxwidth+1);
			printw("+:   2 ( )");		//()좌표, 12,maxwidth+9
		}
		if(poisonnum>=1){
			move(12, maxwidth+1);
			printw("-:   1 (v)");
			mission3=1;
		}
		else{
			move(12, maxwidth+1);
			printw("-:   1 ( )");
		}
		if(gatenum>=0){
			move(13, maxwidth+1);
			printw("G:   0 (v)");
			mission4=1;
		}
		else{
			move(13, maxwidth+1);
			printw("G:   0 ( )");
		}

	}else if(map==1){
		if(snake.size()/5>=1){
			move(10, maxwidth+1);
			printw("B:   1 (v)");
			mission1=1;
		}
		else{
			move(10, maxwidth+1);
			printw("B:   1 ( )");
		}
		if(fruitnum>=1){
			move(11, maxwidth+1);
			printw("+:   1 (v)");
			mission2=1;
		}
		else{
			move(11, maxwidth+1);
			printw("+:   1 ( )");
		}
		if(poisonnum>=1){
			move(12, maxwidth+1);
			printw("-:   1 (v)");
			mission3=1;
		}
		else{
			move(12, maxwidth+1);
			printw("-:   1 ( )");
		}
		if(gatenum>=0){
			move(13, maxwidth+1);
			printw("G:   0 (v)");
			mission4=1;
		}
		else{
			move(13, maxwidth+1);
			printw("G:   0 ( )");
		}
	}else if(map==2){

		if(snake.size()/5>=1){
			move(10, maxwidth+1);
			printw("B:   1 (v)");
			mission1=1;
		}
		else{
			move(10, maxwidth+1);
			printw("B:   1 ( )");
		}
		if(fruitnum>=1){
			move(11, maxwidth+1);
			printw("+:   1 (v)");
			mission2=1;
		}
		else{
			move(11, maxwidth+1);
			printw("+:   1 ( )");
		}
		if(poisonnum>=1){
			move(12, maxwidth+1);
			printw("-:   1 (v)");
			mission3=1;
		}
		else{
			move(12, maxwidth+1);
			printw("-:   1 ( )");
		}
		if(gatenum>=0){
			move(13, maxwidth+1);
			printw("G:   0 (v)");
			mission4=1;
		}
		else{
			move(13, maxwidth+1);
			printw("G:   0 ( )");
		}
	}else if(map==3){
		if(snake.size()/5>=3){
			move(10, maxwidth+1);
			printw("B:   3 (v)");
			mission1=1;
		}
		else{
			move(10, maxwidth+1);
			printw("B:   3 ( )");
		}
		if(fruitnum>=7){
			move(11, maxwidth+1);
			printw("+:   7 (v)");
			mission2=1;

		}
		else{
			move(11, maxwidth+1);
			printw("+:   7 ( )");
		}
		if(poisonnum>=3){
			move(12, maxwidth+1);
			printw("-:   3 (v)");
			mission3=1;
		}
		else{
			move(12, maxwidth+1);
			printw("-:   3 ( )");
		}
		if(gatenum>=2){
			move(13, maxwidth+1);
			printw("G:   2 (v)");
			mission4=1;
		}
		else{
			move(13, maxwidth+1);
			printw("G:   2 ( )");
		}
	}
}

// 화면에 먹이 출력  임의의 위치여야함
void SnakeGame::PositionFruit()
{

	while(1)
	{
		int tmpx = rand()%maxwidth+1; // 1늘린건 0을 피하기위해서  1~maxwidth까지
		int tmpy = rand()%maxheight+1;

		// 뱀 위에는 나타나지않는다
		for (int i = 0; i < snake.size(); i++)
		{
			if (snake[i].x == tmpx && snake[i].y == tmpy)
			{
				continue; // 뱀 위에 나타나는거였으면 밑에 무시하고 다시 루프돌림
			}
		}
		// 맵에 있으면 넘어간다 **
		if (tmpx >= maxwidth-2 || tmpy >= maxheight-3)
		{
			continue;
		}
		//2,3,4스테이지의 맵까지 넘어가게 포함시킨다
		if(map==1){
			if((tmpx==maxwidth/2 and (tmpy>=0 and tmpy<maxheight/2-2)) or
			 (tmpx==maxwidth/2 and (tmpy<=maxheight and tmpy>maxheight/2+2))){
			continue;
			}
		}else if(map==2){
			if((tmpx>=maxwidth/4+5 and tmpx<=(maxwidth*3)/4-6) and (tmpy>=maxheight/4+2 and
						tmpy<=(maxheight*3)/4-3)) {
		continue;
			}

		}else if(map==3){
			if(((tmpx==maxwidth/5) and (tmpy>=maxheight/4 and tmpy<=maxheight/2)) or
					((tmpy==maxheight/4) and (tmpx>maxwidth/5 and tmpx<maxwidth)) or
					((tmpy==(maxheight*3)/4) and (tmpx>0 and tmpx<=(maxwidth*4)/5)) or
				       ((tmpx==(maxwidth*4)/5) and (tmpy>=maxheight/2 and tmpy<=(maxheight*3)/4))){
			continue;
			}
		}
		// 좌표설정되면 맵상에 표시
		fruit.x = tmpx;
		fruit.y = tmpy;
		break;

	}
	move(fruit.y, fruit.x);
	attron(COLOR_PAIR(1));
	addch(fruitchar);
	attroff(COLOR_PAIR(1));
	refresh();
}

void SnakeGame::putPoison()
{
	while(1)
	{
		int tmpx = rand()%maxwidth+1;
		int tmpy = rand()%maxheight+1;
		for(int i=0; i<snake.size(); i++){
			if((snake[i].x == tmpx && snake[i].y ==tmpy) || (fruit.x == tmpx && fruit.y==tmpy))
				continue;
		}
		if(tmpx >= maxwidth-2 || tmpy >= maxheight-3){
			continue;
		}

		if(map==1){
			if((tmpx==maxwidth/2 and (tmpy>=0 and tmpy<maxheight/2-2)) or
					(tmpx==maxwidth/2 and (tmpy<=maxheight and tmpy>maxheight/2+2))){
				continue;
			}
		}else if(map==2){
			if((tmpx>=maxwidth/4+5 and tmpx<=(maxwidth*3)/4-6) and (tmpy>=maxheight/4+2 and
						tmpy<=(maxheight*3)/4-3)){
				continue;
			}
		}else if(map==3){
			if(((tmpx==maxwidth/5) and (tmpy>=maxheight/4 and tmpy<=maxheight/2)) or
					((tmpy==maxheight/4) and (tmpx>maxwidth/5 and tmpx<maxwidth)) or
					((tmpy==(maxheight*3)/4) and (tmpx>0 and tmpx<=(maxwidth*4)/5)) or
					((tmpx==(maxwidth*4)/5) and (tmpy>=maxheight/2 and tmpy<=(maxheight*3)/4))){
				continue;
			}
		}

		poison.x = tmpx;
		poison.y = tmpy;
		break;
	}
	move(poison.y, poison.x);
	
	attron(COLOR_PAIR(2));
	addch(bad);
	attroff(COLOR_PAIR(2));
	refresh();
}

///////////////////////////////
void SnakeGame::putRandom()
{

 while(1)
 {
  int tmpx = rand()%maxwidth+1;
  int tmpy = rand()%maxheight+1;
  for(int i=0; i<snake.size(); i++){
   if((snake[i].x == tmpx && snake[i].y ==tmpy) || (fruit.x == tmpx && fruit.y==tmpy) ||(poison.x == tmpx && poison.y == tmpy))
    continue;
  }
  if(tmpx >= maxwidth-2 || tmpy >= maxheight-3){
   continue;
  }

  if(map==1){
   if((tmpx==maxwidth/2 and (tmpy>=0 and tmpy<maxheight/2-2)) or
     (tmpx==maxwidth/2 and (tmpy<=maxheight and tmpy>maxheight/2+2))){
    continue;
   }
  }else if(map==2){
   if((tmpx>=maxwidth/4+5 and tmpx<=(maxwidth*3)/4-6) and (tmpy>=maxheight/4+2 and
      tmpy<=(maxheight*3)/4-3)){
    continue;
   }
  }else if(map==3){
   if(((tmpx==maxwidth/5) and (tmpy>=maxheight/4 and tmpy<=maxheight/2)) or
     ((tmpy==maxheight/4) and (tmpx>maxwidth/5 and tmpx<maxwidth)) or
     ((tmpy==(maxheight*3)/4) and (tmpx>0 and tmpx<=(maxwidth*4)/5)) or
     ((tmpx==(maxwidth*4)/5) and (tmpy>=maxheight/2 and tmpy<=(maxheight*3)/4))){
    continue;
   }
  }

  random.x = tmpx;
  random.y = tmpy;
  break;
 }
 move(random.y, random.x);

 if(randnum == 0)
 {
   start_color();
   attron(COLOR_PAIR(1));
   addch(fruitchar);
   attroff(COLOR_PAIR(1));
 }
 else
 {
   start_color();
   attron(COLOR_PAIR(2));
   addch(bad);
   attroff(COLOR_PAIR(2));
 }
 refresh();
}
///////////////////////////////


// set game over situations 종료할거면 true를 return
bool SnakeGame::FatalCollision()
{
	
	// 벽에 부딪칠경우
	if(map==0){		//stage 1 충돌조건 작동 잘됨
		if((snake[0].x==gate1.x and snake[0].y==gate1.y) || (snake[0].x==gate2.x and snake[0].y==gate2.y)){
			return false;
		}
		else if (snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-1)
		{
			return true;
		}
	}else if(map==1){	//stage 2 충돌조건 작동 잘됨
		if((snake[0].x==gate1.x and snake[0].y==gate1.y) || (snake[0].x==gate2.x and snake[0].y==gate2.y)){
			return false;
		}
		else if (snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-1
				|| (snake[0].x==maxwidth/2) and (snake[0].y>=0 and snake[0].y<maxheight/2-2)
				|| (snake[0].x==maxwidth/2) and (snake[0].y<=maxheight and snake[0].y>maxheight/2+2))
		{
			return true;
		}
	}else if(map==2){	//stage 3 충돌조건 작동 잘됨
		if((snake[0].x==gate1.x and snake[0].y==gate1.y) || (snake[0].x==gate2.x and snake[0].y==gate2.y)){
			return false;
		}
		else if(snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-1
				|| (snake[0].x==maxwidth/4+5 or snake[0].x==(maxwidth*3)/4-6) and
				(snake[0].y>=maxheight/4+2 and snake[0].y<(maxheight*3)/4-2)
				|| (snake[0].y==maxheight/4+2 or snake[0].y==(maxheight*3)/4-3) and
				(snake[0].x>=maxwidth/4+5 and snake[0].x<(maxwidth*3)/4-5))
		{
			return true;
		}
	}else if(map==3){	//stage 4 충돌조건 작동 잘됨
		if((snake[0].x==gate1.x and snake[0].y==gate1.y) || (snake[0].x==gate2.x and snake[0].y==gate2.y)){
			return false;
		}
		else if(snake[0].x == 0 || snake[0].x == maxwidth-1 || snake[0].y == 0 || snake[0].y == maxheight-1
				|| (snake[0].x==maxwidth/5) and (snake[0].y>=maxheight/4 and snake[0].y<=maxheight/2)
				|| (snake[0].y==maxheight/4) and (snake[0].x>maxwidth/5 and snake[0].x<maxwidth)
				|| (snake[0].y==(maxheight*3)/4) and (snake[0].x>0 and snake[0].x<=(maxwidth*4)/5)
				|| (snake[0].x==(maxwidth*4)/5) and (snake[0].y>=maxheight/2 and
					snake[0].y<=(maxheight*3)/4))
		{
			return true;
		}
	}


	//자기몸에 부딪친경우
	for (int i = 2; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
		{
			return true;
		}
	}

	return false;
}

//맵상의 fruit먹었을때
bool SnakeGame::GetsFruit()
{


	//뱀새끼 대가리좌표 snake[0].x , snake[0].y가 fruit의 좌표와 동일하면 먹는상황임
	if (snake[0].x == fruit.x && snake[0].y == fruit.y)
	{

		//PositionFruit() fruit을 먹었으니 랜덤한 위치에 재생성
		PositionFruit();
		score +=50;
	       	fruitnum++;
		PrintScore();

		// if score is a multiple of 100, increase snake speed
		if ((score%100) == 0)
		{
			del -= 4000;
		}
		return bEatsFruit = true;
	}// ****
	else if(foodtime%100==0){
		move(fruit.y,fruit.x);
		printw(" ");
		//fruitnum--;
		PositionFruit();
	}


	else
	{
		return bEatsFruit = false;
	}

	return bEatsFruit;

}

bool SnakeGame::getPoison()
{
	if(snake[0].x==poison.x && snake[0].y == poison.y)
	{
		get_poison = true;
		putPoison();
		if(snake.size()==4){
			direction='q';
		}
		score-=50;
		if(score<0){		//score는 음수가 안나오게 한다
			score=0;
		}
		poisonnum++;
		PrintScore();
	}
	//***
	else if(poisontime%100==0){
		move(poison.y,poison.x);
		printw(" ");
		putPoison();
	}
	else{
		return get_poison = false;
	}
	return get_poison;

}

///////////////////////////////////////////////
bool SnakeGame::getRandom()
{
	if(snake[0].x == random.x && snake[0].y == random.y)
	{
		randomtime = 0;
		get_random = true;
		
		if((snake.size() == 4) && randnum == 1){
			direction = 'q';
		}
		if(randnum == 0){
			score += 50;
			check = 0;
			fruitnum++;

		}
		else{
			score -= 50;
			check = 1;
			poisonnum++;
			if(score < 0){
				score = 0;
			}
		}
		PrintScore();
		randnum = rand()%2;
		putRandom();
	}
	else if(randomtime % 100 == 0){
		move(random.y, random.x);
		printw(" ");
		randnum = rand()%2;
		putRandom();
	}
	else{
		if(randnum == 0){
			check = 0;
		}
		else{
			check = 1;
		}
		return get_random = false;
	}

	return get_random;
}



void SnakeGame::MoveSnake()
{
	//snake의 움직일 방향 결정한다, 키 입력받아서 switch로 case를 나눠 실행, BackSpace시 게임 종료
	int KeyPressed = getch();

	switch(KeyPressed)
	{
		case KEY_LEFT:
			if (direction != 'r')
			{ direction = 'l'; break;}
			else if(direction == 'r')
			{ direction = 'q'; break;}
		case KEY_RIGHT:
			if (direction != 'l')
			{ direction = 'r'; break;}
			else if(direction == 'l')
			{ direction = 'q'; break;}
		case KEY_UP:
			if (direction != 'd')
			{ direction = 'u'; break;}
			else if(direction == 'd')
			{ direction = 'q'; break;}
		case KEY_DOWN:
			if (direction != 'u')
			{ direction = 'd'; break;}
			else if(direction == 'u')
			{ direction = 'q'; break;}
		case KEY_BACKSPACE:
			direction = 'q'; // key to quit the game
			break;
	}

	// Fruit과 pPoison을 둘 다 안먹었을때 size 그대로, 앞으로 전진, 맨뒤제거, 새로운 머리, 예전머리 몸통으로표기
	if(check == 0)
	{
		if(!bEatsFruit && !get_random)
		{
			//snake 몸통 vector의 맨 마지막부분을 지우고 vector의 크기도 1줄임
			move(snake[snake.size()-1].y, snake[snake.size()-1].x); // 맨 뒤 요소로 이동
			printw(" ");
			refresh();
			snake.pop_back();
		}
		if(get_poison)
		{
			move(snake[snake.size()-1].y, snake[snake.size()-1].x);
			printw(" ");
			move(snake[snake.size()-2].y, snake[snake.size()-2].x);
			printw(" ");
			refresh();
			snake.pop_back();
		}
	}
	else
	{
		if(!bEatsFruit)
		{
			//snake 몸통 vector의 맨 마지막부분을 지우고 vector의 크기도 1줄임
			move(snake[snake.size()-1].y, snake[snake.size()-1].x); // 맨 뒤 요소로 이동
			printw(" ");
			refresh();
			snake.pop_back();
		}
		if(get_poison || get_random)
		{
			move(snake[snake.size()-1].y, snake[snake.size()-1].x);
			printw(" ");
			move(snake[snake.size()-2].y, snake[snake.size()-2].x);
			printw(" ");
			refresh();
			snake.pop_back();
		}
	}
	
	//gate1에 진입시
	if(snake[0].y==gate1.y and snake[0].x==gate1.x){
		gate=true;
		if(direction=='l'){
			if(gate2.x==0){			//맨왼쪽에 두개생겼을때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x,gate2.y-1));
			}else if(gate2.y==0){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x,gate2.y+1));
			}else if(gate2.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
				       	and gate2.y==0){		//map1 일때 중앙부
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
				       	and gate2.y==maxheight-1){	//map1 일때 중앙부
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y != maxheight-1))
					and gate2.x==0){		//map1 일때 중앙부
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
					and gate2.x==maxwidth-1){	//map1 일때 중앙부
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if((gate1.x==0 and gate2.x==maxwidth/2 and gate2.y==maxheight/2+2) or
					(gate1.x==maxwidth-1 and gate2.x==maxwidth/2 and gate2.y==maxheight/2+2)){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==maxwidth/2 and (gate2.y>0 and gate2.y<maxheight-1)){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
				// 나오는 게이트가 map1의 가운데에 있을때.
			}else if(gate2.y==maxheight/4+2 and (gate2.x>=maxwidth/4+5 and gate2.x<=((maxwidth*3)/4-5))){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 위에 있을 때 
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==(maxheight*3)/4-3 and (gate2.x>=maxwidth/4+5 and gate2.x<=((maxwidth*3)/4-5))){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 밑에 있을 때
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==maxwidth/4+5 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 왼쪽일 때
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==(maxwidth*3)/4-6 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 오른쪽일 때
				direction='r';
			        snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));	
			}else if(gate2.y==(maxheight*3)/4 and (gate2.x>0 and gate2.x<(maxwidth*4)/5)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 아래 평행줄에 있을때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==maxheight/4 and (gate2.x>maxwidth/5 and gate2.x<maxwidth-1)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 위 평행줄일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==(maxwidth*4)/5 and (gate2.y>maxheight/2 and gate2.y<(maxheight*3)/4)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 아래 수직줄일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==maxwidth/5 and (gate2.y>maxheight/4 and gate2.y<maxheight/2)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 위 수직줄일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x==maxwidth-1){
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x>gate1.x and gate2.y<gate1.y){	//1시 방향에 있을때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x>gate1.x and gate2.y>gate1.y){	//5시 방향에 있을때
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}
		}else if(direction =='r'){		//1게이트에 오른쪽으로 진입할때
			if(gate2.x==maxwidth-1){	//맨 오른쪽에 두개
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.y==0){
				direction='d';		//--------------------------------
				snake.insert(snake.begin(), CharPosition(gate2.x,gate2.y+1));
			}else if(gate2.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x,gate2.y-1));
			}else if(gate2.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1,gate2.y));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
					and gate2.y==0){	// 가운데에 1게이트 맨 위에 게이트2
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
					and gate2.y==maxheight-1){	//가운데에 1게이트 맨 밑에 게이트2
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
					and gate2.x==0){	//가운데에 1게이트 맨왼쪽에 2게이트
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if((gate1.x==maxwidth/2 and (gate1.y !=0 or gate1.y !=maxheight-1))
					and gate2.x==maxwidth-1){	//가운데에 1게이트 맨 오른쪽에 2게이트
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==maxwidth/2 and (gate2.y>0 and gate2.y<maxheight-1)){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
				//나오는 게이트가 map1의 가운데에 있을때 
			}else if(gate2.y==maxheight/4+2 and (gate2.x>=maxwidth/4+2 and gate2.x<=(maxwidth*3)/4-5)){
				//map2 에서 오른쪽으로 들어갔는데 2게이트가 가운데의 위일때
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==(maxheight*3)/4-3 and (gate2.x>=maxwidth/4+5 and gate2.x<=(maxwidth*3)/4-5)){
				//map2 에서 오른쪽으로 들어갔는데 2게이트가 가운데의 아래일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==maxwidth/4+5 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2 에서 오른쪽으로 들어갔는데 2게이트가 가운데의 왼쪽일때.
				direction ='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==(maxwidth*3)/4-6 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2 에서 오른쪽으로 들어갔는데 2게이트가 가운데의 오른쪽일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.y==(maxheight*3)/4 and (gate2.x>0 and gate2.x<(maxwidth*4)/5)){
				//map3 에서 오른쪽으로 들어갔는데 2게이트가 아래수평줄일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==maxheight/4 and (gate2.x>maxwidth/5 and gate2.x<maxwidth-1)){
				//map3 에서 오른쪽으로 들어갔는데 2게이트가 위수평줄일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==(maxwidth*4)/5 and (gate2.y>maxheight/2 and gate2.y<(maxheight*3)/4)){
				//map3 에서 오른쪽으로 들어갔는데 2게이트가 아래수직줄일때.
				direction='l';
			      	snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));	
			}else if(gate2.x==maxwidth/5 and (gate2.y>maxheight/4 and gate2.y<maxheight/2)){
				//map3 에서 오른쪽으로 들어갔는데 2게이트가 위수직줄일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x==0){
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x<gate1.x and gate2.y<gate1.y){	//gate2가 11시방향에 있을때
				
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x<gate1.x and gate2.y>gate1.y){	//gate2가 7시 방향에 있을때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}
		}else if(direction =='u'){		//맨 위에 두개생겼을때.
			if(gate2.y==0){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x==maxwidth-1){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==maxheight/4+2 and (gate2.x>=maxwidth/4+5 and gate2.x<=(maxwidth*3)/4-5)){
				//map2 일때 위로 진입했는데 2게이트가 가운데의 위인경우
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==(maxheight*3)/4-3 and (gate2.x>=maxwidth/4+5 and gate2.x<=(maxwidth*3)/4-5)){
				//map2 일때 위로 진입했는데 2게이트가 가운데의 아래인경우
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==maxwidth/4+5 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2 일때 위로 진입했는데 2게이트가 가운데의 왼쪽인경우
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==(maxwidth*3)/4-6 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2 일때 위로 진입했는데 2게이트가 가운데의 오른쪽인경우
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.y==(maxheight*3)/4 and (gate2.x>0 and gate2.x<(maxwidth*4)/5)){
				//map3 일때 위로 진입했는데 2게이트가 아래 수평줄일때. 
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==maxheight/4 and (gate2.x>maxwidth/5 and gate2.x<maxwidth-1)){
				//map3 일때 위로 진입했는데 2게이트가 위 수평줄일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==(maxwidth*4)/5 and (gate2.y>maxheight/2 and gate2.y<(maxheight*3)/4)){
				//map3 일때 위로 진입했는데 2게이트가 아래 수직줄일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==maxwidth/5 and (gate2.y>maxheight/4 and gate2.y<maxheight/2)){
				//map3 일때 위로 진입했는데 2게이트가 위 수직줄일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x<gate1.x and gate2.y>gate1.y){	//7시 방향에 있을때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x>gate1.x and gate2.y>gate1.y){	// 5시 방향에 있을때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}
		}else if(direction=='d'){		//맨 아래에 두개
			if(gate2.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==0){
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==maxwidth-1){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.y==maxheight/4+2 and (gate2.x>=maxwidth/4+5 and gate2.x<=(maxwidth*3)/4-5)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 위일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==(maxheight*3)/4-3 and (gate2.x>=maxwidth/4+5 and gate2.x<=(maxwidth*3)/4-5)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 아래일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==maxwidth/4+5 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 왼쪽일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==(maxwidth*3)/4-6 and (gate2.y>maxheight/4+2 and gate2.y<(maxheight*3)/4-2)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 오른쪽일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.y==(maxheight*3)/4 and (gate2.x>0 and gate2.x<(maxwidth*4)/5)){
				//map3에서 아래로 진입했는데 2게이트가 아래수평선일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y-1));
			}else if(gate2.y==maxheight/4 and (gate2.x>maxwidth/5 and gate2.x<maxwidth-1)){
				//map3에서 아래로 진입했는데 2게이트가 위수평줄일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate2.x, gate2.y+1));
			}else if(gate2.x==(maxwidth*4)/5 and (gate2.y>maxheight/2 and gate2.y<(maxheight*3)/4)){
				//map3에서 아래로 진입했는데 2게이트가 아래수직줄일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}else if(gate2.x==maxwidth/5 and (gate2.y>maxheight/4 and gate2.y<maxheight/2)){
				//map3에서 아래로 진입했는데 2게이트가 위수직줄일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x<gate1.x and gate2.y<gate1.y){	//11시 방향에 있을때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate2.x+1, gate2.y));
			}else if(gate2.x>gate1.x and gate2.y<gate1.y){	//1시방향에있을때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate2.x-1, gate2.y));
			}
		}
		
		// gate2으로 들어갈때의 경우
	}else if(snake[0].y==gate2.y and snake[0].x==gate2.x){
		gate=true;
		if(direction=='l'){
			if(gate1.x==0){			//맨 왼쪽일때
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x,gate1.y-1));
			}else if(gate1.x==maxwidth-1){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1,gate1.y));
			}else if(gate1.y==0){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.y==0){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
				       and gate1.x==maxwidth-1){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if((gate2.x==0 and gate1.x==maxwidth/2 and gate1.y==maxheight/2+2)or
					(gate2.x==maxwidth-1 and gate1.x==maxwidth/2 and gate1.y==maxheight/2+2)){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==maxwidth/2 and (gate1.y>0 and gate1.y<maxheight-1)){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
				// 나오는 게이트가 map1의 가운데에 있을 때
			}else if(gate1.y==maxheight/4+2 and (gate1.x>=maxwidth/4+5 and gate1.x<=((maxwidth*3)/4-5))){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 위에 있을 때
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==(maxheight*3)/4-3 and (gate1.x>=maxwidth/4+5 and gate1.x<=((maxwidth*3)/4-5))){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 밑에 있을 때
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==maxwidth/4+5 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 왼쪽일 때
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==(maxwidth*3)/4-6 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2 일때 왼쪽으로 들어가는 경우 2게이트가 가운데의 오른쪽일 때
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.y==(maxheight*3)/4 and (gate1.x>0 and gate1.x<(maxwidth*4)/5)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 아래수평선일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==maxheight/4 and (gate1.x>maxwidth/5 and gate1.x<maxwidth-1)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 위수평선일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==(maxwidth*4)/5 and (gate1.y>maxheight/2 and gate1.y<(maxheight*3)/4)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 아래수직일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==maxwidth/5 and (gate1.y>maxheight/4 and gate1.y<maxheight/2)){
				//map3 일때 왼쪽으로 들어가는 경우 2게이트가 위수직일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.x==maxwidth-1){
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x>gate2.x and gate1.y<gate2.y){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x>gate2.x and gate1.y>gate2.y){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}
		}else if(direction=='r'){
			if(gate1.x==maxwidth-1){	//맨 오른쪽일때,
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.y==0){		//---------------------
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x,gate1.y+1));
			}else if(gate1.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x,gate1.y-1));
			}else if(gate1.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.y==0){	//gate2가 가운데이고 1게이트가 맨 위일때
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.y==maxheight-1){	// 2게이트가 가운데이고 1게이트가 맨밑
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.x==0){	//2게이트가 가운데이고 1게이트가 맨왼쪽
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if((gate2.x==maxwidth/2 and (gate2.y !=0 or gate2.y !=maxheight-1))
					and gate1.x==maxwidth-1){	//2게이트가 가운데이고 1게이트가 맨오른쪽
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}				//map1 일때. 중앙부 처리
			else if(gate1.x==maxwidth/2 and (gate1.y>0 and gate1.y<maxheight-1)){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
				//나오는 게이트가 map1의 가운데에 있을 때
			}else if(gate1.y==maxheight/4+2 and (gate1.x>=maxwidth/4+5 and gate1.x<=(maxwidth*3)/4-5)){
				//map2에서 오른쪽으로 들어갔는데 2게이트가 가운데의 위일때
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==(maxheight*3)/4-3 and (gate1.x>=maxwidth/4+5 and gate1.x<=(maxwidth*3)/4-5)){
				//map2에서 오른쪽으로 들어갔는데 2게이트가 가운데의 아래일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==maxwidth/4+5 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2에서 오른쪽으로 들어갔는데 2게이트가 가운데의 왼쪽일 때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==(maxwidth*3)/4-6 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2에서 오른쪽으로 들어갔는데 2게이트가 가운데의 오른쪽일 때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.y==(maxheight*3)/4 and (gate1.x>0 and gate1.x<(maxwidth*4)/5)){
				//map3에서 오른쪽으로 들어갔는데 2게이트가 아래수평선일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==maxheight/4 and (gate1.x>maxwidth/5 and gate1.x<maxwidth-1)){
				//map3에서 오른쪽으로 들어갔는데 2게이트가 위수평선일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==(maxwidth*4)/5 and (gate1.y>maxheight/2 and gate1.y<(maxheight*3)/4)){
				//map3에서 오른쪽으로 들어갈때 2게이트가 아래수직일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==maxwidth/5 and (gate1.y>maxheight/4 and gate1.y<maxheight/2)){
				//map3에서 오른쪽으로 들어갈때 2게이트가 위수직일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.x==0){
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.x<gate2.x and gate1.y<gate2.y){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x<gate2.x and gate1.y>gate2.y){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}
		}else if(direction=='u'){
			if(gate1.y==0){			//맨 위일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.x==maxwidth-1){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==maxheight-1){
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==maxheight/4+2 and (gate1.x>=maxwidth/4+5 and gate1.x<=(maxwidth*3)/4-5)){
				//map2에서 위로 진입했는데 2게이트가 맵 가운데의 위일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==(maxheight*3)/4-3 and (gate1.x>=maxwidth/4+5 and gate1.x<=(maxwidth*3)/4-5)){
				//map2에서 위로 진입했는데 2게이트가 맵 가운데의 아래일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==maxwidth/4+5 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2에서 위로 진입했는데 2게이트가 맵 가운데의 왼쪽일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==(maxwidth*3)/4-6 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2에서 위로 진입했는데 2게이트가 맵 가운데의 오른쪽일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.y==(maxheight*3)/4 and (gate1.x>0 and gate1.x<(maxwidth*4)/5)){
				//map3에서 위로 진입했는데 2게이트가 아래수평선일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==maxheight/4 and (gate1.x>maxwidth/5 and gate1.x<maxwidth-1)){
				//map3에서 위로 진입했는데 2게이트가 위수평선일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==(maxwidth*4)/5 and (gate1.y>maxheight/2 and gate1.y<(maxheight*3)/4)){
				//map3에서 위로 진입했는데 2게이트가 아래수직일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==maxwidth/5 and (gate1.y>maxheight/4 and gate1.y<maxheight/2)){
				//map3에서 위로 진입했는데 2게이트가 위수직일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			
			}else if(gate1.x<gate2.x and gate1.y>gate2.y){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.x>gate2.x and gate1.y>gate2.y){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}
		}else if(direction=='d'){
			if(gate1.y==maxheight-1){
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==0){
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==maxwidth-1){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==0){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.y==maxheight/4+2 and (gate1.x>=maxwidth/4+5 and gate1.x<=(maxwidth*3)/4-5)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 위일 때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==(maxheight*3)/4-3 and (gate1.x>=maxwidth/4+5 and gate1.x<=(maxwidth*3)/4-5)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 아래일 때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==maxwidth/4+5 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 왼쪽일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==(maxwidth*3)/4-6 and (gate1.y>maxheight/4+2 and gate1.y<(maxheight*3)/4-2)){
				//map2에서 아래로 진입했는데 2게이트가 맵 가운데의 오른쪽일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.y==(maxheight*3)/4 and (gate1.x>0 and gate1.x<(maxwidth*4)/5)){
				//map3에서 아래로 진입했는데 2게이트가 아래수평선일때.
				direction='u';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y-1));
			}else if(gate1.y==maxheight/4 and (gate1.x>maxwidth/5 and gate1.x<maxwidth-1)){
				//map3에서 아래로 진입했는데 2게이트가 위수평선일때.
				direction='d';
				snake.insert(snake.begin(), CharPosition(gate1.x, gate1.y+1));
			}else if(gate1.x==(maxwidth*4)/5 and (gate1.y>maxheight/2 and gate1.y<(maxheight*3)/4)){
				//map3에서 아래로 진입했는데 2게이트가 아래수직일때.
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}else if(gate1.x==maxwidth/5 and (gate1.y>maxheight/4 and gate1.y<maxheight/2)){
				//map3에서 아래로 진입했는데 2게이트가 위수직일때.
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));

			}else if(gate1.x<gate2.x and gate1.y<gate2.y){
				direction='r';
				snake.insert(snake.begin(), CharPosition(gate1.x+1, gate1.y));
			}else if(gate1.x>gate2.x and gate1.y<gate2.y){
				direction='l';
				snake.insert(snake.begin(), CharPosition(gate1.x-1, gate1.y));
			}
		}
		
	}else{		

	// 이동하는 방향에다가 문자를 넣어야
	if (direction == 'l')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x-1, snake[0].y)); }
	else if (direction == 'r')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x+1, snake[0].y)); }
	else if (direction == 'u')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y-1)); }
	else if (direction == 'd')
	{ snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y+1)); }

	//snake의 head부분색 지정
	attron(COLOR_PAIR(4));
	move(snake[0].y, snake[0].x);
	addch(partchar); // 새로운 머리를 지정
	attroff(COLOR_PAIR(4));
	//head부 색 지정 끝, 뒤이을 몸통들 색 지정
	attron(COLOR_PAIR(3));
	move(snake[1].y, snake[1].x);
	addch(partchar); // 머리와 몸통의 문자가 다르니까 머리가 이동하고 그 이전 자리를 몸통의 문자로 바꿔줘야함
	attroff(COLOR_PAIR(3));
	
	// 이부분은 snake 가 gate를 통과하면 게이트의 색이 사라져버리므로 계속 색을 칠해주는 역할을한다 
	attron(COLOR_PAIR(8));
	move(gate1.y, gate1.x);
	addch((char)33);
	move(gate2.y, gate2.x);
	addch((char)33);
	attroff(COLOR_PAIR(8));
	
	PrintScore();
	refresh();
	return;
	}


}

// SnakeGame newGame >> newGame.PlayGame(); 으로 게임 시작
void SnakeGame::PlayGame()
{

    while(1)
    {
	foodtime+=1;
	poisontime+=1;
	randomtime+=1;
	//머리 충돌시, 역방향입력시 게임 종료 가운데에 출력
        if (FatalCollision())
        {
            move((maxheight-2)/2,(maxwidth-5)/2-5);
            printw("GAME OVER");
            break;
        }

	//먹었냐안먹었냐 bool 판별한 뒤에 MoveSnake()실행
	GetsFruit();
	getPoison();
	getRandom();
	MoveSnake();
	//-----------------------------------------------------------
	if(gate==true){
		gatenum=gatenum+1;
		PrintScore();
		gate=false;
    }
	else
		gate=false;

	mission=mission1+mission2+mission3+mission4;
	//조건 달성시(mission=4일때, 미션이 전부 달성시) 증감, 종료
	if(mission==4){
		map+=1;
		foodtime=0;
		poisontime=0;
		randomtime = 0;
		if(map==4){
			direction='f';
		}else{
			direction='p';
		}
	}

	if(foodtime==100){				//-----------게이트 생성 조건, 일단은 foodtime=100
		
		makeGate();
		makeGate2();
		
		gateadd=false;
	}

	//4스테이지 까지 클리어 했을 시 실행 축하 메시지와 함께 게임 종료
	if(direction=='f'){
		move((maxheight-2)/2, (maxwidth-5)/2-10);
		printw("Congratualtion!!!, YOU ARE WINNER!!");
		refresh();
		usleep(2000000);
		break;
	}
        if (direction=='q') //exit
        {
		move((maxheight-2)/2,(maxwidth-5)/2+2);
		printw("GAME OVER");


        	break;
        }
	//스테이지 미션 클리어시 실행, nextStage()함수로 루프 내에서 반복
	if(direction=='p'){

		move((maxheight-2)/2,(maxwidth-5)/2-2);
		printw("Next Stage");
		refresh();
		//미션 충족시 next stage가 중앙에 뜨고 2초 뒤 자동으로 다음 스테이지
		usleep(2000000);
		clear();
		refresh();

		direction='l';
		nextStage();


	}

	/* 프로그램 속도 조절  usleep메소드는 usleep(int micro_seconds) 따라서 usleep(백만)= 1초 후 다음 작업 실행
	 * 즉, usleep(백만)은 1초의 딜레이를 준다는 것 우리는 0.5초의 틱을 준다면 50만으로 초기설정
	 */

        usleep(del); // delay, while문이 1번하는데 딜레이 >>GetsFruit()과 MoveSnake()가 딜레이마다 실행
    }



}
