/*
 *  main.c
 *  This program draws several overlapping filled polygons
 *  to demonstrate the effect order has on alpha blending results.
 *  Use the 't' key to toggle the order of drawing polygons.
 */

#include <stdlib.h>
#include <iostream>
#include <cstdlib>
#include <GL/glut.h>
#include "RussianCube.h"
using namespace std; 

static int leftFirst = GL_TRUE;
float _angle = 0.0;

//int temp_moveDown = 0;


Game russianCube; //声明一个Game类的对象


/*  Initialize alpha blending function.
 */
static void init(void)
{
   glEnable (GL_BLEND); // 启动混合并设置混合因子
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_FLAT); //选择平直或平滑着色
   glClearColor (0.0, 0.0, 0.0, 0.0);

   russianCube.init(2.0f/3.0f, 1.0f, 15, 10);

}

static void drawQuad(void)
{
	/* draw pink quad on RHS of screen */

	glBegin (GL_QUADS);
		glColor4f(1.0, 0.0, 0.0, 0.75);
		glVertex3f(0.45f,0.3f,0.0);
		glVertex3f(0.55f,0.3f,0.0);
		glVertex3f(0.55f,0.7f,0.0);
		glVertex3f(0.45f,0.7f,0.0);
	glEnd();
}

static void drawCubeElement(CubeElement* element)
{
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

   russianCube.draw();//画图的函数。

   glutSwapBuffers();//使用双缓存是为了把完整图画一次性显示在窗口上，或者是为了实现动画.
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h) 
      gluOrtho2D (0.0, 1.0, 0.0, 1.0*(GLfloat)h/(GLfloat)w);
   else 
      gluOrtho2D (0.0, 1.0*(GLfloat)w/(GLfloat)h, 0.0, 1.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case ' ':
		  russianCube.rotate();
		  ++flag_space;
		  glutPostRedisplay();
		  break;
	  case 'd':
	  case 'D':
		  /* do something here*/
		  russianCube.right();
		  glutPostRedisplay();//将当前窗口打上标记，标记其需要再次显示
		  break;
	  case 'A':
	  case 'a':
		  /* do something here*/
		  // your commands
		  russianCube.left();
		  glutPostRedisplay();
         break;
	  case 's':
	  case 'S':
		  russianCube.speedup();
		  glutPostRedisplay();
		  break;
      case 27:  /*  Escape key  */
			exit(0);
			break;
	  case 'p':
	  case 'P':
		  flag_pause = 1;
		  glutPostRedisplay();		  
		  break;
	  case 'o':
	  case 'O':
		  flag_pause = 0;
		  glutPostRedisplay();
		  break;
      default:
         break;
   }
}

void while_flag()
{
	while(flag_pause==1 && flag_start==0)
	{
		glutKeyboardFunc (keyboard);
	}
}

void timer(int value)
{
	russianCube.step(); //该函数里面进行4*4网格的设置，并且里面有resetMovingLabel函数
	while_flag();
	glutPostRedisplay();//将当前窗口打上标记，标记其需要再次显示
	glutTimerFunc(flag_time, timer, 0);
	flag_moveDown = 0;
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);//使用双缓存，深度缓存
   glutInitWindowSize (600, 900);//设置glut程序产生窗口的大小和位置，以像素为单位
   glutCreateWindow (argv[0]);//产生一个顶层的窗口
   init();

   glutReshapeFunc (reshape);//指定当前窗口大小改变时，调用的函数
   glutKeyboardFunc (keyboard);//注册当前窗口的键盘回调函数
   if(!flag_pause){
		glutDisplayFunc (display);//为当前窗口设置显示回调函数
		glutTimerFunc (flag_time, timer, 0);//注册一个回调函数，等待700ms刷新一次，timer是注册的函数
   }
   glutMainLoop();//让glut程序进入事件循环
   return 0;
}
