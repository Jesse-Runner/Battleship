#include <vector>
using namespace std;

void draw_top_matrix(vector<vector<int> > &board,int cur_row, int cur_col, int offset);
void draw_bot_matrix(vector<vector<int> > &board,int cur_row, int cur_col, int offset, int hit);
vector<vector<int> > move_StartPos(vector<vector<int> > &board, int &rightpress, int &cur_row, int &cur_col);
vector<vector<int> > first_Move(vector<vector<int>> &board, int &cur_row, int &cur_col);
vector<vector<int>> make_Move(vector<vector<int>> board, vector<vector<int>> oppboard, bool firstmove, int hit);
