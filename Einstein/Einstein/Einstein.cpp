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

struct nextMove {
	int piece, move;
	nextMove(int piece, int move) :piece(piece), move(move) {};
};

class situation {
public:
	int board[5][5];
	int red[7][2], blue[7][2];
	bool player;//true ºì false À¶
	situation *father=NULL, *brother=NULL, *son=NULL;
	int cnt_visit=0, cnt_win=0;
	int blue_avaliable = 0, red_avaliable = 0;
	situation(int m[5][5],bool player)
	{
		for(int i=0;i<5;i++)
			for (int j = 0; j < 5; j++)
			{
				board[i][j] = m[i][j];
				if (m[i][j] > 0)
				{
					red[m[i][j]][0] = i;
					red[m[i][j]][1] = j;
					red_avaliable++;
				}
				if (m[i][j] < 0)
				{
					blue[-m[i][j]][0] = i;
					blue[-m[i][j]][1] = j;
					blue_avaliable++;
				}
			}
		this->player = player;
	}
	situation(const situation& old, int piece, int move)
	{
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 5; j++)
				board[i][j] = old.board[i][j];
		for (int i = 1; i <= 6; i++)
		{
			red[i][0] = old.red[i][0];  red[i][1] = old.red[i][1];
			blue[i][0] = old.blue[i][0];  blue[i][1] = old.blue[i][1];
		}
		red_avaliable = old.red_avaliable;
		blue_avaliable = old.blue_avaliable;
		player = !old.player;
		int nx, ny;
		if (player)
		{
			board[red[piece][0]][red[piece][1]] = 0;
			nx = red[piece][0] + redMove[move][0];
			ny = red[piece][1] + redMove[move][1];
			red[piece][0] = nx;  red[piece][1] = ny;
		}
		else
		{
			board[blue[piece][0]][blue[piece][1]] = 0;
			nx = blue[piece][0] + blueMove[move][0];
			ny = blue[piece][0] + blueMove[move][1];
			blue[piece][0] = nx;  blue[piece][1] = ny;
		}
		if (board[nx][ny] > 0)
		{
			red[board[nx][ny]][0] = red[board[nx][ny]][1] = -1;
			red_avaliable--;
		}
		else if (board[nx][ny] < 0)
		{
			blue[board[nx][ny]][0] = blue[board[nx][ny]][1] = -1;
			blue_avaliable--;
		}
		board[nx][ny] = piece;
	}
};

bool simulate(situation cur)
{
	int piece=
}
int main()
{
	srand((unsigned int)time(NULL));
}
