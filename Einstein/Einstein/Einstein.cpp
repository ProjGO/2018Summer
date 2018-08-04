#include <cstdio>
#include <cstdlib>
#include <ctime>

using namespace std;

const int redMove[3][2] = {
	0,1,
	1,1,
	1,0
};
const int blueMove[3][2] = {
	-1,0,
	-1,-1,
	0,-1
};

bool ourColor = false;
int Board[5][5];
char name[50] = "AI";

class situation;
struct Move;
void move(situation &cur, int piece, int move);

struct Move {
	int piece, move;
	Move(int piece = 0, int move = 0) :piece(piece), move(move) {};
};

class situation {
public:
	int board[5][5];
	int red[7][2], blue[7][2];
	bool player;//true 红 false 蓝
	situation *father = NULL, *brother = NULL, *son = NULL;
	int cnt_visit = 0, cnt_win = 0;
	int blue_alive = 0, red_alive = 0;
	situation(int m[5][5], bool player)
	{
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
			{
				board[i][j] = m[i][j];
				if (m[i][j] > 0)
				{
					red[m[i][j]][0] = i;
					red[m[i][j]][1] = j;
					red_alive++;
				}
				if (m[i][j] < 0)
				{
					blue[-m[i][j]][0] = i;
					blue[-m[i][j]][1] = j;
					blue_alive++;
				}
			}
		this->player = player;
	}
	situation(const situation &old)
	{
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				board[i][j] = old.board[i][j];
		for (int i = 1; i <= 6; i++)
		{
			red[i][0] = old.red[i][0];  red[i][1] = old.red[i][1];
			blue[i][0] = old.blue[i][0];  blue[i][1] = old.blue[i][1];
		}
		red_alive = old.red_alive;
		blue_alive = old.blue_alive;
		player = old.player;
		brother = old.brother;
		father = old.father;
		son = old.son;
		cnt_win = old.cnt_win;
		cnt_visit = old.cnt_visit;
	}
	situation(const situation& old, int piece, int moveNo)
	{
		*this = situation(old);
		move(*this, piece, moveNo);
	}

	float GetProbability(bool player, int piece)
	{

	}
};

void move(situation &cur, int piece, int moveNo)
{
	cur.player = !cur.player;
	int nx, ny;
	if (cur.player)
	{
		cur.board[cur.red[piece][0]][cur.red[piece][1]] = 0;
		nx = cur.red[piece][0] + redMove[moveNo][0];
		ny = cur.red[piece][1] + redMove[moveNo][1];
		cur.red[piece][0] = nx;  cur.red[piece][1] = ny;
	}
	else
	{
		cur.board[cur.blue[piece][0]][cur.blue[piece][1]] = 0;
		nx = cur.blue[piece][0] + blueMove[moveNo][0];
		ny = cur.blue[piece][1] + blueMove[moveNo][1];
		cur.blue[piece][0] = nx;  cur.blue[piece][1] = ny;
	}
	if (cur.board[nx][ny] > 0)
	{
		cur.red[cur.board[nx][ny]][0] = -1;
		cur.red[cur.board[nx][ny]][1] = -1;
		cur.red_alive--;
	}
	else if (cur.board[nx][ny] < 0)
	{
		cur.blue[-cur.board[nx][ny]][0] = -1;
		cur.blue[-cur.board[nx][ny]][1] = -1;
		cur.blue_alive--;
	}
	if (cur.player)
		cur.board[nx][ny] = piece;
	else
		cur.board[nx][ny] = -piece;
}

bool simulate(situation cur)
{
	while (cur.board[0][0]>=0 && cur.board[4][4]<=0 && cur.red_alive&&cur.blue_alive)
	{
		if (cur.player)//如果这一步是红棋下的，接下来这步该蓝棋下
		{
			int t = rand() % cur.blue_alive + 1;
			int piece;
			for (piece = 1; piece <= 6; piece++)
			{
				if (cur.blue[piece][0] != -1 && cur.blue[piece][1] != -1)
					t--;
				if (!t)
					break;
			}
			int moveNo;
			if (cur.blue[piece][0] == 0)
				moveNo = 2;
			else if (cur.blue[piece][1] == 0)
				moveNo = 0;
			else
				moveNo = rand() % 3;
			move(cur, piece, moveNo);
		}
		else
		{
			int t = rand() % cur.red_alive + 1;
			int piece;
			for (piece = 1; piece <= 6; piece++)
			{
				if (cur.red[piece][0] != -1&&cur.red[piece][1]!=-1)
					t--;
				if (!t)
					break;
			}
			int moveNo;
			if (cur.red[piece][0] == 4)
				moveNo = 0;
			else if (cur.red[piece][1] == 4)
				moveNo = 2;
			else
				moveNo = rand() % 3;
			move(cur, piece, moveNo);
		}
	}
	bool result;
	if (cur.board[4][4] > 0 || cur.blue_alive == 0)
		result = true;
	if (cur.board[0][0] < 0 || cur.red_alive == 0)
		result = false;
	if (ourColor)
		return result;
	else
		return !result;
}

void InitializeBoard(int red[6],int blue[6])
{
	Board[0][0] = red[0];
	Board[0][1] = red[1];
	Board[0][2] = red[2];
	Board[1][0] = red[3];
	Board[1][1] = red[4];
	Board[2][0] = red[5];

	Board[4][4] = blue[0];
	Board[4][3] = blue[1];
	Board[4][2] = blue[2];
	Board[3][4] = blue[3];
	Board[3][3] = blue[4];
	Board[2][4] = blue[5];
}


int main()
{
	int red[6], blue[6];
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 6; i++)
	{
		red[i] = i + 1;
		blue[i] = -i - 1;
	}
	InitializeBoard(red, blue);
	situation test(Board, false);
	int win = 0, lose = 0;
	int starttime, endtime;
	starttime = endtime = clock();
	int cnt = 0;
	while (endtime-starttime<=1000)
	{
		if (simulate(test))
			win++;
		else
			lose++;
		endtime = clock();
		cnt++;
	}
	printf("%d %d\n", win, lose);
	printf("%d\n", cnt);
	getchar();
	getchar();
}
