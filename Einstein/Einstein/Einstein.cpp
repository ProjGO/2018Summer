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
	bool player;
	situation *father = NULL, *brother = NULL, *son = NULL;
	int cnt_visit = 0, cnt_win = 0;
private:
	pos positions[2][7];
	int alive_cnt[2];//false À¶ true ºì
};