//https://blog.csdn.net/HANZY72/article/details/109555179
#include <GLFW/glfw3.h>
#include<iostream>
#include<vector>
#include<cmath>

#define WIDTH 960.f
#define HEIGHT 600.f
using namespace std;

// hide the console window
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )


struct Color {
	float r, g, b;
	Color() { }
	Color(float x, float y, float z)
		:r(x), g(y), b(z) { }
} YELLOW(1, 1, 0), RED(1, 0, 0);

struct Point {
	double x, y;
	Point() { }
	Point(double a, double b)
		:x(a), y(b) { }
};
vector<Point> points; // drawing points
vector<vector<Point> > Line;
bool Modifying = false;

// To choose the end-point: dis<=10
bool near(double x1, double y1, double x2, double y2) {
	// calculate the distance to judge whether chhoose the end-point or not
	double dis = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	if (dis <= 10) return true;
	return false;
}

// draw
void draw(double x, double y, Color color) {
	// transform the coordinates to the drawing: (0, 0) == (width/2, height/2)
	x = (x - WIDTH / 2) / WIDTH * 2;
	y = (HEIGHT / 2 - y) / HEIGHT * 2;
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(x, y);  // draw a pixel
	glEnd();
}
// use the window's coordinates: leftBottom(0,0), the drawing coordinates is different
void BresemhamLine(double x0, double y0, double x1, double y1, Color color) {
	double x, y, dx, dy;
	double k, e;
	dx = x1 - x0, dy = y1 - y0, k = dy / dx;
	e = -0.5, x = x0, y = y0;

	// |k| <= 1
	if (abs(k) <= 1) {
		for (int i = 0; i <= abs(dx); i++) {
			draw(x, HEIGHT - y, color);
			// direction control
			if (dx >= 0) x += 1;
			else x -= 1;
			e += abs(k);  // e = d - 0.5

			if (e >= 0) {
				// direction control
				if (dy >= 0) y += 1;
				else y -= 1;
				e -= 1;
			}
		}
	}
	// |k| > 1; change x and y
	else {
		for (int i = 0; i <= abs(dy); i++) {
			draw(x, HEIGHT - y, color);
			// direction control
			if (dy >= 0) y += 1;
			else y -= 1;
			e += abs(1 / k);

			if (e >= 0) {
				// direction control
				if (dx >= 0) x += 1;
				else x -= 1;
				e -= 1;
			}
		}
	}
}

// Display the lines 
void displayLine() {
	// display modifying line dynamically
	if (!points.empty()) {
		int s = points.size();
		double x1 = points[0].x, y1 = points[0].y;
		double x2 = points[s - 1].x, y2 = points[s - 1].y;
		// display the painting line
		BresemhamLine(x1, y1, x2, y2, RED);
	}

	// display the certain lines
	for (int i = 0; i < Line.size(); i++)
		BresemhamLine(Line[i][0].x, Line[i][0].y, Line[i][1].x, Line[i][1].y, YELLOW);
}

// mouse button callback
void getMouseButton(GLFWwindow *window, int button, int action, int mods) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		// if release: add line
		if (action == GLFW_RELEASE) {
			Modifying = false;  // stop modifying the end-point
			if (!points.empty()) {  // just finish drawing lines
				int s = points.size();
				// update the lines: keep old; add new
				vector<Point> tmp;
				tmp.push_back(points[0]);
				tmp.push_back(points[s - 1]);
				Line.push_back(tmp);

				points.clear();
			}
		}
	}
}

// Pressing left button to draw/modify a line with position of cursordynamically
void leftButtonOP(double x, double y) {
	int s = points.size();
	// transfer the coordinates
	y = HEIGHT - y;

	// move the line: when the line is existed and choose the end-points
	for (int i = 0; !Modifying && i < Line.size(); i++) {
		Point tmp;
		bool start_near = near(x, y, Line[i][0].x, Line[i][0].y);
		bool end_near = near(x, y, Line[i][1].x, Line[i][1].y);

		// move the start point
		if (!Modifying && start_near)
			tmp.x = Line[i][1].x, tmp.y = Line[i][1].y;
		// move the end point
		else if (!Modifying && end_near)
			tmp.x = Line[i][0].x, tmp.y = Line[i][0].y;

		if (start_near || end_near) { // can move: modify the coresponding point
			Line.erase(Line.begin() + i);  // earse the old line
			points.push_back(tmp);  // add the start point; not drawing, tmp is the first point
			break;
		}
	}

	// preserve the drawing points
	points.push_back(Point(x, y));
	Modifying = true;  // drawing new line or moving a line
}

// CursorPos callback
void cursorPosCallback(GLFWwindow *window, double x, double y) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == 1) {
		// press button to add or modify a line
		leftButtonOP(x, y);
	}
}

// keyboard callback
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
		// keep the clipped lines stay, erase the redundant parts
		Line.clear();
	}
}




int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Bresemham", NULL, NULL);
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
		glfwSetCursorPosCallback(window, cursorPosCallback);
		glfwSetMouseButtonCallback(window, getMouseButton);

		// keyboard
		glfwSetKeyCallback(window, keyCallback);

		// draw here
		displayLine();


		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


