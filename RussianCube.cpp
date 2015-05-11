#include "RussianCube.h"
#include <GL/glut.h>
#include <iostream>
using namespace std;  
#define random(x) (rand()%x)
#define INFTY 5000;

int flag_moveDown=0,flag_stop=0,flag_left_stop=0,flag_right_stop=0,flag_stop_leftdown=0,flag_stop_rightdown=0;
int temp_I;
int temp_J;
int time_cont=300;
int flag_space=0,flag_down_floor=0,flag_spe1=0,flag_time=time_cont,flag_pause=0,flag_start=0;
int temp_e[4][4];
int nrows = 15, ncols = 10, dw = 60;

void print_demo(int pointer)
{
	cout<<pointer<<endl;
}

void show_label(int** label)
{
	for (int i=0;i<4;++i)             //输出label矩阵
	{
		cout<<"the temp_e label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<4;++j)
		{
			cout<<temp_e[i][j]<<' ';
		}
		cout<<endl;
	}

	for (int i_tmp=0;i_tmp<15;++i_tmp)             //输出label矩阵
	{
		cout<<"the setting label matrix of"<<' '<<i_tmp<<" rows*************"<<endl;
		for (int j_tmp=0;j_tmp<10;++j_tmp)
		{
			cout<<label[i_tmp][j_tmp]<<' ';
		}
		cout<<endl;
	}
}

void find_overlap(int** label,CubeElement* e)  //找出label[15][10]里面和temp_e[4][4]重叠的部分，并且清零
{
	if(e->getI()>=0)
	{
		for(int i=0;i<4;++i)
		{
			int I = e->getI() + i;
			if (I >= 15) continue;
			for (int j = 0; j < 4; ++j)
			{
				int J = e->getJ() + j;
				if (J >= 10) continue;



				if(I>=0)
				{
					if(label[I][J]!=temp_e[i][j])
					{
						label[I][J] = 1;
					}
					else 
						label[I][J] = 0;
				}

			}
		}
	}

	if(e->getI()==-1)
	{
		for(int i=0;i<4;++i)
		{
			int I = e->getI() + i;
			if (I >= 15) continue;
			for (int j = 0; j < 4; ++j)
			{
				int J = e->getJ() + j;
				if (J >= 10) continue;

					if(label[I+1][J]!=temp_e[i][j])
					{
						label[I+1][J] = 1;
					}
					else 
						label[I+1][J] = 0;

			}
		}
	}

}

CubeGrid::CubeGrid(float w_, float h_, int nrows_, int ncols_) : 
	  nrows(nrows_), ncols(ncols_), drow(h_/nrows_), dcol(w_/ncols_) 
{
	label = new int*[nrows]; //数组指针
	for (int i = 0; i < nrows; ++i)
	{
		label[i] = new int[ncols];
		for (int j = 0; j < ncols; ++j)
		label[i][j] = 0;  //15*10的小方格全部置为黑色
	}
}

CubeGrid::~CubeGrid()
{
	if (this->label != 0)
	{
		for(int i = 0; i < nrows; ++i)
		{
			delete[] label[i];
		}
		delete[] label;
	}
}



void CubeGrid::setMovingLabel(CubeElement* e)  //用于将e[4][4]映射到label[15][10]上面去
{
	for (int i = 0; i < 4; ++i)
	{
		int I = e->getI() + i;
		if (I >= nrows) continue;
		for (int j = 0; j < 4; ++j)
		{
			int J = e->getJ() + j;
			if (J >= ncols) continue;
			if(I>=0)
			{
				if (abs(e->getMat().get(i,j)) > 0.5f)		//第一个映射函数----------------------------
				{
					label[I][J] = 1;                            //出现绿色方块显示的label设置；
				}
			}	//I常常为12，J为6.		
		}
	}

	//找出4*4矩阵中最靠下方的方格是否有值，从而判断是否悬停
	for (int j = 0; j < 4; ++j)
	{
		int J = e->getJ() + j;
		if (J >= ncols) continue;
		for (int i = 0; i < 4; ++i)
		{
			int I = e->getI() + i;
			if (I >= nrows) continue;
			if(e->getMat().get(i,j)==1 && e->getI()!=0)
			{
				if(I>=1)
				{
					if(label[I-1][J]==1)
					{
						flag_moveDown = 1;
						flag_stop = 1;
						e->setI(nrows-1);
						e->setJ(ncols/2);
						int rand_type = random(7);
						e->crt_type(rand_type);
						flag_time =time_cont;
					}
				}
				break;
			} 
		}
	}


	//若方块到达底部，则将前面部分I置为原值重新掉下方块
	find_down(e);
	if (flag_down_floor == 1)
	{
		flag_down_floor = 0;
		for (int i_tmp = 0; i_tmp < 4; ++i_tmp)
		{
			int I = e->getI() + i_tmp;
			if (I >= 15) continue;
			for (int j_tmp = 0; j_tmp < 4; ++j_tmp)
			{
				int J = e->getJ() + j_tmp;
				if (J >= 10) continue;
				if(I>=0)
				{
					if (abs(e->getMat().get(i_tmp,j_tmp)) > 0.5f)
					{
						label[I][J] = 1;                            //出现绿色方块显示的label设置；
					}
				}
			}
		}

		//产生随机stype的随机方块，并且行数置为15-1=14.
		flag_stop = 1;
		e->setI(nrows-1);
		e->setJ(ncols/2);
		int rand_type = random(7);
		e->crt_type(rand_type);
		flag_time =time_cont;
	}
		
	//判断方块的左边是否有阻挡，并且进行相应的操作。
	find_left(e);
	//判断方块的右边是否有阻挡，并且进行相应的操作。
	find_right(e);

}

