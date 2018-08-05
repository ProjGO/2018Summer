#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

const int INF = 10000;

const int MaxDepth = 4;

const int MIN_SIM = 5;
const int MAX_SIM = 100;
const float MAX_UCB = 10000;
const float UCB_K = 1;
const int MAX_TIME = 100;

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

struct Pos;
struct Move;
class situation;
void move(situation &cur, int piece, int moveNo);
void move(int board[5][5], int dx, int dy);
Move GetMove(bool player, int moveNo);

int Board[5][5];
bool ourColor, firstColor;
char name[50] = "AI";

typedef struct Pos {
	int x, y;
	Pos(int x=0, int y=0) :x(x), y(y) {};
	bool operator == (const Pos &rhs) { return x == rhs.x&&y == rhs.y; }
} Pos;

typedef struct Move {
	int dx, dy;
	Move(int dx=0, int dy=0) :dx(dx), dy(dy) {};
}Move;

class situation {
public:
	int board[5][5];
	Pos positions[2][7];
	int alive_cnt[2] = { 0,0 };//false 蓝 true 红
	bool player;
	situation *father = NULL, *brother = NULL, *son = NULL;
	int cnt_visit = 0, cnt_win = 0;
	situation(int m[5][5], bool player)
	{
		for(int i=0;i<5;i++)
			for (int j = 0; j < 5; j++)
			{
				board[i][j] = m[i][j];
				if (m[i][j] > 0)
				{
					positions[1][m[i][j]] = Pos(i, j);
					alive_cnt[1]++;
				}
				if (m[i][j] < 0)
				{
					positions[0][-m[i][j]] = Pos(i, j);
					alive_cnt[0]++;
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
			positions[0][i] = old.positions[0][i];
			positions[1][i] = old.positions[1][i];
		}
		alive_cnt[0] = old.alive_cnt[0];
		alive_cnt[1] = old.alive_cnt[1];
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
		float ans = 1 / 6;
		for (int i = piece - 1; i > 0; i--)
			if (positions[player][piece].x == -1)
				ans += 1 / 12;
			else
				break;
		for (int i = piece + 1; piece <= 6; piece++)
			if (positions[player][piece].x == -1)
				ans += 1 / 12;
			else
				break;
		return ans;
	}

	int GetDistance(bool player, int piece)
	{
		if (player)
			return 4 - positions[1][piece].x - positions[1][piece].y;
		else
			return positions[0][piece].x + positions[0][piece].y;
	}

	Pos GetPosition(bool player, int piece)
	{
		return positions[player][piece];
	}
};

Move GetMove(bool player, int moveNo)
{
	if (player)
		return Move(redMove[moveNo][0], redMove[moveNo][1]);
	else
		return Move(blueMove[moveNo][0], blueMove[moveNo][1]);
}

void move(situation &cur, int piece, int moveNo)
{
	cur.player = !cur.player;
	int nx, ny;

	cur.board[cur.positions[cur.player][piece].x][cur.positions[cur.player][piece].y] = 0;
	nx = cur.positions[cur.player][piece].x + GetMove(cur.player, moveNo).dx;
	ny = cur.positions[cur.player][piece].y + GetMove(cur.player, moveNo).dy;
	cur.positions[cur.player][piece] = Pos(nx, ny);

	if (cur.board[nx][ny] < 0)
	{
		cur.positions[0][-cur.board[nx][ny]] = Pos(-1, -1);
		cur.alive_cnt[0]--;
	}
	else if (cur.board[nx][ny] > 0)
	{
		cur.positions[1][cur.board[nx][ny]] = Pos(-1, -1);
		cur.alive_cnt[1]--;
	}

	if (cur.player)
		cur.board[nx][ny] = piece;
	else
		cur.board[nx][ny] = -piece;
}

void move(int board[5][5], int dx, int dy)
{
	int x = dx / 10, y = dx % 10;
	int nx = dy / 10, ny = dy % 10;
	int &old = board[x][y], &niu = board[nx][ny];
	niu = old;
	old = 0;
}

bool simulate(situation cur)
{
	while (cur.board[0][0] >= 0 && cur.board[4][4] <= 0 && cur.alive_cnt[0] && cur.alive_cnt[1])
	{
		int t = rand() % cur.alive_cnt[!cur.player] + 1;
		int piece;
		for (piece = 1; piece <= 6; piece++)
		{
			if (cur.positions[!cur.player][piece].x != -1)
				t--;
			if (!t)
				break;
		}
		int moveNo;
		if (!cur.player)
		{
			if (cur.positions[1][piece].x == 4)
				moveNo = 0;
			else if (cur.positions[1][piece].y == 4)
				moveNo = 2;
			else
				moveNo = rand() % 3;
		}
		else
		{
			if (cur.positions[0][piece].x == 0)
				moveNo = 2;
			else if (cur.positions[0][piece].y == 0)
				moveNo = 0;
			else
				moveNo = rand() % 3;
		}
		move(cur, piece, moveNo);
	}
	bool result;
	if (cur.board[4][4] > 0 || cur.alive_cnt[false] == 0)
		result = true;
	if (cur.board[0][0] < 0 || cur.alive_cnt[true] == 0)
		result = false;
	if (ourColor)
		return result;
	else
		return !result;
}

float Evaluate(situation cur)
{
	return 0;
}

float UCT(situation *cur)
{
	float win=0;
	for (int i = 0; i < 100; i++)
		if (simulate(*cur))
			win++;
	float p = (float)win / 100;
	return p;
}

float AlphaBeta(situation cur, int depth, float alpha, float beta)
{
	float val;
	if (depth == 0)
	{
		float p = UCT(&cur);
		return p;
	}
	bool nextPlayer = !cur.player;
	for (int piece = 1; piece <= 6; piece++)
	{
		if (cur.positions[nextPlayer][piece].x != -1)
		{
			for (int moveNo = 0; moveNo < 3; moveNo++)
			{
				int nx = cur.positions[nextPlayer][piece].x + GetMove(nextPlayer, moveNo).dx;
				int ny = cur.positions[nextPlayer][piece].y + GetMove(nextPlayer, moveNo).dy;
				if (nx >= 0 && ny >= 0 && nx < 5 && ny < 5)
				{
					situation next = situation(cur, piece, moveNo);
					val = AlphaBeta(next, depth - 1, -beta, -alpha) + Evaluate(cur);
					if (val >= beta)
						return beta;
					if (val > alpha)
						alpha = val;
				}
			}
		}
	}
	return alpha;
}

void InitializeBoard(int red[6], int blue[6])
{
	Board[0][0] = red[0];
	Board[0][1] = red[1];
	Board[0][2] = red[2];
	Board[1][0] = red[3];
	Board[1][1] = red[4];
	Board[2][0] = red[5];

	Board[4][4] = -blue[0];
	Board[4][3] = -blue[1];
	Board[4][2] = -blue[2];
	Board[3][4] = -blue[3];
	Board[3][3] = -blue[4];
	Board[2][4] = -blue[5];
}

void print()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (Board[i][j] >= 0)
				cout << " " << Board[i][j] << " ";
			else
				cout << Board[i][j] << " ";
		}
		printf("\n");
	}
}

