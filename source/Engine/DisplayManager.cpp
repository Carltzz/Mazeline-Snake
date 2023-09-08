#include "DisplayManager.h"
#include "MazelineEngine.h"
#include "GLew/glew.h"
#ifdef _WIN32
#include "GLew/wglew.h"
#endif
#include <gl/GL.h>


using namespace Mazeline;

const char* g_titleBar = "Mazeline Engine";

KeyEventArgs g_keyEventArgs;
KeyInputEventArgs g_keyInputArgs;
KeyCommandEventArgs g_keyCmdArgs;
MouseEventArgs g_mouseEventArgs;
MouseMoveEventArgs g_mouseMouseArgs;
MouseScrollEventArgs g_mouseScrollArgs;

#ifdef _WIN32
#include <Windows.h>

bool g_windowClassRegistered = false;
WNDCLASSEX g_windowClass;
HWND g_hWnd = 0;
const char* g_className = "MazWindow";

HDC g_deviceContext = 0;
HGLRC g_glRenderContext = 0;

bool Windows_InitGL() {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), 1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, 8, 0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	
	GLint attribs[] =
	{
		// Here we ask for OpenGL 2.1
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 5,
		// Uncomment this for forward compatibility mode
		//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		// Uncomment this for Compatibility profile
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		// We are using Core profile here
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	g_deviceContext = GetDC(g_hWnd);
	int pixelFormat = ChoosePixelFormat(g_deviceContext, &pfd);
	SetPixelFormat(g_deviceContext, pixelFormat, &pfd);
	HGLRC tempContext = wglCreateContext(g_deviceContext);
	wglMakeCurrent(g_deviceContext, tempContext);

	GLenum result = glewInit();
	if (result != GLEW_OK) {
		MDebugLog("glewInit failed: %s", glewGetErrorString(result));
		return 0;
	}

	if (wglewIsSupported("WGL_ARB_create_context")) {
		g_glRenderContext = wglCreateContextAttribsARB(g_deviceContext, 0, attribs);
		wglMakeCurrent(0, 0);
		wglDeleteContext(tempContext);
		if (!g_glRenderContext || !wglMakeCurrent(g_deviceContext, g_glRenderContext)) {
			MDebugLog("Failed to create OpenGL Core Context");
			return 0;
		}
	}

	MDebugLog("OpenGL Version: %s", glGetString(GL_VERSION));
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	DisplayManager& disp = DisplayManager::GetInstance();
	InputManager& input = InputManager::GetInstance();

	switch (msg) {
	case WM_CLOSE:
		disp.RequestClose();
		break;
	case WM_KEYDOWN:
		g_keyEventArgs.Key = (MKey)MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
		input.RaiseKeyDownEvent(g_keyEventArgs);
		break;
	case WM_KEYUP:
		g_keyEventArgs.Key = (MKey)MapVirtualKey(wParam, MAPVK_VK_TO_CHAR);
		input.RaiseKeyPressEvent(g_keyEventArgs);
		break;
	case WM_CHAR:
		break;
	case WM_LBUTTONDOWN:
		break;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE: {
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);
		disp.UpdateMouse(mouseX, mouseY);
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
#endif

bool DisplayManager::Load() {
#ifdef _WIN32
	g_windowClass.cbSize = sizeof(WNDCLASSEX);
	g_windowClass.style = 0;
	g_windowClass.lpfnWndProc = WndProc;
	g_windowClass.cbClsExtra = 0;
	g_windowClass.cbWndExtra = 0;
	g_windowClass.hInstance = GetModuleHandle(NULL);
	g_windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	g_windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	g_windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	g_windowClass.lpszMenuName = NULL;
	g_windowClass.lpszClassName = g_className;
	g_windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&g_windowClass)) {
		MDebugLog("Failed to register the window class.");
		return false;
	}
#else
	return false;
#endif
	m_hasLoaded = true;
	g_windowClassRegistered = true;
	return true;
}

bool DisplayManager::Unload() {
#ifdef _WIN32
	// UNREGISTER THE CLASS
#endif
	return true;
}

DisplayManager::DisplayManager() {
	m_hasLoaded = false;
	m_closeRequested = false;
	m_shouldUpdate = false;
	m_papi = 0;
	m_inputManager = &InputManager::GetInstance();
}

DisplayManager& DisplayManager::GetInstance() {
	static DisplayManager displayManager;
	return displayManager;
}

bool DisplayManager::Create(DisplayParams& disp) {
	m_displayParams = disp;
#ifdef _WIN32
	if (!g_windowClassRegistered) {
		MDebugLog("Window class hasn't been registered yet...Try DisplayManager::Loaad()");
		return false;
	}

	if (!g_hWnd) {
		// Create the display
		g_hWnd = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			g_className,
			g_titleBar,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT,
			m_displayParams.Width,
			m_displayParams.Height,
			NULL, NULL,
			g_windowClass.hInstance, NULL
		);

		if (!g_hWnd) {
			HRESULT result = GetLastError();
			MDebugLog("Failed to create the display!");
			return false;
		}
		else {
			ShowWindow(g_hWnd, SW_SHOW);
			UpdateWindow(g_hWnd);
			m_shouldUpdate = true;
			if (Windows_InitGL()) {
				m_papi = new OpenGLApi();
				m_papi->Initialise();
				m_firstFrame = true;
				return true;
			}
			return false;
		}
	}
	else {
		// Change the display
		MDebugLog("I should probably implement this...");
		return false;
	}
#else
	return false;
#endif

	return false;
}

double DisplayManager::Update() {
	if (!m_shouldUpdate)
		return 0;
#ifdef _WIN32
	MSG msg;
	int result = -1;
	while (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if ((result = GetMessage(&msg, NULL, 0, 0)) != 0) {
			if (result == -1) {
				MDebugLog("GetMessage returned -1...Yikes!");
				Close();
				return -1;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	SwapBuffers(g_deviceContext);

	clock_t timeNow = clock();

	if (m_firstFrame) {
		m_lastRenderTime = timeNow;
		m_firstFrame = false;
	}

	double renderTime = (timeNow - m_lastRenderTime) * 1000 / CLOCKS_PER_SEC;
	m_lastRenderTime = timeNow;

	m_shouldUpdate = !(msg.message == WM_QUIT || result == 0);
	return renderTime;
#else
#endif
}

void DisplayManager::UpdateMouse(int mouseX, int mouseY) {
	if (m_mouseX == -1) {
		m_mouseX = mouseX;
		m_mouseY = mouseY;
	}

	int dX = mouseX - m_mouseX;
	int dY = mouseY - m_mouseY;
	m_mouseX = mouseX;
	m_mouseY = mouseY;

	MouseMoveEventArgs args;
	args.dX = dX;
	args.dY = dY;
	args.X = m_mouseX;
	args.Y = m_mouseY;

	InputManager::GetInstance().RaiseMouseMoveEvent(args);
}

bool DisplayManager::CloseRequested() {
	if (m_closeRequested) {
		m_closeRequested = false;
		return true;
	}
	return false;
}

void DisplayManager::Close() {
	if (m_papi)
		delete m_papi;
#ifdef _WIN32
	if (g_hWnd) {
		DestroyWindow(g_hWnd);
		PostQuitMessage(0);
	}
#else
#endif
}

void DisplayManager::RequestClose() {
	m_closeRequested = true;
}

GraphicsApi* DisplayManager::GetGraphics() {
	return m_papi;
}

bool DisplayManager::Destroy() {
	return true;
}