void CubeGrid::resetMovingLabel(CubeElement* e)  //重置e[4][4]里面的label值
{
	temp_I = e->getI();
	temp_J = e->getJ();

	if(flag_space==2)
	{
		flag_space = 0;
		for(int i=0;i<4;++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				temp_e[i][j] = 0;
				temp_e[i][j] = e->getMat().get(i,j);
			}
		}
	}
	if(flag_space==1)
		++flag_space;
	
	for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}

	
	for (int i=0;i<4;++i)             //输出label矩阵
	{
		cout<<"the reset e matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<4;++j)
		{
			cout<<temp_e[i][j]<<' ';
		}
		cout<<endl;
	}
/*	for (int i = 0; i < 4; ++i)
	{
		int I = e->getI() + i;
		if (I >= nrows) continue;
		for (int j = 0; j < 4; ++j)
		{
				int J = e->getJ() + j;
				if(j==0 && label[I][J]==1)
				{
					label[I][J] = 1;
				}
				else if(j==3 && label[I][J]==1 && e->type==5) //**************************
				{
					label[I][J] = 1;
				}
				else
				{
					if (J >= ncols) continue;
					label[I][J] = 0;                    //清零的方式需要改变*******************	
					//e->getI()
				}
		}
	}*/

	find_overlap(label,e);
	
//	label[temp_I][temp_J] = 1;

	for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}
	for (int i=0;i<4;++i)             //输出label矩阵
	{
		cout<<"the reset e matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<4;++j)
		{
			cout<<temp_e[i][j]<<' ';
		}
		cout<<endl;
	}
}

