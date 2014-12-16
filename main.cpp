//Taken from http://www.cityintherain.com/greensquare.html
#include <windows.h>
#include <scrnsave.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "resource.h"
#include "MerryChristmas2011.h"

#pragma comment(lib, "scrnsave.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "comctl32.lib")

#define TIMER 1

void InitGL(HWND hwnd, HDC &hdc, HGLRC &hrc);
void CloseGL(HWND hwnd, HDC hdc, HGLRC hrc);
void GetConfig();               
void WriteConfig(HWND hdlg);
void SetupAnimation(int width, int height);
void CleanupAnimation();
void OnTimer(HDC hdc);

int width, height;

MerryChristmas2011 greeting(800,600,32,"Christmas2011ScreenSaver.log");

// Screen Saver Procedure
LRESULT WINAPI ScreenSaverProc(HWND hwnd, UINT msg, 
	WPARAM wparam, LPARAM lparam)
{
	static HDC hdc;
	static HGLRC hrc;
	static RECT rect;

	switch ( msg ) {

	case WM_CREATE: 
		// get window dimensions
		GetClientRect( hwnd, &rect );
		width = rect.right;         
		height = rect.bottom;

		//get configuration from registry
		GetConfig();

		// setup OpenGL, then animation
		InitGL( hwnd, hdc, hrc );
		SetupAnimation(width, height);

		//set timer to tick every 10 ms
		SetTimer( hwnd, TIMER, 10, NULL );
		return 0;

	case WM_DESTROY:
		KillTimer( hwnd, TIMER );
		CleanupAnimation();
		CloseGL( hwnd, hdc, hrc );
		return 0;

	case WM_TIMER:
		OnTimer(hdc);       //animate!      
		return 0;                           

	}

	return DefScreenSaverProc(hwnd, msg, wparam, lparam );

}

//bool bTumble = true;


BOOL WINAPI
	ScreenSaverConfigureDialog(HWND hDlg, UINT msg, 
	WPARAM wparam, LPARAM lparam)
{
	return FALSE;
	/*
	//InitCommonControls();  
	//would need this for slider bars or other common controls

	HWND aCheck;

	switch ( message ) 
	{

	case WM_INITDIALOG:
		LoadString(hMainInstance, IDS_DESCRIPTION, szAppName, 40);

		GetConfig();

		aCheck = GetDlgItem( hDlg, IDC_TUMBLE );
		SendMessage( aCheck, BM_SETCHECK, 
			bTumble ? BST_CHECKED : BST_UNCHECKED, 0 );

		return TRUE;

	case WM_COMMAND:
		switch( LOWORD( wParam ) ) 
		{ 

		case IDC_TUMBLE:
			bTumble = (IsDlgButtonChecked( hDlg, IDC_TUMBLE ) == BST_CHECKED);
			return TRUE;

			//cases for other controls would go here

		case IDOK:
			WriteConfig(hDlg);      //get info from controls
			EndDialog( hDlg, LOWORD( wParam ) == IDOK ); 
			return TRUE; 

		case IDCANCEL: 
			EndDialog( hDlg, LOWORD( wParam ) == IDOK ); 
			return TRUE;   
		}

	}     //end command switch

	return FALSE;
	*/
}



// needed for SCRNSAVE.LIB
BOOL WINAPI RegisterDialogClasses(HANDLE hInst)
{
	return TRUE;
}


/////////////////////////////////////////////////
////   INFRASTRUCTURE ENDS, SPECIFICS BEGIN   ///
////                                          ///
////    In a more complex scr, I'd put all    ///
////     the following into other files.      ///
/////////////////////////////////////////////////


// Initialize OpenGL
static void InitGL(HWND hWnd, HDC & hDC, HGLRC & hRC)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory( &pfd, sizeof pfd );
	pfd.nSize = sizeof pfd;
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;

	hDC = GetDC( hWnd );

	int i = ChoosePixelFormat( hDC, &pfd );  
	SetPixelFormat( hDC, i, &pfd );

	hRC = wglCreateContext( hDC );
	wglMakeCurrent( hDC, hRC );
}

// Shut down OpenGL
static void CloseGL(HWND hWnd, HDC hDC, HGLRC hRC)
{
	wglMakeCurrent( NULL, NULL );
	wglDeleteContext( hRC );

	ReleaseDC( hWnd, hDC );
}


void SetupAnimation(int Width, int Height)
{
	greeting.Initialize(width, height, 32);
	//greeting.ChangeResolution(width,height,32);
	/*
	//window resizing stuff
	glViewport(0, 0, (GLsizei) Width, (GLsizei) Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-300, 300, -240, 240, 25, 75);  
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(0.0, 0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//camera xyz, the xyz to look at, and the up vector (+y is up)

	//background
	glClearColor(0.0, 0.0, 0.0, 0.0); //0.0s is black


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
	glShadeModel(GL_SMOOTH); 

	//no need to initialize any objects
	//but this is where I'd do it

	glColor3f(0.1f, 1.0f, 0.3f); //green
	*/
}

static GLfloat spin=0;   //a global to keep track of the square's spinning


void OnTimer(HDC hDC) //increment and display
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	greeting.DrawScene();
	/*
	spin = spin + 1; 

	glPushMatrix();
	glRotatef(spin, 0.0, 0.0, 1.0);

	glPushMatrix();

	glTranslatef(150, 0, 0);

	if(bTumble)
		glRotatef(spin * -3.0, 0.0, 0.0, 1.0); 
	else
		glRotatef(spin * -1.0, 0.0, 0.0, 1.0);  

	//draw the square (rotated to be a diamond)

	float xvals[] = {-30.0, 0.0, 30.0, 0.0};
	float yvals[] = {0.0, -30.0, 0.0, 30.0};

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_POLYGON);
	for (int i=0; i < 4; i++)
		glVertex2f(xvals[i], yvals[i]);
	glEnd();

	glPopMatrix();
	*/
	glFlush();
	SwapBuffers(hDC);
	glPopMatrix();
}

void CleanupAnimation()
{
	//didn't create any objects, so no need to clean them up
}



/////////   REGISTRY ACCESS FUNCTIONS     ///////////

void GetConfig()
{
	/*
	HKEY key;
	//DWORD lpdw;

	if (RegOpenKeyEx( HKEY_CURRENT_USER,"Software\\GreenSquare", 0, KEY_QUERY_VALUE, &key) == ERROR_SUCCESS) 
	{
		DWORD dsize = sizeof(bTumble);
		DWORD dwtype =  0;

		RegQueryValueEx(key,"Tumble", NULL, &dwtype, 
			(BYTE*)&bTumble, &dsize);


		//Finished with key
		RegCloseKey(key);
	}
	else //key isn't there yet--set defaults
	{
		bTumble = true;
	}
	*/

}

void WriteConfig(HWND hDlg)
{
	/*
	HKEY key;
	DWORD lpdw;

	if (RegCreateKeyEx( HKEY_CURRENT_USER,
		"Software\\GreenSquare", //lpctstr
		0,                      //reserved
		"",                     //ptr to null-term string specifying the object type of this key
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&key,
		&lpdw) == ERROR_SUCCESS)

	{
		RegSetValueEx(key,"Tumble", 0, REG_DWORD, 
			(BYTE*)&bTumble, sizeof(bTumble));

		//Finished with keys
		RegCloseKey(key);
	}
	*/
}
