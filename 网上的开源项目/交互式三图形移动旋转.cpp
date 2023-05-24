/*
键盘上下左右平移
鼠标左键选中 右键顺时针旋转
https://blog.csdn.net/OOFFrankDura/article/details/79841314
 */
#include <stdlib.h>
#include <glut.h>

#include<math.h>
#include <iostream>
using namespace std;
//屏幕尺寸
int SCREEN_WIDTH = 600;
int SCREEN_HEIGHT = 600;
int radius = 60;
int flag = 1;
float scales = 1.1;
float cut_scales = 0.01;
int init_step = 110;                    //记录一开始各个图形之间的间距
float PI = 3.1415926;
float angles = 0.01;
//存放选中物体的缓冲的尺寸
#define SIZE 500
//选中区域的尺寸
#define N 10
struct obj {
	int index;
	GLfloat rtx0 = 0, rty0 = 0, rtz0 = 1;
	GLfloat xmin = 0, ymin = 0, xmax = 0, ymax = 0;
	GLfloat x1 = xmin, y1 = ymin, x2 = xmax, y2 = ymin, x3 = xmax, y3 = ymax, x4 = xmin, y4 = ymax;
	GLfloat xscale = 1.0, yscale = 1.0, zscale = 1.0;
	int l, r, u, d;
};
//当前选中物体
obj obj_con[4];
int select_point = 0;//1 是第一个点，2是第二个，以此类推
int selected = 0;
void init()
{
	/*函数说明：
	 进行初始化布局*/
	glClearColor(0, 0, 0, 1);
	//首先初始化布局画布
	for (int i = 1; i <= 3; i++) {
		if (i == 1) {
			obj_con[i].xmin = 0 + init_step;
			obj_con[i].xmax = 100 + init_step;
			obj_con[i].ymin = 320;
			obj_con[i].ymax = 420;
			obj_con[i].index = 1;
			obj_con[i].r = 400;
			obj_con[i].l = -120;
			obj_con[i].u = 160;
			obj_con[i].d = -330;
		}
		else if (i == 2) {
			obj_con[i].xmin = 60 + 2 * init_step;
			obj_con[i].xmax = 160 + 2 * init_step;
			obj_con[i].ymin = 320;
			obj_con[i].ymax = 420;
			obj_con[i].index = 2;
			obj_con[i].r = 230;
			obj_con[i].l = -290;
			obj_con[i].u = 160;
			obj_con[i].d = -330;
		}
		else {
			obj_con[i].r = 20;
			obj_con[i].l = -480;
			obj_con[i].u = 150;
			obj_con[i].d = -320;
		}
		obj_con[i].x1 = obj_con[i].xmin;
		obj_con[i].y1 = obj_con[i].ymin;
		obj_con[i].x2 = obj_con[i].xmax;
		obj_con[i].y2 = obj_con[i].ymin;
		obj_con[i].x3 = obj_con[i].xmax;
		obj_con[i].y3 = obj_con[i].ymax;
		obj_con[i].x4 = obj_con[i].xmin;
		obj_con[i].y4 = obj_con[i].ymax;
	}

}
void Drawtri()
{
	/*
	 函数说明：绘制圆形并进行颜色填充
	 理解为足够小的三角形拼凑成为圆形
	 */
	double n = 1000;//分段数
	float R = 20;//半径
	int i;
	glPushMatrix();
	glColor3f(0.0, 0.2, 0.8);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0, 0.0);
	for (i = 0; i <= n; i++)
		glVertex2f(R*cos(2 * PI / n * i), R*sin(2 * PI / n * i));
	glEnd();
	glPopMatrix();
}
void draw(GLenum mode)
{
	const static GLfloat color_selected[] = { 0.5f,1.0f,0.0f };
	const static GLfloat color_unselected[] = { 1.0f,1.0f,1.0f };
	//先绘制指示灯
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(0, 570); //定点坐标范围
	glVertex2f(600, 570);
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 1.0);
	glVertex2f(0, 574); //定点坐标范围
	glVertex2f(600, 574);
	glEnd();
	//模型视图矩阵堆栈是干嘛用的呢？我们在三维空间中绘制模型，大部分时候需要对模
	//型进行移动、旋转、缩放操作.
	//而OpenGL移动的不是模型，而是坐标系
	for (int i = 1; i <= 3; i++)//5个点
	{
		glPointSize(15);
		glBegin(GL_POINTS);
		glVertex2f(0, 0);
		glEnd();
		//我们将每个一个坐标系记录在矩阵栈中
		//为了实现鼠标点击我们将每个图形放入对象栈中
		//我们不将指示灯放入坐标矩阵中
		if (mode == GL_SELECT) glLoadName(i);
		glColor3fv((selected == i) ? color_selected : color_unselected);
		glPointSize(15);
		glBegin(GL_POINTS);
		glVertex2f(20 + 30 * i, 587);
		glEnd();
		glPushMatrix();
		if (i == 1) {
			glTranslatef(obj_con[i].rtx0, obj_con[i].rty0, obj_con[i].rtz0);//指定移动坐标系
			glBegin(GL_QUADS);
			glColor3f(0.0f, 1.0f, 0.0f); //这个只是对点着色然后好看些
			glVertex2f(obj_con[i].x1, obj_con[i].y1);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(obj_con[i].x2, obj_con[i].y2);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(obj_con[i].x3, obj_con[i].y3);
			glColor3f(0.5f, 0.5f, 0.5f);
			glVertex2f(obj_con[i].x4, obj_con[i].y4);
			glEnd();
		}
		else if (i == 2) {
			glTranslatef(obj_con[i].rtx0, obj_con[i].rty0, obj_con[i].rtz0);//指定移动坐标系
			glBegin(GL_TRIANGLES);
			glColor3f(0.0f, 1.0f, 0.0f); //这个只是对点着色然后好看些
			glVertex2f(obj_con[i].x1, obj_con[i].y1);
			glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(obj_con[i].x2, obj_con[i].y2);
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(obj_con[i].x3, obj_con[i].y3);
			glEnd();
		}
		else {
			glTranslatef(obj_con[i].rtx0, obj_con[i].rty0, obj_con[i].rtz0);//指定移动坐标系
			obj_con[i].xmin = 200 + 3 * init_step;
			obj_con[i].ymin = 370;
			obj_con[i].index = 3;
			int count;
			int sections = 10000;
			GLfloat TWOPI = 2.0f * 3.14159f;
			glBegin(GL_TRIANGLE_FAN);
			glVertex2f(obj_con[i].xmin, obj_con[i].ymin);
			glColor3f(1.0f, 0.8f, 0.3f);
			for (count = 0; count <= sections; count++)
				glVertex2f(obj_con[i].xmin + radius * cos(count*TWOPI / sections), obj_con[i].ymin + radius * sin(count*TWOPI / sections));
			glEnd();
		}//这里设置我们的图形
		glPopMatrix();
	}
	glutSwapBuffers();
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	draw(GL_RENDER);
	glFlush();
}
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	glMatrixMode(GL_MODELVIEW);
}
void processHits(GLint hits, GLuint buffer[])
{
	unsigned int i, j;
	GLuint name;
	for (i = 0; i < hits; i++)
	{
		name = buffer[3 + i * 4];
		select_point = name;//每个选中物体有占用名字栈中4个单位，第4个是物体名字的值
		selected = name;
	}
}
double get_x(double x, double y, double theta) {
	//得到旋转后的x坐标
	return x * cos(theta) - sin(theta) * y;
}
double get_y(double x, double y, double theta) {
	//得到旋转后的y坐标
	return x * sin(theta) + cos(theta) * y;
}
void show_point(int x) {
	/*
	 函数说明：控制指示灯的闪亮情况
	 */
	glBegin(GL_POINTS);
	glPointSize(15);
	glColor3f(1.0, 1.0, 0.0);
	glVertex2f(20 + 30 * x, 787);
	glEnd();
}
void move_to_o() {
	/*函数说明：先将物体移动到原点
	 再进行目标操作（旋转放缩）
	 */
	float x = (obj_con[selected].xmin + obj_con[selected].xmax) / 2;
	float y = (obj_con[selected].ymin + obj_con[selected].ymax) / 2;
	obj_con[selected].x1 -= x;
	obj_con[selected].x2 -= x;
	obj_con[selected].x3 -= x;
	obj_con[selected].x4 -= x;
	obj_con[selected].y1 -= y;
	obj_con[selected].y2 -= y;
	obj_con[selected].y3 -= y;
	obj_con[selected].y4 -= y;
}
void move_back() {
	/*
	 函数说明：将移动到原点的物体移动回到原来的位置
	 */
	float x = (obj_con[selected].xmin + obj_con[selected].xmax) / 2;
	float y = (obj_con[selected].ymin + obj_con[selected].ymax) / 2;
	obj_con[selected].x1 += x;
	obj_con[selected].x2 += x;
	obj_con[selected].x3 += x;
	obj_con[selected].x4 += x;
	obj_con[selected].y1 += y;
	obj_con[selected].y2 += y;
	obj_con[selected].y3 += y;
	obj_con[selected].y4 += y;
}
void move_to_o2() {
	/*函数说明：先将物体移动到原点
	 该函数控制的是错切变换的结果
	 */
	float x = (obj_con[selected].xmin + obj_con[selected].xmax) / 2 - (obj_con[selected].xmax - obj_con[selected].xmin) / 2;
	float y = (obj_con[selected].ymin + obj_con[selected].ymax) / 2 - (obj_con[selected].ymax - obj_con[selected].ymin) / 2;
	obj_con[selected].x1 -= x;
	obj_con[selected].x2 -= x;
	obj_con[selected].x3 -= x;
	obj_con[selected].x4 -= x;
	obj_con[selected].y1 -= y;
	obj_con[selected].y2 -= y;
	obj_con[selected].y3 -= y;
	obj_con[selected].y4 -= y;
}
void move_back2() {
	/*
	 函数说明：将移动到原点的物体移动回到原来的位置
	 该函数控制的是错切变换的结果
	 */
	float x = (obj_con[selected].xmin + obj_con[selected].xmax) / 2 - (obj_con[selected].xmax - obj_con[selected].xmin) / 2;
	float y = (obj_con[selected].ymin + obj_con[selected].ymax) / 2 - (obj_con[selected].ymax - obj_con[selected].ymin) / 2;
	obj_con[selected].x1 += x;
	obj_con[selected].x2 += x;
	obj_con[selected].x3 += x;
	obj_con[selected].x4 += x;
	obj_con[selected].y1 += y;
	obj_con[selected].y2 += y;
	obj_con[selected].y3 += y;
	obj_con[selected].y4 += y;
}
/*
 注意下述旋转、错切、平移、放缩等
 都需要实现进行移动坐标在移动回来
 */
