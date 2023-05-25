#define _CRT_SECURE_NO_WARNINGS
#include <GL/glew.h>
#include <freeglut/freeglut.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
using namespace std;

#define Pi 3.1415926536f  
struct point {
	long x;
	long y;
	point(long _x = 0, long _y = 0) :x(_x), y(_y) {}
};//�����Ľṹ��  

vector<point> line = {}, circle = {};
vector<vector<point>> poly = { {{0,0}} }, points = { {} };

int translatingLine = -1;
int translatingPoly = -1;
int translatingCircle = -1;
vector<point> cutline(2), translate(2);
point rotatePoly;
point scalePoly;
point fillPoly;
point symPoly;
point eraser;

int n = 1000;//����Բ�ľ���  
int shape = 0;//��¼����Ҫ������״����  
int color = 1;//��¼����Ҫ������ɫ  
int screenWidth = 640;
int screenHeight = 480;
//��ʼ��  
void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}
//б����0-1֮�ڵ��е㻭�߷�
vector<point> M(int x1, int y1, int x2, int y2) {
	vector<point> res(x2 - x1 + 1);
	int a = y1 - y2, b = x2 - x1, c = -(a*x1 + b * y1);
	int d = a + a + b, da = a + a, db = b + b;
	int x = x1, y = y1;
	for (int i = 0; x <= x2; ++i) {
		res[i] = { x, y };
		if (d >= 0) {
			d += da;
		}
		else {
			d += da + db;
			++y;
		}
		++x;
	}
	return res;
}
//б����0-1֮�ڵ�Bresham���߷�
vector<point> B(int x1, int y1, int x2, int y2) {
	vector<point> res(x2 - x1 + 1);
	int dy = y2 - y1, dx = x2 - x1;
	int e = dy + dy - dx;
	int ddy = dy + dy, ddx = -dx - dx;
	int x = x1, y = y1;
	for (int i = 0; x <= x2; ++i) {
		res[i] = { x, y };
		if (e >= 0) {
			e += ddy + ddx;
			++y;
		}
		else {
			e += ddy;
		}
		++x;
	}
	return res;
}
//����  
void drawThread(int x1, int y1, int x2, int y2)
{
	vector<point> res;
	glBegin(GL_POINTS);
	int dy = y2 - y1, dx = x2 - x1;
	if (dy * dx < 0) {
		if (abs(dy) > abs(dx)) {
			if (shape == 2) {
				if (y1 < y2)
					res = M(y1, -x1, y2, -x2);
				else
					res = M(y2, -x2, y1, -x1);
			}
			else {
				if (y1 < y2)
					res = B(y1, -x1, y2, -x2);
				else
					res = B(y2, -x2, y1, -x1);
			}
			for (auto p : res)
				glVertex2i(-p.y, p.x);
		}
		else {
			if (shape == 2) {
				if (x1 < x2)
					res = M(x1, -y1, x2, -y2);
				else
					res = M(x2, -y2, x1, -y1);
			}
			else {
				if (x1 < x2)
					res = B(x1, -y1, x2, -y2);
				else
					res = B(x2, -y2, x1, -y1);
			}
			for (auto p : res)
				glVertex2i(p.x, -p.y);
		}
	}
	else if (abs(dy) > abs(dx)) {
		if (shape == 2) {
			if (y1 < y2)
				res = M(y1, x1, y2, x2);
			else
				res = M(y2, x2, y1, x1);
		}
		else {
			if (y1 < y2)
				res = B(y1, x1, y2, x2);
			else
				res = B(y2, x2, y1, x1);
		}
		for (auto p : res)
			glVertex2i(p.y, p.x);
	}
	else {
		if (shape == 2) {
			if (x1 < x2)
				res = M(x1, y1, x2, y2);
			else
				res = M(x2, y2, x1, y1);
		}
		else {
			if (x1 < x2)
				res = B(x1, y1, x2, y2);
			else
				res = B(x2, y2, x1, y1);
		}
		for (auto p : res)
			glVertex2i(p.x, p.y);
	}
	glEnd();
}
//��Բ  
//�е㻭�߷���Brensenham���߷�
void drawCircle(int x1, int y1, int x2, int y2)
{
	point mid = { (x1 + x2) / 2, (y1 + y2) / 2 };
	int r = sqrt((x1 - mid.x)*(x1 - mid.x) + (y1 - mid.y)*(y1 - mid.y));
	glBegin(GL_POINTS);
	if (shape == 5)
	{
		int d = 1 - r;
		int x = 0, y = r;
		while (y >= x)
		{
			//����
			//......
			glVertex2i(x + mid.x, y + mid.y);
			glVertex2i(y + mid.x, x + mid.y);
			glVertex2i(-x + mid.x, y + mid.y);
			glVertex2i(-y + mid.x, x + mid.y);
			glVertex2i(-x + mid.x, -y + mid.y);
			glVertex2i(-y + mid.x, -x + mid.y);
			glVertex2i(x + mid.x, -y + mid.y);
			glVertex2i(y + mid.x, -x + mid.y);

			//����
			if (d > 0)
			{
				++x, --y;
				d += x + x - y - y + 1;
			}
			else
			{
				++x;
				d += x + x + 1;
			}
		}
	}
	else
	{
		int e = 2 - r - r;
		int d;
		int x = 0, y = r;
		int flag;
		while (y >= 0)
		{
			//����...
			glVertex2i(x + mid.x, y + mid.y);
			glVertex2i(-x + mid.x, y + mid.y);
			glVertex2i(-x + mid.x, -y + mid.y);
			glVertex2i(x + mid.x, -y + mid.y);
			//����
			if (e > 0)
			{
				d = (e - x) * 2 - 1;
				if (d > 0)
				{
					--y;
					flag = 3;
				}
				else
				{
					++x, --y;
					flag = 2;
				}
			}
			else
			{
				d = (e + y) * 2 - 1;
				if (d > 0)
				{
					++x, --y;
					flag = 2;
				}
				else
				{
					++x;
					flag = 1;
				}
			}
			switch (flag)
			{
			case 1:
				e += x + x + 1;
				break;
			case 2:
				e += x + x - y - y + 2;
				break;
			case 3:
				e += -y - y + 1;
				break;
			}
		}
	}
	glEnd();
}
double angle2D(int x1, int y1, int x2, int y2)
{
	//���Ҷ�����cosA = a dot b / |a||b|
	double alpha = acos((x1 * x2 + y1 * y2) / sqrt(x1*x1 + y1 * y1) / sqrt(x2*x2 + y2 * y2));
	if (x1 * y2 - x2 * y1 < 0)
	{
		return -alpha;
	}
	return alpha;
}
//�жϵ���Բ��
int pickCircle(point pt)
{
	for (int i = 0; i + 1 < circle.size(); i += 2)
	{
		point mid = { (circle[i].x + circle[i + 1].x) / 2, (circle[i].y + circle[i + 1].y) / 2 };
		double r2 = (circle[i].x - mid.x)*(circle[i].x - mid.x) + (circle[i].y - mid.y)*(circle[i].y - mid.y);
		double r1 = (pt.x - mid.x)*(pt.x - mid.x) + (pt.y - mid.y)*(pt.y - mid.y);
		if (r1 < r2)
			return i;
	}
	return -1;
}
//�жϵ���ֱ����
int pickLine(point pt)
{
	for (int i = 0; i + 1 < line.size(); i += 2)
	{
		//(pt.y-p1.y)/(pt.x-p1.x) == (p2.y-p1.y)/(p2.x-p1.x)
		double k1 = (pt.y - line[i].y)*0.1 / (pt.x - line[i].x);
		double k2 = (line[i + 1].y - line[i].y) / (line[i + 1].x - line[i].x);
		//Ax+By+C=0
		//C=-x1(y2-y1)+y1(x2-x1)
		int A = (line[i + 1].y - line[i].y);
		int B = -(line[i + 1].x - line[i].x);
		int C = -line[i].x*A - line[i].y*B;
		double diff = 0.1*(A*pt.x + B * pt.y + C) / sqrt(A*A + B * B);
		if (diff < 1 && diff > -1)
		{
			return i;
		}
	}
	return -1;
}
//�ж���ѡ�����ڵĶ���Σ��޷���-1�����򷵻�poly�±�
int pickPoly(point pt)
{
	//���ü���ǶȺ�
	for (int j = 0; j < poly.size(); ++j)
	{
		auto &p = poly[j];
		if (p.size() < 2)
			continue;
		double angle = 0;
		for (int i = 1; i < p.size() - 1; ++i)
		{
			angle += angle2D(p[i].x - pt.x, p[i].y - pt.y, p[i + 1].x - pt.x, p[i + 1].y - pt.y);
			cout << angle << endl;
		}
		angle += angle2D(p.back().x - pt.x, p.back().y - pt.y, p[1].x - pt.x, p[1].y - pt.y);
		cout << angle << endl;
		if (angle > 1e-6 || angle < -1e-6)
			return j;
	}
	return -1;
}
//ѡ����ɫ
void pickColor(int k = 0)
{
	switch (1 + (color + k) % 3)
	{
	case 1:
		glColor3f(1.0, 0.0, 0.0);
		break;
	case 2:
		glColor3f(0.0, 1.0, 0.0);
		break;
	case 3:
		glColor3f(0.0, 0.0, 1.0);
		break;
	}
}
//������ü���Χ�ĵ�ı���
unsigned char mask(point p, long x_min, long x_max, long y_min, long y_max)
{
	unsigned char res = 0;
	if (p.y > y_max)
		res += 1;
	if (p.y < y_min)
		res += 2;
	if (p.x > x_max)
		res += 4;
	if (p.x < x_min)
		res += 8;
	printf("mask of (%d, %d) is %0x\n", p.x, p.y, res);
	return res;
}
//��ֱ�߶�p1p2��ֱ��x=����x��y=����y�Ľ���
point cross(point p1, point p2, long x, long y)
{
	if (p1.x == p2.x)
		return { p1.x, y };
	point res;
	float k = (float)(p1.y - p2.y) / (p1.x - p2.x);
	if (x == 0)
	{
		res.y = y;
		//(res.y-p1.y)/(res.x-p1.x)=k
		res.x = p1.x + (float)(y - p1.y) / k;
	}
	else
	{
		res.x = x;
		res.y = p1.y + (float)(x - p1.x) * k;
	}
	cout << "cross.x=" << res.x << " ,cross.y=" << res.y << endl;
	return res;
}
//����ü�
std::vector<point> doCutline(point p1, point p2, point r1, point r2)
{
	long x_min = min(r1.x, r2.x), x_max = max(r1.x, r2.x);
	long y_min = min(r1.y, r2.y), y_max = max(r1.y, r2.y);

	cout << "block.x_min=" << x_min << " , x_max=" << x_max << " ,y_min=" << y_min << " ,y_max=" << y_max << endl;
	unsigned char mask1 = mask(p1, x_min, x_max, y_min, y_max);
	unsigned char mask2 = mask(p2, x_min, x_max, y_min, y_max);
	vector<point> ret;
	if (!mask1)
	{
		ret.push_back(p1);
	}
	if (!mask2)
	{
		ret.push_back(p2);
	}
	if (ret.size() == 2)
		return ret;
	if (mask1 & mask2)
	{
		return {};
	}
	unsigned char res = mask1 | mask2;
	if (res & 0x1)
	{
		point p = cross(p1, p2, 0, y_max);
		if (mask(p, x_min, x_max, y_min, y_max) == 0)
		{
			ret.push_back(p);
			if (ret.size() == 2)
				return ret;
		}
	}
	if (res & 0x2)
	{
		point p = cross(p1, p2, 0, y_min);
		if (mask(p, x_min, x_max, y_min, y_max) == 0)
		{
			ret.push_back(p);
			if (ret.size() == 2)
				return ret;
		}
	}
	if (res & 0x4)
	{
		point p = cross(p1, p2, x_max, 0);
		if (mask(p, x_min, x_max, y_min, y_max) == 0)
		{
			ret.push_back(p);
			if (ret.size() == 2)
				return ret;
		}
	}
	if (res & 0x8)
	{
		point p = cross(p1, p2, x_min, 0);
		if (mask(p, x_min, x_max, y_min, y_max) == 0)
		{
			ret.push_back(p);
			if (ret.size() == 2)
				return ret;
		}
	}
}
point center(vector<point>& _poly)
{
	long centerx = 0, centery = 0;
	for (int i = 1; i < _poly.size(); ++i)
		centerx += _poly[i].x, centery += _poly[i].y;
	centerx /= (_poly.size() - 1), centery /= (_poly.size() - 1);
	return { centerx, centery };
}
//��ת90��
void rotate(vector<point>& _poly)
{
	point c = center(_poly);
	for (int i = 1; i < _poly.size(); ++i)
	{
		_poly[i].x -= c.x, _poly[i].y -= c.y;
		//x=cos90*x-sin90*y x = -y
		//y=sin90*x+cos90*y y = x
		long tx = _poly[i].x;
		_poly[i].x = -_poly[i].y;
		_poly[i].y = tx;
		_poly[i].x += c.x, _poly[i].y += c.y;
	}
}
//�����ķŴ�1.5��
void scale(vector<point>& _poly)
{
	point c = center(_poly);
	for (int i = 1; i < _poly.size(); ++i)
	{
		_poly[i].x -= c.x, _poly[i].y -= c.y;

		_poly[i].x *= 1.5;
		_poly[i].y *= 1.5;

		_poly[i].x += c.x, _poly[i].y += c.y;
	}
}
//���������ڴ�ֱ�߶Գ�
void sym(vector<point>& _poly)
{
	point c = center(_poly);
	for (int i = 1; i < _poly.size(); ++i)
	{
		_poly[i].x = 2 * c.x - _poly[i].x;
	}
}
//ƽ��dx��dy
void translateRange(vector<point>::iterator be, vector<point>::iterator en, int dx, int dy)
{
	for (; be != en; ++be)
	{
		be->x += dx;
		be->y += dy;
	}
}
//�����ݽṹ
struct edge {
	float x;//�ײ��˵�x����
	int y;//�ײ��˵�y����
	float dx;//y����1��x�ı仯��
	int maxy;//�����˵�y����
	edge(int x1, int y1, int x2, int y2) {
		if (y1 < y2) {
			x = x1;
			y = y1;
			maxy = y2;
		}
		else {
			x = x2;
			y = y2;
			maxy = y1;
		}
		dx = (float)(x1 - x2) / (y1 - y2);
	}
	bool operator <(const edge& other) {
		return (y == other.y) ? dx < other.dx : y < other.y;
	}
};
//���Ա����
void fill(vector<point>& _poly) {
	if (_poly.size() < 2)
		return;
	//�����±߱�
	vector<edge> v;
	for (int i = 1; i + 1 < _poly.size(); i++)
	{
		if (_poly[i].y != _poly[i + 1].y)
			v.emplace_back(_poly[i].x, _poly[i].y, _poly[i + 1].x, _poly[i + 1].y);
	}
	if (_poly[1].y != _poly.back().y)
		v.emplace_back(_poly.back().x, _poly.back().y, _poly[1].x, _poly[1].y);
	//����y��������
	sort(v.begin(), v.end());
	float y = v[0].y - 1.5, maxy = -1;
	//�����yֵ
	for_each(v.begin(), v.end(), [&maxy](edge e) {if (e.maxy > maxy) maxy = e.maxy; });
	int i = 0;
	//���Ա�
	list<edge> list;
	glColor3f(1.0, 1.0, 0);
	glBegin(GL_POINTS);
	while (++y < maxy) {
		//ɾ���ɱ�
		list.remove_if([y](const edge& e)->bool {return e.maxy < y; });
		//����x����ֵ
		for_each(list.begin(), list.end(), [](edge& e) {e.x += e.dx; });
		//�����±�
		for (; i < v.size() && v[i].y < y; ++i) {
			cout << "new edge : x=" << v[i].x << " dx=" << v[i].dx << " maxy=" << v[i].maxy << endl;
			int x = v[i].x, dx = v[i].dx;
			//�ҵ�����λ��
			auto pos = find_if(list.begin(), list.end(),
				[x, dx](edge e)->bool {return (e.x == x) ? e.dx >= dx : e.x > x; });
			list.insert(pos, v[i]);
		}
		//��������
		for (auto it = list.begin(); it != list.end(); ++it) {
			int beg = it++->x;
			int last = it->x;
			for (int j = beg; j < last; ++j)
				glVertex2i(j, y);
		}
	}
	glEnd();
	pickColor();
}
//��Ƥ��ȥ����
void doErase(point e)
{
	for (int j = 0; j < points.size(); ++j)
	{
		auto& p = points[j];
		for (int i = 0; i < p.size(); ++i)
		{
			if (p[i].x - e.x < 2 && p[i].x - e.x > -2 && p[i].y - e.y < 2 && p[i].y - e.y > -2)
			{
				vector<point> tmp(p.begin() + i + 1, p.end());
				p.erase(p.begin() + i, p.end());
				points.insert(points.begin() + j + 1, tmp);
				return;
			}
		}
	}
}
//��ʾ����  
void myDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT);            //����  
	pickColor();
	//����
	for (auto l : points)
	{
		for (int i = 0; i + 1 < l.size(); ++i)
		{
			drawThread(l[i].x, l[i].y, l[i + 1].x, l[i + 1].y);
		}
	}
	//��ֱ��
	for (int i = 0; i + 1 < line.size(); i += 2)
	{
		drawThread(line[i].x, line[i].y, line[i + 1].x, line[i + 1].y);
	}
	//�������
	for (auto p : poly)
	{
		for (int i = 1; i + 1 < p.size(); i++)
		{
			drawThread(p[i].x, p[i].y, p[i + 1].x, p[i + 1].y);
		}
		if (p.size() > 2)
		{
			drawThread(p[1].x, p[1].y, p.back().x, p.back().y);
		}
	}
	//��Բ
	for (int i = 0; i + 1 < circle.size(); i += 2)
	{
		drawCircle(circle[i].x, circle[i].y, circle[i + 1].x, circle[i + 1].y);
	}
	//���
	for (auto p : poly)
	{
		if (p[0].x)
		{
			fill(p);
		}
	}
	//���ü���
	if (shape == 6)
	{
		pickColor(1);
		int x1 = cutline[0].x, x2 = cutline[1].x;
		int y1 = cutline[0].y, y2 = cutline[1].y;
		drawThread(x1, y1, x1, y2);
		drawThread(x2, y1, x2, y2);
		drawThread(x1, y1, x2, y1);
		drawThread(x1, y2, x2, y2);
		pickColor();
	}
	glFlush();       //�������������ʾ�豸  
}
//����¼�  
void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		//��¼̧��ĵ�
		if (state == GLUT_UP)
		{
			switch (shape)
			{
			case 1:
				points.push_back({});
				break;
			case 6:
				if (cutline[0].x != cutline[1].x && cutline[0].y != cutline[1].y)
				{
					//�ü��㷨
					for (int i = 0; i + 1 < line.size();)
					{
						auto res = doCutline(line[i], line[i + 1], cutline[0], cutline[1]);
						if (res.size() == 2)
						{
							line[i] = res[0];
							line[i + 1] = res[1];
							i += 2;
						}
						else
						{
							line.erase(line.begin() + i);
							line.erase(line.begin() + i);
						}
					}
				}
				break;
			}
		}
		//��¼���µĵ�
		if (state == GLUT_DOWN)
		{
			int res;
			switch (shape)
			{
			case 2:
			case 3:
				line.emplace_back(x, screenHeight - y);
				line.emplace_back(x, screenHeight - y);
				break;
			case 4:
				poly.back().emplace_back(x, screenHeight - y);
				break;
			case 5:
			case 12:
				circle.emplace_back(x, screenHeight - y);
				circle.emplace_back(x, screenHeight - y);
				break;
			case 6:
				cutline[1] = cutline[0] = { x, screenHeight - y };
				break;
			case 7:
				fillPoly.x = x;
				fillPoly.y = screenHeight - y;
				//��Ǳ����Ķ����
				int res;
				if ((res = pickPoly(fillPoly)) != -1)
				{
					poly[res][0].x = 1;
				}
				break;
			case 8:
				translate[0] = { x, screenHeight - y };
				translate[1] = { x, screenHeight - y };
				translatingLine = pickLine(translate[0]);
				translatingCircle = pickCircle(translate[0]);
				translatingPoly = pickPoly(translate[0]);
				break;
			case 9:
				rotatePoly.x = x;
				rotatePoly.y = screenHeight - y;
				//�ж϶����
				if ((res = pickPoly(rotatePoly)) != -1)
				{
					//��ת�ö����
					rotate(poly[res]);
				}
				break;
			case 10:
				scalePoly.x = x;
				scalePoly.y = screenHeight - y;
				//�ж϶����
				if ((res = pickPoly(scalePoly)) != -1)
				{
					//�Ŵ�ö����
					scale(poly[res]);
				}
				break;
			case 11:
				symPoly.x = x;
				symPoly.y = screenHeight - y;
				//�ж϶����
				if ((res = pickPoly(symPoly)) != -1)
				{
					//�ԳƸö����
					sym(poly[res]);
				}
				break;
			case 13:
				eraser.x = x;
				eraser.y = screenHeight - y;
				doErase(eraser);
				break;
			}
		}
	}
	myDisplay();
}
//��갴ס�ƶ��¼�  
void myMotion(int x, int y)
{
	int dx, dy;
	switch (shape)
	{
	case 1:
		points.back().emplace_back(x, screenHeight - y);
		break;
	case 2:
	case 3:
		line.back() = { x, screenHeight - y };
		break;
	case 5:
	case 12:
		circle.back() = { x, screenHeight - y };
		break;
	case 6:
		cutline[1] = { x, screenHeight - y };
		break;
	case 8:
		translate[1] = { x, screenHeight - y };
		dx = translate[1].x - translate[0].x;
		dy = translate[1].y - translate[0].y;
		translate[0] = translate[1];
		if (translatingCircle != -1)
		{
			translateRange(circle.begin() + translatingCircle, circle.begin() + translatingCircle + 2, dx, dy);
		}
		if (translatingPoly != -1)
		{
			translateRange(poly[translatingPoly].begin() + 1, poly[translatingPoly].end(), dx, dy);
		}
		if (translatingLine != -1)
		{
			translateRange(line.begin() + translatingLine, line.begin() + translatingLine + 2, dx, dy);
		}
		break;
	case 13:
		eraser.x = x;
		eraser.y = screenHeight - y;
		doErase(eraser);
		break;
	}

	myDisplay();
}
//����ɿ��ƶ��¼�  
void myPassiveMotion(int x, int y)
{
	myDisplay();
}
//��ɫ�˵�  
void colorSubMenu(GLint colorOption)
{
	color = colorOption;
}
//ͼ�β˵�  
void shapeSubMenu(GLint shapeOption)
{
	if (shape == 4)
	{
		poly.push_back({ {0,0} });
	}
	else if (shape == 1)
	{
		points.push_back({});
	}
	shape = shapeOption;
}
//���˵�  
void mainMenu(GLint renderingOption)
{
	switch (renderingOption)
	{
	case 3:
		line.clear();
		circle.clear();
		poly.clear();
		poly.push_back({ {0,0} });
		points.clear();
		points.push_back({});
		myDisplay();
		break;
	}
}
int main(int argc, char *argv[])
{
	GLint subMenu1; //�����Ӳ˵�  
	GLint subMenu2; //�����Ӳ˵�  
	GLint subMenu3; //�����Ӳ˵�  
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 100);            //���ô�������Ļ�ϵ�λ��  
	glutCreateWindow("Mini-Draw (right click for menus)");
	init();

	subMenu1 = glutCreateMenu(shapeSubMenu);
	glutAddMenuEntry("points", 1);
	glutAddMenuEntry("M-line", 2);
	glutAddMenuEntry("B-line", 3);
	glutAddMenuEntry("M-circle", 5);
	glutAddMenuEntry("B-circle", 12);
	glutAddMenuEntry("polygon", 4);
	subMenu2 = glutCreateMenu(colorSubMenu);
	glutAddMenuEntry("red", 1);
	glutAddMenuEntry("green", 2);
	glutAddMenuEntry("blue", 3);
	subMenu3 = glutCreateMenu(shapeSubMenu);
	glutAddMenuEntry("erase points", 13);
	glutAddMenuEntry("translate", 8);
	glutAddMenuEntry("rotate", 9);
	glutAddMenuEntry("scale", 10);
	glutAddMenuEntry("symemric", 11);
	glutAddMenuEntry("cut lines", 6);
	glutAddMenuEntry("fill polygons", 7);
	glutCreateMenu(mainMenu);      // �����˵�  
	glutAddSubMenu("draw", subMenu1);
	glutAddSubMenu("operations", subMenu3);
	glutAddSubMenu("color", subMenu2);
	glutAddMenuEntry("clear", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutPassiveMotionFunc(myPassiveMotion);
	glutDisplayFunc(myDisplay);
	glutMainLoop();
}