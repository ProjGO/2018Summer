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