void CubeGrid::drawGrid()   //将label[15][10]画出来
{
	glColor4f(0.5, 0.7, 0.5, 0.75);//设置当前颜色
	glBegin (GL_QUADS);  //指定画什么内容
	for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}

	//判断是否需要消行
	int temp_max = 0;
	int flag_era = 0;
	if(flag_stop==1)
	{
		flag_stop = 0;
		for (int i = 0; i < nrows; ++i)  
		{
			int temp_sum=0;
			for (int j = 0; j < ncols; ++j)
			{
				temp_sum = temp_sum + label[i][j];
				if(temp_sum==ncols)
				{
					for(int z = 0; z < ncols; ++z)
					{
						label[i][z] = 0;
						flag_era = 1;
					}
				}
				if(temp_sum > 0)
				{
					temp_max = i;
				}
			}
		}
		for (int i=0;i<15;++i)             //输出label矩阵
		{
			cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
			for (int j=0;j<10;++j)
			{
				cout<<label[i][j]<<' ';
			}
			cout<<endl;
		}
		if(flag_era==1)
		{
			flag_era = 0;
			for (int i = 0; i < ncols; ++i)  //表示列；
			{
				for (int j = 0; j <= temp_max; ++j) //表示行；
				{
					if(label[j][i] == 0)
					{
						for(int z = j+1; z <= temp_max; ++z) //表示行；
						{
							if(label[z][i] == 1)
							{
								label[j][i] = 1;
								label[z][i] = 0;
								break;
							}
						}
					}
				}
			}
		}
	}

	for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}

	for (int i = 0; i < nrows; ++i)
	{
		for (int j = 0; j < ncols; ++j)
		{
			if (label[i][j] == 1)   //如果label[i][j] == 1，则对相应区域的小格子涂颜色
				drawGridQuad(i,j);  //第二个映射函数----------------------------------------------
		}
	}
	for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}


	
		
	glEnd();

	glColor4f(1.0, 1.0, 0.0, 0.3); //设置当前颜色
	glLineWidth(2);
	glBegin(GL_LINES);   //画俄罗斯方块的网格
		for(int i=0;i<15;++i)
		{
			for(int j=0;j<10;++j)
			{
				float x1_2 = j*dcol, x2_2 = (j+1)*dcol;
				float y1_2 = i*drow, y2_2 = (i+1)*drow;
				glVertex2f(x1_2,y1_2);
				glVertex2f(x2_2,y1_2);
				glVertex2f(x2_2,y1_2);
				glVertex2f(x2_2,y2_2);
				glVertex2f(x2_2,y2_2);
				glVertex2f(x1_2,y2_2);
				glVertex2f(x1_2,y2_2);
				glVertex2f(x1_2,y1_2);
			}
		}
	glEnd();

	glColor4f(1.0, 1.0, 0.0, 0.75); //设置当前颜色
	glLineWidth(3);
	glBegin(GL_LINES);   //画俄罗斯方块的网格
		float x1_1 = 11*dcol, x2_1 = 15*dcol;
		float y1_1 = 11*drow, y2_1 = 15*drow;
				glVertex2f(x1_1,y1_1);
				glVertex2f(x2_1,y1_1);
				glVertex2f(x2_1,y1_1);
				glVertex2f(x2_1,y2_1);
				glVertex2f(x2_1,y2_1);
				glVertex2f(x1_1,y2_1);
				glVertex2f(x1_1,y2_1);
				glVertex2f(x1_1,y1_1);
				
	glEnd();

}

void CubeGrid::draw_squaGrid()
{
	glColor4f(0.5, 0.7, 0.5, 0.75);//设置当前颜色
	glBegin (GL_QUADS);  //指定画什么内容
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (temp_e[i][j] == 1)   //如果label[i][j] == 1，则对相应区域的小格子涂颜色
				drawGridQuad(11+i,11+j);  //第二个映射函数----------------------------------------------
		}
	}
	glEnd();
	glColor4f(1.0, 1.0, 0.0, 0.75); //设置当前颜色
	glLineWidth(3);
	glBegin(GL_LINES);   //画俄罗斯方块的网格
		for(int i=11;i<=14;++i)
		{
			for(int j=11;j<=15;++j)
			{
				float x1_2 = j*dcol, x2_2 = (j+1)*dcol;
				float y1_2 = i*drow, y2_2 = (i+1)*drow;
				glVertex2f(x1_2,y1_2);
				glVertex2f(x2_2,y1_2);
				glVertex2f(x2_2,y1_2);
				glVertex2f(x2_2,y2_2);
				glVertex2f(x2_2,y2_2);
				glVertex2f(x1_2,y2_2);
				glVertex2f(x1_2,y2_2);
				glVertex2f(x1_2,y1_2);
			}
		}
				
	glEnd();
}

void CubeGrid::drawGridQuad(int i, int j)
{
	float x1 = j*dcol, x2 = (j+1)*dcol;
	float y1 = i*drow, y2 = (i+1)*drow;
	glVertex3f(x1,y1,0.0);
	glVertex3f(x2,y1,0.0);
	glVertex3f(x2,y2,0.0);
	glVertex3f(x1,y2,0.0);
}

void Game::init(float w_, float h_, int nrows_, int ncols_)
{
	nrows = nrows_;
	ncols = ncols_;
	grid = new CubeGrid(w_, h_, nrows, ncols);
	int rand_type = random(7);
	cube = new CubeElement(nrows-1,5,rand_type);
}

void Game::step()    //控制图形方块的运动轨迹
{
	grid->resetMovingLabel(cube); //把网格里面的4*4矩阵labels全部清零
	cube->moveDown();             //每次cube->i--
	grid->setMovingLabel(cube);//此步控制画图的地方
}

void Game::left()  //控制程序中方块向左调整位置
{
	grid->resetMovingLabel(cube);
	if(flag_left_stop==0)
	{
		cube->moveLeft();
		flag_stop_leftdown = 1;
	}
	if (cube->getI() == 0)
		cube->setI(nrows-1);
	grid->setMovingLabel(cube);
	flag_moveDown = 0;
}

