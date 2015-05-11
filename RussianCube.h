#pragma once

#include <math.h>
#include <iostream>
using namespace std;
#define random(x) (rand()%x)
#define INFTY 5000;

extern int flag_moveDown,flag_stop,flag_left_stop,flag_right_stop,flag_right_stop,flag_stop_leftdown,flag_stop_rightdown;
extern int temp_I;
extern int temp_J;
extern int flag_space,flag_down_floor,flag_spe1,flag_time,flag_pause,flag_start,flag_pause;
extern int temp_e[4][4];
extern int nrows, ncols, dw;

/*在该文件中声明类和类的函数*/

class MyMatrix4
{
public:
	MyMatrix4() //构造函数，用于完成创建对象时完成对对象属性的一些初始化等操作
	{
		set(0);
	}
	friend class Game;
	void set(int value)
	{
		e[0][0] = e[0][1] = e[0][2] = e[0][3] = 
		e[1][0] = e[1][1] = e[1][2] = e[1][3] = 
		e[2][0] = e[2][1] = e[2][2] = e[2][3] = 
		e[3][0] = e[3][1] = e[3][2] = e[3][3] = value;
	}
	int get(int i, int j)
	{
		return e[i][j];
	}
	void setColumn(int col, int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[0][col+1] = e[1][col+1] = e[2][col+1] = e[3][col+1] = value;
		trans_val();
	}
	void setRow(int row, int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[row][0] = e[row][1] = e[row][2] = e[row][3] = value;
		trans_val();
	}
	void setZ_left(int col,int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[2][col] = e[1][col] = e[1][col+1] = e[0][col+1] = value;
		trans_val();
	}
	void setZ_right(int col,int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[2][col+1] = e[1][col+1] = e[1][col] = e[0][col] = value;
		trans_val();
	}
	void setL_left(int col,int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[0][col] = e[0][col+1] = e[1][col+1] = e[2][col+1] = value;
		trans_val();
	}
	void setL_right(int col,int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[0][col+1] = e[0][col] = e[1][col] = e[2][col] = value;
		trans_val();
	}
	void setTian(int col,int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[0][col] = e[0][col+1] = e[1][col] = e[1][col+1] = value;
		trans_val();
	}
	void setTu(int col,int value)
	{
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				e[i][j]=0;
			}
		}
		e[2][col] = e[1][col+1] = e[1][col] = e[1][col-1] = value;
		trans_val();
	}
	
	void rotate90()
	{
		int n = 4;
		int f = floor((float)n/2);
		int c = ceil((float)n/2);

		for (int x = 0; x < f; ++x)
		  for (int y = 0; y < c; ++y)
		  {
			int temp = e[x][y];
			e[x][y] = e[y][n-1-x];
			e[y][n-1-x] = e[n-1-x][n-1-y];
			e[n-1-x][n-1-y] = e[n-1-y][x];
			e[n-1-y][x] = temp;
		  }


		for (int i=0;i<4;++i)             //输出label矩阵
		{
			cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
			for (int j=0;j<4;++j)
			{
				cout<<e[i][j]<<' ';
			}
			cout<<endl;
		}
	
	}

	void trans_val()
	{
		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				temp_e[i][j] = e[i][j];
			}
	}


private:
	int e[4][4];
};

class CubeElement
{
public:
	int type;
	CubeElement(int i_, int j_, int type_) : i(i_),j(j_),type(type_){ //使用初始化表
		switch(type){
			case 0:                  //长条状的俄罗斯方块；
				{
					mat4.setColumn(1,1);
				}
				break;
			case 1:					//左z状的俄罗斯方块；
				{
					mat4.setZ_left(1,1);
				}
				break;
			case 2:					//右z状的俄罗斯方块；
				{
					mat4.setZ_right(1,1);
				}
				break;
			case 3:					//左L状的俄罗斯方块；
				{
					mat4.setL_left(1,1);
				}
				break;
			case 4:					//右L状的俄罗斯方块；
				{
					mat4.setL_right(1,1);
				}
				break;
			case 5:					//田状的俄罗斯方块；
				{
					mat4.setTian(1,1);
				}
				break;
			case 6:					//土状的俄罗斯方块；
				{
					mat4.setTu(1,1);
				}
				break;
				// ... more
		}
	}

	void crt_type(int type)
	{
		switch(type){
			case 0:                  //长条状的俄罗斯方块；
				{
					mat4.setColumn(1,1);
				}
				break;
			case 1:					//左z状的俄罗斯方块；
				{
					mat4.setZ_left(1,1);
				}
				break;
			case 2:					//右z状的俄罗斯方块；
				{
					mat4.setZ_right(1,1);
				}
				break;
			case 3:					//左L状的俄罗斯方块；
				{
					mat4.setL_left(1,1);
				}
				break;
			case 4:					//右L状的俄罗斯方块；
				{
					mat4.setL_right(1,1);
				}
				break;
			case 5:					//田状的俄罗斯方块；
				{
					mat4.setTian(1,1);
				}
				break;
			case 6:					//土状的俄罗斯方块；
				{
					mat4.setTu(1,1);
				}
				break;
				// ... more
		}
	}

	void moveDown()
	{
		if(i!=0 && flag_moveDown==0)
			this->i--;
		else if(i==0 && flag_moveDown==0)
		{
			this->i--;

			//i = 14;
		}
		if(i==-2)
		{
			i = -1;
			flag_spe1 = 1;
		}
/*		if(i==1)
		{
			flag_moveDown = 1;
			for (int i_tmp = 0; i_tmp < 4; ++i_tmp)
			{
				int I = i + i_tmp;
				if (I >= 15) continue;
				for (int j_tmp = 0; j_tmp < 4; ++j_tmp)
				{
					int J = j + j_tmp;
					if (J >= 10) continue;
					if (abs(mat4.get(i,j)) > 0.5f)
					{
						label[I][J] = 1;                            //出现绿色方块显示的label设置；
					}
				}
			}
		}
*/
	}
	void moveUp()
	{

	}
	void moveLeft()
	{
		this->j--;
	}
	void moveRight()
	{
		this->j++;
	}
	void setI(int i_)
	{
		i = i_;
	}
	void setJ(int j_)
	{
		j = j_;
	}
	int getI()
	{
		return i;
	}
	int getJ()
	{
		return j;
	}
	MyMatrix4& getMat()
	{
		return mat4;
	}
private:
	int i, j; // row and col index
	MyMatrix4 mat4;
};

class CubeGrid
{
public:
	CubeGrid(float w_, float h_, int nrows_, int ncols_);
	~CubeGrid();

	void setMovingLabel(CubeElement* e);		// set the tags of current occupied valid grids to 1
	void resetMovingLabel(CubeElement* e);		// reset the tags of current occupied grids to 0
	void find_left(CubeElement* e);
	void find_right(CubeElement* e);
	void find_down(CubeElement* e);
	void drawGrid();
	void draw_squaGrid();

private:
	void drawGridQuad(int i, int j);

private:
	float drow, dcol;	 // element width and height 
	int nrows, ncols;
	int** label;
};

class Game
{
public:
	Game()
	{

	}
	~Game()
	{
		delete grid; grid = 0;
		delete cube; cube = 0;
	}

	void init(float w_, float h_, int nrows_, int ncols_);
	void rotate();
	void start();
	void step();
	void left();
	void right();
	void pause();
	void end();
	void speedup();

	void draw();

private:
	CubeGrid* grid;
	CubeElement* cube;
	int nrows, ncols;
};

