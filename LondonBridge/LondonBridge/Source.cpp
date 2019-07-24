#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include<math.h>
#include<iostream>

#pragma comment (lib, "OpenGL32.lib")
#pragma comment (lib, "GLU32.lib")
#pragma warning(disable:4996)

#define WINDOW_TITLE "OpenGL Window"

void drawCylinder(double baseRadius, double topRadius, double height, int slices, int stacks);
void drawCuboid(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ);
void drawCurve();
void drawPencil();
void drawTowerBlock();
void drawRoadsidePoles();

int num_texture = -1;

GLuint texture = 0;
BITMAP BMP;
HBITMAP hBMP = NULL;

GLuint texRailing, texRoofCube, texLinkBridge, texRoad, texTower, texTowerBase;



void LoadBitmapImage(const char *filename) {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL), filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	GetObject(hBMP, sizeof(BMP), &BMP);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
}

void endTexture() {
	glDisable(GL_TEXTURE_2D);
	DeleteObject(hBMP);
	glDeleteTextures(1, &texture);
}

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

	//textureFunction();
	
	glRotatef(0.05, 1, 1, 1);

	glPushMatrix();
		glTranslatef(-250, 0, 0);
		drawTowerBlock();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(250, 0, 0);
		drawTowerBlock();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(300, 0, -50);
		drawRoadsidePoles();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(300, 0, 50);
		drawRoadsidePoles();
	glPopMatrix();


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
void drawCuboid(float topLeftX, float topLeftY, float topLeftZ, float topRightX, float topRightY, float topRightZ, float botLeftX, float botLeftY, float botLeftZ, float botRightX, float botRightY, float botRightZ) {
	glBegin(GL_QUADS);
	//Face 1
	glTexCoord2f(0, 1); 
		glVertex3f(topLeftX, topLeftY, topLeftZ);
	glTexCoord2f(0, 0);
		glVertex3f(botLeftX, botLeftY, botLeftZ);
	glTexCoord2f(1, 0);
		glVertex3f(botRightX, botRightY, botRightZ);
	glTexCoord2f(1, 1);
		glVertex3f(topRightX, topRightY, topRightZ);

	//Face 2
	glVertex3f(topLeftX, topLeftY, topLeftZ);
	glVertex3f(botLeftX, botLeftY, botLeftZ);
	glVertex3f(-botLeftX, botLeftY, botLeftZ);
	glVertex3f(-topLeftX, topLeftY, topLeftZ);
	//Face 3
	glVertex3f(-topLeftX, topLeftY, topLeftZ);
	glVertex3f(-botLeftX, botLeftY, botLeftZ);
	glVertex3f(-botRightX, botRightY, botRightZ);
	glVertex3f(-topRightX, topRightY, topRightZ);
	//Face 4
	glVertex3f(-topRightX, topRightY, topRightZ);
	glVertex3f(-botRightX, botRightY, botRightZ);
	glVertex3f(botRightX, botRightY, botRightZ);
	glVertex3f(topRightX, topRightY, topRightZ);
	//Face 5
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
void drawCurve() {
	float radiusSmile = 400;
	float twoPI = 2 * 3.142;

	glPointSize(10.0);
	glBegin(GL_POINTS);
	glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
	for (float i = twoPI / 4; i <= 3 * twoPI / 4; i += 0.001)
	{
		glVertex2f((sin(i)*radiusSmile), (cos(i)*radiusSmile) + -0.1);
	}
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

void drawTowerBlock() {

		
		//tower
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			LoadBitmapImage("images/towerwaterface.bmp");
			drawCuboid(-50, 325, 50, -50, 325, -50, -50, 0, 50, -50, 0, -50);
			endTexture();
		glPopMatrix();
		//tower dome
		drawCuboid(-10, 370, 10, -10, 370, -10, -50, 325, 50, -50, 325, -50);
		//tower roof cube front
		drawCuboid(-25, 350, 50, -20, 350, -50, -25, 325, 50, -25, 325, -50);
		//tower roof cube side
		drawCuboid(-50, 350, 25, -50, 350, -25, -50, 325, 25, -50, 325, -25);

		//pencil top left
		glPushMatrix();
			glTranslatef(-50, 0, -50);
			drawPencil();
		glPopMatrix();

		//pencil top right
		glPushMatrix();
			glTranslatef(-50, 0, 50);
			drawPencil();
		glPopMatrix();

		//pencil bottom left
		glPushMatrix();
			glTranslatef(50, 0, -50);
			drawPencil();
		glPopMatrix();

		//pencil bottom left
		glPushMatrix();
			glTranslatef(50, 0, 50);
			drawPencil();
		glPopMatrix();

		//tower base
		drawCuboid(-50, 0, 50, -50, 0, -50, -50, -100, 50, -50, -100, -50);

		//tower base cyclinder platform front
		glPushMatrix();
			glTranslatef(0, 0, -50);
			glRotatef(90, 1, 0, 0);
			drawCylinder(50, 50, 100, 30, 30);
		glPopMatrix();

		//tower base cyclinder platform back
		glPushMatrix();
			glTranslatef(0, 0, 50);
			glRotatef(90, 1, 0, 0);
			drawCylinder(50, 50, 100, 30, 30);
		glPopMatrix();

}

void drawRoadsidePoles() {
	double baseRadius = 10;
	double topRadius = 10;
	double cylinderHeight = 200;
	int slices = 30;
	int stacks = 30;

	glPushMatrix();
		glTranslatef(50, 0, 0);
		glRotatef(-90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(100, 0, 0);
		glRotatef(-90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight-50, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(150, 0, 0);
		glRotatef(-90, 1, 0, 0);
		drawCylinder(baseRadius, topRadius, cylinderHeight-100, slices, stacks);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(400, 400, 0);
		drawCurve();
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