void Game::right() //控制程序中方块向右调整位置
{
	grid->resetMovingLabel(cube);
	if(flag_right_stop==0)
	{
		cube->moveRight();
	}
	if (cube->getI() == 0)
		cube->setI(nrows-1);
	grid->setMovingLabel(cube);
	flag_moveDown = 0;
}

void Game::rotate()
{
	cube->getMat().rotate90();
}

void Game::draw()
{
/*	int rand_type = random(5);
	if(flag_moveDown==1)
		cube = new CubeElement(nrows-1,ncols/2,rand_type);*/
	grid->draw_squaGrid();
	grid->drawGrid();
}

void Game::speedup()
{
	if(flag_time>=0 ) //speed button
		flag_time = 2;
}

void Game::pause()
{
	flag_time = INFTY;
}

void Game::start()
{
	flag_time = time_cont;
}

//控制往左前进的时候不会碰壁；
void CubeGrid::find_left(CubeElement* e)
{
	int flag_out = 0,flag_i3=0;
	for (int j = 0; j < 4; ++j)
	{
		int J = e->getJ() + j;
		if (J >= ncols) continue;
		for (int i = 0; i < 4; ++i)
		{
			int I = e->getI() + i;
			if (I >= nrows) continue;
			if(e->getMat().get(i,j)==1)
			{
				if(e->getMat().get(i,j)==1 && J<=0)
				{
					flag_left_stop = 1;
					flag_out = 1;
					break;
				}
				else if(e->getMat().get(i,j)==1 && J>0)
				{
					if(I>=0)
					{
					if(label[I][J-1]==1)
					{
						flag_left_stop = 1;
						flag_out = 1;
						break;
					}
					}
					flag_left_stop = 0;
				}
				else
					flag_left_stop = 0;
				flag_i3 = 1;
			}
			if(i==3 && flag_i3==1)
			{
				flag_out = 1;
				flag_i3 = 0;
			}
		}
		if(flag_out==1)
		{
			flag_out = 0;
			break;
		}
		flag_left_stop = 0;
	}
}

//控制往右前进的时候不会碰壁；
void CubeGrid::find_right(CubeElement* e)
{
	int flag_out = 0,flag_i3=0;
	for (int j = 3; j >= 0; --j)
	{
		int J = e->getJ() + j;

		for (int i = 0; i < 4; ++i)
		{
			int I = e->getI() + i;
			if (I >= nrows) continue;
			if(e->getMat().get(i,j)==1)
			{
				if(e->getMat().get(i,j)==1 && J>=9)
				{
					flag_right_stop = 1;
					flag_out = 1;
					break;
				}
				else if(e->getMat().get(i,j)==1 && J<9)
				{
					if(I>=0)
					{
						if(label[I][J+1]==1)
						{
							flag_right_stop = 1;
							flag_out = 1;
							break;
						}
					}
					flag_right_stop = 0;
				}
				else
					flag_right_stop = 0;
				flag_i3 = 1;
			}
			if(i==3 && flag_i3==1)
			{
				flag_out = 1;
				flag_i3 = 0;
			}
		}
		if(flag_out==1)
		{
			flag_out = 0;
			break;
		}
		flag_right_stop = 0;
	}
}

void CubeGrid::find_down(CubeElement* e)
{

	for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}

	int flag_value_1=0;
	for (int i = 0; i < 4; ++i)
	{
		int I = e->getI() + i;
		if (I >= nrows) continue;
		for (int j = 0; j < 4; ++j)
		{
			int J = e->getJ() + j;
			if (J >= ncols) continue;
			if(e->getMat().get(i,j)==1)
			{
				flag_value_1 = 1;
				if(I==0)
				{
					flag_down_floor = 1;
				}
				else if(I>0 && label[I-1][J]==1)
					flag_down_floor = 1;
				break;
				
			}
		}
		if(i==0 && flag_value_1==0 && I==0)
		{
			e->moveDown();
			flag_down_floor = 0;
			break;
		}
		if(i==0 && flag_value_1==0 && I==-1)
		{
			flag_down_floor = 1;
			break;
		}

		if(flag_down_floor==1 || flag_value_1==1)
		{
			flag_value_1 = 0;
			break;
		}
	}

		for (int i=0;i<15;++i)             //输出label矩阵
	{
		cout<<"the reset label matrix of"<<' '<<i<<" rows*************"<<endl;
		for (int j=0;j<10;++j)
		{
			cout<<label[i][j]<<' ';
		}
		cout<<endl;
	}

}




