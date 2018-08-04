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

typedef struct pos {
	int x, y;
	pos(int x=0, int y=0) :x(x), y(y) {};
	bool operator == (const pos &rhs) { return x == rhs.x&&y == rhs.y; }
} pos;

class situation {
public:
	int board[5][5];
	pos positions[2][7];
	int alive_cnt[2];//false À¶ true ºì
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
					positions[1][m[i][j]] = pos(i, j);
					alive_cnt[1]++;
				}
				if (m[i][j] < 0)
				{
					positions[0][-m[i][j]] = pos(i, j);
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
	}
	situation(const situation& old, int piece, int moveNo)
	{
		*this = situation(old);
		move(*this, piece, moveNo);
	}

};

void move(situation &cur, int piece, int moveNo)
{
	cur.player = !cur.player;
	int nx, ny;

	cur.board[cur.positions[cur.player][piece].x][cur.positions[cur.player][piece].y] = 0;

}