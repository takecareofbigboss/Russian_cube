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


Game russianCube; //����һ��Game��Ķ���


/*  Initialize alpha blending function.
 */
static void init(void)
{
   glEnable (GL_BLEND); // ������ϲ����û������
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_FLAT); //ѡ��ƽֱ��ƽ����ɫ
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

   russianCube.draw();//��ͼ�ĺ�����

   glutSwapBuffers();//ʹ��˫������Ϊ�˰�����ͼ��һ������ʾ�ڴ����ϣ�������Ϊ��ʵ�ֶ���.
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
		  glutPostRedisplay();//����ǰ���ڴ��ϱ�ǣ��������Ҫ�ٴ���ʾ
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
	russianCube.step(); //�ú����������4*4��������ã�����������resetMovingLabel����
	while_flag();
	glutPostRedisplay();//����ǰ���ڴ��ϱ�ǣ��������Ҫ�ٴ���ʾ
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
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);//ʹ��˫���棬��Ȼ���
   glutInitWindowSize (600, 900);//����glut����������ڵĴ�С��λ�ã�������Ϊ��λ
   glutCreateWindow (argv[0]);//����һ������Ĵ���
   init();

   glutReshapeFunc (reshape);//ָ����ǰ���ڴ�С�ı�ʱ�����õĺ���
   glutKeyboardFunc (keyboard);//ע�ᵱǰ���ڵļ��̻ص�����
   if(!flag_pause){
		glutDisplayFunc (display);//Ϊ��ǰ����������ʾ�ص�����
		glutTimerFunc (flag_time, timer, 0);//ע��һ���ص��������ȴ�700msˢ��һ�Σ�timer��ע��ĺ���
   }
   glutMainLoop();//��glut��������¼�ѭ��
   return 0;
}
