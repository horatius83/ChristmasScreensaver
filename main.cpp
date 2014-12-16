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
}

static GLfloat spin=0;   //a global to keep track of the square's spinning


void OnTimer(HDC hDC) //increment and display
{
	greeting.DrawScene();
	
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
