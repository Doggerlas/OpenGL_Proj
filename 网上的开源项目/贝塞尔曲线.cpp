//https://blog.csdn.net/HANZY72/article/details/109555198
#include <GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<cmath>
using namespace std;

// hide the console window
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#define WIDTH 900.f
#define HEIGHT 600.f
#define T 1


struct Color {
	double r, g, b;
	Color() { }
	Color(double x, double y, double z)
		:r(x), g(y), b(z) { }
} YELLOW(1, 1, 0), RED(1, 0, 0), BLACK(0.3, 0.3, 0.3);

struct Point {
	double x, y;
	Point() { }
	Point(double a, double b)
		:x(a), y(b) { }
	Point(const Point &p) {
		this->x = p.x;
		this->y = p.y;
	}

	// scaler * Point
	Point operator* (const double &t) {
		return Point(t*this->x, t*this->y);
	}
	// Point + Point
	Point operator+ (const Point &p) {
		return Point(this->x + p.x, this->y + p.y);
	}
};

vector<Point> points; // control points
vector<Point>::iterator moveIter, insertIter;
vector<Point> moving_points;  // moving points(in order to draw dynamic)
vector<Point> bezier_points;  // bezier curve points
bool MOVE = false, DEL = false, INS = false;

// To choose the end-point: dis<=10
bool close_to(double x1, double y1, double x2, double y2) {
	// calculate the distance to judge whether chhoose the end-point or not
	double dis = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	if (dis <= 10) return true;
	return false;
}

// draw controls
void drawControls(Color c) {
	int beginMode[] = { GL_POINTS ,GL_LINE_STRIP };
	for (int mode = 0; mode < 2; mode++) {
		// set points size
		if (mode == 0) glPointSize(9);
		// draw points and lines
		glBegin(beginMode[mode]);
		glColor3f(c.r, c.g, c.b);
		for (auto iter = points.begin(); iter != points.end(); iter++) {
			double xpos, ypos;
			if (MOVE && !moving_points.empty() && iter == moveIter) {
				xpos = moving_points[moving_points.size() - 1].x;
				ypos = moving_points[moving_points.size() - 1].y;
			}
			else xpos = iter->x, ypos = iter->y;

			xpos = (xpos - WIDTH / 2) / WIDTH * 2;
			ypos = (HEIGHT / 2 - ypos) / HEIGHT * 2;
			glVertex2f(xpos, ypos);
		}
		glEnd();
	}
}
// draw bezier curve
void drawCurve(Color c) {
	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(c.r, c.g, c.b);
	for (auto p : bezier_points) {
		double x = (p.x - WIDTH / 2) / WIDTH * 2;
		double y = (HEIGHT / 2 - p.y) / HEIGHT * 2;
		glVertex2f(x, y);
	}
	glEnd();
}


// de Casteliau algorithm
void deCasteliau(double t) {
	vector<Point> deCas = points;
	for (int k = 1; k < points.size(); k++)  // each layer
		for (int i = 0; i < points.size() - k; i++)  // each point in each layer
			deCas[i] = (deCas[i] * (1 - t)) + (deCas[i + 1] * t);
	if (!deCas.empty())
		bezier_points.push_back(deCas[0]);
}
void bezier() {
	bezier_points.clear();
	for (double t = 0; t < T; t += 0.0001)
		deCasteliau(t);
}

// move
void leftButtonOP(GLFWwindow *window, double x, double y) {
	for (auto iter = points.begin(); !DEL && !MOVE && iter != points.end(); iter++) {
		if (close_to(iter->x, iter->y, x, y)) {
			// move point
			MOVE = true;
			moveIter = iter;
			break;
		}
	}
	if (MOVE)
		moving_points.emplace_back(x, y);
}

// cursor pos callback  -> to show dynamic move
void cursorPosCallback(GLFWwindow *window, double x, double y) {
	// move or delete point
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1) {
		leftButtonOP(window, x, y);
	}
}
// mouse button callback
void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	// delete point
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		if (DEL) {
			for (auto iter = points.begin(); iter != points.end(); iter++) {
				if (close_to(iter->x, iter->y, xpos, ypos)) {
					points.erase(iter);
					break;
				}
			}
			// recompute bezier curve
			bezier();
		}
	}
	// move or add points
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		// modify points
		if (MOVE) {
			moveIter->x = xpos;
			moveIter->y = ypos;
		}
		// add point
		else if (!MOVE && !DEL)
			points.emplace_back(xpos, ypos);
		// recompute bezier curve
		bezier();
		MOVE = false;
		moving_points.clear();
	}
	// insert node
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		for (insertIter = points.begin(); insertIter != points.end(); insertIter++) {
			if (close_to(xpos, ypos, insertIter->x, insertIter->y)) {
				INS = true;
				break;
			}
		}
	}
	else if (INS && button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
		points.insert(insertIter, Point(xpos, ypos));
		INS = false;
		bezier();
	}
}
// key callback
void keyCallback(GLFWwindow *window, int key, int sancode, int action, int mods) {
	// delete points
	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		DEL = true;
	}
	else if (key == GLFW_KEY_DELETE && action == GLFW_RELEASE) {
		DEL = false;
	}
	// erase the control points
	else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
		points.clear();
		moving_points.clear();
		MOVE = false;
		DEL = false;
	}
	// clear the whole window
	else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		points.clear();
		moving_points.clear();
		bezier_points.clear();
		MOVE = false;
		DEL = false;
	}
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Bezier Curve", NULL, NULL);
	glfwSetWindowPos(window, 600, 200);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// mouse
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, cursorPosCallback);

		// key board
		glfwSetKeyCallback(window, keyCallback);

		// draw here
		drawControls(YELLOW);

		if (!MOVE) {
			// bezier
			drawCurve(RED);
		}


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

