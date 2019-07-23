#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")

#define WINDOW_TITLE "OpenGL Window"

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);
void dawQuad(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ);
void drawPencil();

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);

}
//--------------------------------------------------------------------

bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	drawPencil();

}
//--------------------------------------------------------------------
void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks) {
	GLUquadricObj *cylinder = NULL;
	cylinder = gluNewQuadric();
	gluQuadricDrawStyle(cylinder, GLU_FILL);
	gluCylinder(cylinder, baseRadius, topRadius, height, slices, stacks);
	gluDeleteQuadric(cylinder);
}
void drawCone(double baseRadius, double height, int slices, int stacks) {
	GLUquadricObj *cone = NULL;
	cone = gluNewQuadric();
	gluQuadricDrawStyle(cone, GLU_LINE);
	gluCylinder(cone, baseRadius, 0, height, slices, stacks);
	gluDeleteQuadric(cone);
}
void dawQuad(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ) {
	glBegin(GL_QUADS);
	//Face 1
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(topLeftX, topLeftY, topLeftZ);
	glVertex3f(botLeftX, botLeftY, botLeftZ);
	glVertex3f(botRightX, botRightY, botRightZ);
	glVertex3f(topRightX, topRightY, topRightZ);
	//Face 2
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(topLeftX, topLeftY, topLeftZ);
	glVertex3f(botLeftX, botLeftY, botLeftZ);
	glVertex3f(-botLeftX, botLeftY, botLeftZ);
	glVertex3f(-topLeftX, topLeftY, topLeftZ);
	//Face 3
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(-topLeftX, topLeftY, topLeftZ);
	glVertex3f(-botLeftX, botLeftY, botLeftZ);
	glVertex3f(-botRightX, botRightY, botRightZ);
	glVertex3f(-topRightX, topRightY, topRightZ);
	//Face 4
	glColor3f(1.0, 1.0, 0.0);
	glVertex3f(-topRightX, topRightY, topRightZ);
	glVertex3f(-botRightX, botRightY, botRightZ);
	glVertex3f(botRightX, botRightY, botRightZ);
	glVertex3f(topRightX, topRightY, topRightZ);
	//Face 5
	glColor3f(1.0, 0.0, 1.0);
	glVertex3f(topRightX, topRightY, topRightZ);
	glVertex3f(topLeftX, topLeftY, topLeftZ);
	glVertex3f(-topLeftX, topLeftY, topLeftZ);
	glVertex3f(-topRightX, topRightY, topRightZ);


	glEnd();
}
void drawLine(float lineWidth, float x1, float y1, float x2, float y2) {
	glLineWidth(lineWidth);
	glBegin(GL_LINE);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void drawPencil() {
	double baseRadius = 10;
	double topRadius = 10;
	double cylinderHeight = 350;
	double coneHeight = 20;
	int slices = 30;
	int stacks = 30;

	glPushMatrix();
		glTranslatef(0, 350, 0);
		glRotatef(90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, 350, 0);
		glRotatef(-90, 1, 0, 0);
		drawCone(baseRadius, coneHeight, slices, stacks);
	glPopMatrix();
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-500, 500, -500, 500, -500, 500);


	//gluPerspective(60, 1, 0.1, 1000);

	//glMatrixMode(GL_MODELVIEW);
	//glTranslatef(0.0, 0.0, -200);

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------