void rotato() {
	/*
	 函数说明：进行旋转操作
	 */
	obj_con[selected].x1 = get_x(obj_con[selected].x1, obj_con[selected].y1, angles);
	obj_con[selected].y1 = get_y(obj_con[selected].x1, obj_con[selected].y1, angles);
	obj_con[selected].x2 = get_x(obj_con[selected].x2, obj_con[selected].y2, angles);
	obj_con[selected].y2 = get_y(obj_con[selected].x2, obj_con[selected].y2, angles);
	obj_con[selected].x3 = get_x(obj_con[selected].x3, obj_con[selected].y3, angles);
	obj_con[selected].y3 = get_y(obj_con[selected].x3, obj_con[selected].y3, angles);
	obj_con[selected].x4 = get_x(obj_con[selected].x4, obj_con[selected].y4, angles);
	obj_con[selected].y4 = get_y(obj_con[selected].x4, obj_con[selected].y4, angles);
}
void cut_move(int flag, int flage) {
	/*函数说明：进行指定方向的错切操作
	 参数说明：第一个参数flag指定x方向和y方向 0，1
	 第二个参数flage指定是放大还是缩小
	 */
	float sc = cut_scales;
	if (flage == 1)      //看是放大还是缩小
		sc = -sc;
	if (flag == 1) {
		/*x = cx+y
		 保持y不变*/
		obj_con[selected].x1 = sc * obj_con[selected].y1 + obj_con[selected].x1;
		obj_con[selected].x2 = sc * obj_con[selected].y2 + obj_con[selected].x2;
		obj_con[selected].x3 = sc * obj_con[selected].y3 + obj_con[selected].x3;
		obj_con[selected].x4 = sc * obj_con[selected].y4 + obj_con[selected].x4;
	}
	else if (flag == 2) {
		obj_con[selected].y1 = sc * obj_con[selected].x1 + obj_con[selected].y1;
		obj_con[selected].y2 = sc * obj_con[selected].x2 + obj_con[selected].y2;
		obj_con[selected].y3 = sc * obj_con[selected].x3 + obj_con[selected].y3;
		obj_con[selected].y4 = sc * obj_con[selected].x4 + obj_con[selected].y4;
	}
}
void enlarge(int flag, int flage) {
	/*函数说明：进行指定方向的放缩操作
	   参数说明：第一个参数flag指定x方向和y方向以及全方向
					  第二个参数flage指定是放大还是缩小
	 */
	float sc = scales;
	if (flage == 1)
		sc = 1 / sc;  //判断是放大还是缩小
	if (flag == 1) {
		obj_con[selected].x1 *= sc;
		obj_con[selected].x2 *= sc;
		obj_con[selected].x3 *= sc;
		obj_con[selected].x4 *= sc;
	}
	else if (flag == 2) {
		obj_con[selected].y1 *= sc;
		obj_con[selected].y2 *= sc;
		obj_con[selected].y3 *= sc;
		obj_con[selected].y4 *= sc;
	}
	else {
		obj_con[selected].x1 *= sc;
		obj_con[selected].x2 *= sc;
		obj_con[selected].x3 *= sc;
		obj_con[selected].x4 *= sc;
		obj_con[selected].y1 *= sc;
		obj_con[selected].y2 *= sc;
		obj_con[selected].y3 *= sc;
		obj_con[selected].y4 *= sc;
	}
}
void mouse(int button, int state, int x, int y)
{   /*
	 函数说明：控制鼠标的事件
	 */
	GLuint selectBuffer[SIZE];//存放物体名称的栈
	GLint hits;//存放被选中对象个数
	GLint viewport[4];//存放可视区参数
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON)//当鼠标左键按下时
	{
		glGetIntegerv(GL_VIEWPORT, viewport);         //获取当前视口坐标参数
		glSelectBuffer(SIZE, selectBuffer);                  //选择名称栈存放被选中的名称
		glRenderMode(GL_SELECT);                        //设置当前为 选择模式
		glInitNames();                                             //名称栈
		glPushName(0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPickMatrix(x, viewport[3] - y, N, N, viewport);//创建用于选择的投影矩阵栈
		gluOrtho2D(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);//设置投影方式
		draw(GL_SELECT);//绘制场景
		glPopMatrix();
		glFlush();
		hits = glRenderMode(GL_RENDER);
		glMatrixMode(GL_MODELVIEW);
		if (hits > 0)processHits(hits, selectBuffer);
		glutPostRedisplay();
	}
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON)//当鼠标左键按下时
	{
		//旋转
		if (obj_con[selected].index == 3) {
			return;
		}
		else if (obj_con[selected].index == 1 || obj_con[selected].index == 2) {
			move_to_o();                                                                //首先移动到原点
			rotato();
			move_back();                                                                //然后移动到原来的位置
		}
		glutPostRedisplay();
	}
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) //当鼠标左键抬起时
	{
		select_point = 0;
		glRenderMode(GL_RENDER);
		draw(GL_RENDER);
		glutPostRedisplay();
	}
}
void processKeyBoard(int key, int x, int y)
{/*
  函数说明：处理键盘的信息。
  具体的操作参考实际操作目录表
  */    int step = 10;
switch (key)
{
case '1':
	selected = 1;
	break;
case '2':
	selected = 2;
	break;
case '3':
	selected = 3;
	break;
case 'u':
	move_to_o2();                                                                //首先移动到原点
	cut_move(1, 0);
	move_back2();                                                                //然后移动到原来的位置
	break;
case 'i':
	move_to_o2();                                                                //首先移动到原点
	cut_move(2, 0);
	move_back2();
	break;
case 'j':
	move_to_o2();                                                                //首先移动到原点
	cut_move(1, 1);
	move_back2();                                                                //然后移动到原来的位置
	break;
case 't':
	rotato();
	break;
case 'k':
	move_to_o2();                                                                //首先移动到原点
	cut_move(2, 1);
	move_back2();
	break;
case 'p':
	scales += 0.01;
	if (scales < 1)
		scales = 1;
	cout << "加速缩放:" << scales << endl;
	break;
case 'o':
	scales -= 0.01;
	if (scales < 1)
		scales = 1;
	cout << "减速缩放:" << scales << endl;
	break;
case 'z':
	if (selected == 3) {
		radius = radius * scales;
	}
	else {
		move_to_o();                                                                //首先移动到原点
		enlarge(1, 0);
		move_back();                                                                //然后移动到原来的位置
	}
	break;
case 'x':
	if (selected == 3) {
		radius = radius * scales;
	}
	else {
		move_to_o();                                                                //首先移动到原点
		enlarge(2, 0);
		move_back();                                                                //然后移动到原来的位置
	}
	break;
case 'c':
	if (selected == 3) {
		radius = radius * (scales);
	}
	else {
		move_to_o();                                                                //首先移动到原点
		enlarge(3, 0);
		move_back();                                                                //然后移动到原来的位置
	}
	break;
case 'v':
	if (selected == 3) {
		radius = radius * (1 / scales);
	}
	else {
		move_to_o();                                                                //首先移动到原点
		enlarge(1, 1);
		move_back();                                                                //然后移动到原来的位置
	}
	break;
case 'b':
	if (selected == 3) {
		radius = radius * (1 / scales);
	}
	else {
		move_to_o();                                                                //首先移动到原点
		enlarge(2, 1);
		move_back();                                                                //然后移动到原来的位置
	}
	break;
case 'n':
	if (selected == 3) {
		radius = radius * (1 / scales);
	}
	else {
		move_to_o();                                                                //首先移动到原点
		enlarge(3, 1);
		move_back();                                                                //然后移动到原来的位置
	}
	break;
case GLUT_KEY_LEFT:
	if (obj_con[selected].rtx0 - step > obj_con[selected].l)obj_con[selected].rtx0 -= step;
	break;
case GLUT_KEY_RIGHT:
	if (obj_con[selected].rtx0 + step < obj_con[selected].r)obj_con[selected].rtx0 += step;
	break;
case GLUT_KEY_UP:
	if (obj_con[selected].rty0 + step < obj_con[selected].u)obj_con[selected].rty0 += step;
	break;
case GLUT_KEY_DOWN:
	if (obj_con[selected].rty0 - step > obj_con[selected].d)obj_con[selected].rty0 -= step;
	break;
case 'a':
	//表示可以按照反向旋转
	angles = -angles;
	break;
default:
	break;
}
glutPostRedisplay();
return;
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(300, 10);
	glutCreateWindow("CG实验1");
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutSpecialFunc(processKeyBoard);//定义键盘事件

	glutMainLoop();

	return 1;
}

