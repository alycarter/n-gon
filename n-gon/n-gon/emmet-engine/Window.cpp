#include "Controls.h"
#include "Window.h"

//message handling function
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

Window::Window(void)
{
	width = 0;
	height = 0;
	handle = NULL;
	quitRequested = false;
}

Window::~Window(void)
{
}

//create the window ready to be shown
void Window::create(wchar_t * name, int x, int y, int widthIn, int heightIn, HINSTANCE hInstance)
{
	//set up window size variables
	width = widthIn;
	height = heightIn;
	// this struct holds information for the window class
	WNDCLASSEX wc;
	// clear out the window class for use
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	// fill in the struct with the needed information
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wchar_t className[] =L"WindowClass1"; 
	wc.lpszClassName = className;
	// register the window class
	RegisterClassEx(&wc);
	// create the window and use the result as the handle
	handle = CreateWindowEx(NULL, className, name, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, hInstance, NULL);
}

//make the window visible
void Window::show()
{
 	ShowWindow(handle, SW_SHOW);
}

//hide the window
void Window::hide()
{
	ShowWindow(handle, SW_HIDE);
}

//read any messages from the os. will handle any key inputs 
void Window::readMessages(Controls * controls)
{
	MSG msg;
	// using peek message rather than get message because we want to stop reading messages if none are available
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		switch(msg.message)
		{
		//if a quit message has been sent
		case WM_QUIT:
			//set our quit flag to true
			quitRequested = true;
			break;
		case WM_KEYDOWN:
			controls->handleKeyPressed(msg.wParam);
			break;
		case WM_KEYUP:
			controls->handleKeyRelease(msg.wParam);
			break;
		default:
			break;
		}
		// translate keystroke messages into the right format
		TranslateMessage(&msg);
		// send the message to the WindowProc function
		DispatchMessage(&msg);
    }
}

bool Window::isQuitRequested()
{
	return quitRequested;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // if the window needs to destroy then post a quit message
    if(message == WM_DESTROY)
    {
		PostQuitMessage(0);
		return 0;
	}
    // return any other type of message
    return DefWindowProc (hWnd, message, wParam, lParam);
}

int Window::getWidth()
{
	return width;
}

int Window::getHeight()
{
	return height;
}

HWND Window::getWindowHandle()
{
	return handle;
}