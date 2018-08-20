#define DEBUG
//#define DEBUG1

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stack>

using namespace std;

const int INF = 10000;

//const int MaxDepth = 4;

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
void print(int board[5][5]);

int Board[5][5];
bool ourColor, firstColor;
char name[50] = "AI";

typedef struct Pos {
	int x, y;
	Pos(int x=-1, int y=-1) :x(x), y(y) {};
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
			return 8 - positions[1][piece].x - positions[1][piece].y;
		else
			return positions[0][piece].x + positions[0][piece].y;
	}

	int GetDistance(bool player, int x, int y)
	{
		if (player)
			return 8 - x - y;
		else
			return x + y;
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

int cnnt = 0;
bool simulate(situation cur)
{
	cnnt++;
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
#ifdef DEBUG1
	print(cur.board);
	printf("\n");
#endif
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

float Evaluate(situation cur,int piece,int moveNo)
{
	float ans=0;
	bool player = !cur.player;
	int nx=cur.positions[player][piece].x + GetMove(player, moveNo).dx;
	int ny = cur.positions[player][piece].y + GetMove(player, moveNo).dy;

	if (cur.board[nx][ny] != 0)
	{
		if ((cur.board[nx][ny] > 0) ^ player)
		{
			ans += 0.5;//吃掉对面的
			if (cur.GetDistance(cur.player, abs(cur.board[nx][ny])) <= 2)
				ans += 0.5;
		}
		else
			ans -= 0.5;//吃掉自己的
	}

	float risk=0;
	for (int i = 0; i < 3; i++)
	{
		int nnx = nx + GetMove(player, i).dx, nny = GetMove(player, i).dy;
		if (nnx >= 0 && nnx < 5 && nny >= 0 && nny < 5&&cur.board[nnx][nny]!=0&&((cur.board[nnx][nny]>0)^player))
			risk += cur.GetProbability(cur.player, cur.board[nnx][nny]);
	}
	ans -= risk;

	ans += (cur.GetDistance(player, piece)-cur.GetDistance(player,piece));

	if (cur.GetDistance(player, nx, ny) <= 2)
		ans += 1;
	if (cur.GetDistance(player, nx, ny) == 0)
		ans += 1;
	return ans;
}

float MC(situation *cur)
{
	float win=0;
	for (int i = 0; i < 300; i++)
		if (simulate(*cur))
			win++;
	float p = (float)win / 300;
	return p;
}

float AlphaBeta(situation cur, int depth, float alpha, float beta)
{
	float val;
	if (depth == 0)
	{
		float p = MC(&cur);
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
					float eval = Evaluate(cur, piece, moveNo);
					situation next = situation(cur, piece, moveNo);
					val = 0.5*AlphaBeta(next, depth - 1, -beta, -alpha) + (eval+0.5)/4;
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

void print(int board[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (board[i][j] >= 0)
				cout << " " << board[i][j] << " ";
			else
				cout << board[i][j] << " ";
		}
		printf("\n");
	}
}

void test(situation cur)
{
	int start, end;
	start = end = clock();
	int win = 0, cnt = 0;
	while (end - start <= 5000)
	{
		if (simulate(cur))
			win++;
		cnt++;
		end = clock();
	}
	cout << cnt << endl;
	cout << win << endl;
}

stack <class situation> History;

int main()
{
	srand(time(NULL));
	int red[6], blue[6];
	string temp;
	cin >> temp;//输入"name?"
	std::cout << name << endl;//输出名字
	cin >> temp;//输入先手颜色
	if (temp == "redfirst")
		firstColor = true;
	else
		firstColor = false;
	cin >> temp;//输入电脑的颜色
	if (temp == "red")
		ourColor = true;
	else
		ourColor = false;
	std::cout << "123456" << endl;//输出电脑的布局
	cin >> temp;//输入begin
	cin >> temp;//输入对手布局
	for (int i = 0; i < 6; i++)
	{
		red[i] = i + 1;
		blue[i] = i+1;
	}
	InitializeBoard(red, blue);
	situation cur=situation(Board,ourColor);
#ifdef DEBUG1
	test(cur);
#endif
	if (firstColor == ourColor)
		cur.player = !ourColor;
	getchar();
	string cmd;
	while (getline(cin,cmd)&&cmd != "stop")
	{
		if (cmd == "regret")
		{
			History.pop();
			cur = History.top();
			continue;
		}
		stringstream ss(cmd);
		if (cur.player == !ourColor)
		{
			int piece;
			ss >> piece;
			int pieces[2], cnt = 0;
			if (cur.positions[ourColor][piece].x != -1)
			{
				pieces[0] = piece;
				cnt = 1;
			}
			else
			{
				for (int i = piece; i <= 6; i++)
					if (cur.positions[ourColor][i].x != -1)
					{
						pieces[cnt++] = i;
						break;
					}
				for (int i = piece; i >= 0; i--)
					if (cur.positions[ourColor][i].x != -1)
					{
						pieces[cnt++] = i;
						break;
					}
			}
			Move bestMove;
			int bestMoveNo;
			float bestValue = -INF; 
			for (int i = 0; i < cnt; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					int nx = cur.positions[ourColor][pieces[i]].x + GetMove(ourColor, j).dx;
					int ny = cur.positions[ourColor][pieces[i]].y + GetMove(ourColor, j).dy;
					if (nx >= 0 && ny >= 0 && nx < 5 && ny < 5)
					{
						situation next = situation(cur);
						float eval = Evaluate(cur, piece, j);
						move(next, pieces[i], j);
						float val = 0.5*AlphaBeta(next, 5, -5.0f, 5.0f) + (eval + 0.5) / 4;
#ifdef DEBUG
						printf("%d %d %f\n", pieces[i],j ,val);
#endif
						if (val >= bestValue)
						{
							bestMove = GetMove(ourColor, j);
							bestValue = val;
							bestMoveNo = j;
							piece = pieces[i];
						}
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
#ifdef DEBUG
			cout << cnnt << endl;
			print(Board);
#endif
		}
		else
		{
			int old, niu, piece;
			ss >> temp >> old >> niu >> piece;
			move(Board, old, niu);
			cur = situation(Board, !ourColor);
#ifdef DEBUG
			print(Board);
#endif
		}
		History.push(cur);
	}
}