int main()
{
	srand(time(NULL));
	int red[6], blue[6];
	string temp;
	cin >> temp;
	std::cout << name << endl;//name
	cin >> temp;//先手
	if (temp == "redfirst")
		firstColor = true;
	else
		firstColor = false;
	cin >> temp;//电脑的颜色
	if (temp == "red")
		ourColor = true;
	else
		ourColor = false;
	std::cout << "123456" << endl;
	cin >> temp;//输入begin
	cin >> temp;//输入对手布局
	for (int i = 0; i < 6; i++)
	{
		red[i] = i + 1;
		blue[i] = i+1;
	}
	InitializeBoard(red, blue);
	situation cur=situation(Board,ourColor);
	if (firstColor == ourColor)
		cur.player = !ourColor;
	getchar();
	string cmd;
	while (getline(cin,cmd)&&cmd != "stop")
	{
		//print();
		stringstream ss(cmd);
		if (cur.player == !ourColor)
		{
			int piece;
			ss >> piece;
			Move bestMove;
			int bestMoveNo;
			float bestValue = -INF;
			for (int i = 0; i < 3; i++)
			{
				int nx = cur.positions[ourColor][piece].x + GetMove(ourColor, i).dx;
				int ny = cur.positions[ourColor][piece].y + GetMove(ourColor, i).dy;
				if (nx >= 0 && ny >= 0 && nx < 5 && ny < 5)
				{
					situation next = situation(cur);
					move(next, piece, i);
					float val = AlphaBeta(next, 5, -1.0f, 1.0f);
					if (val >= bestValue)
					{
						bestMove = GetMove(ourColor, i);
						bestValue = val;
						bestMoveNo = i;
					}
				}
			}
			int x = cur.positions[ourColor][piece].x;
			int y = cur.positions[ourColor][piece].y;
			int nx = x + bestMove.dx;
			int ny = y + bestMove.dy;
			move(cur, piece, bestMoveNo);
			Board[nx][ny] = Board[x][y];
			Board[x][y] = 0;
			cout << "move " << x<<y << " " << nx<<ny << " " << piece << endl;
			//print();
		}
		else
		{
			int old, niu, piece;
			ss >> temp >> old >> niu >> piece;
			move(Board, old, niu);
			cur = situation(Board, !ourColor);
		}
	}
}