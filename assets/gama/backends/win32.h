#ifndef GAMA_WIN32_H_INCLUDED
#define GAMA_WIN32_H_INCLUDED

#include "../app.h"
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void EnableOpenGL(HWND hwnd, HDC *, HGLRC *);
void DisableOpenGL(HWND, HDC, HGLRC);

void init(App *);
void create();
void update(float);
void render();
void gama_click(App *, MouseClickEvent *);

App *gama;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  gama = GamaCreateApp();
  init(gama);
  WNDCLASSEX wcex;
  HWND hwnd;
  HDC hDC;
  HGLRC hRC;
  MSG msg;
  BOOL bQuit = FALSE;

  /* register window class */
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_OWNDC;
  wcex.lpfnWndProc = WindowProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = "GLSample";
  wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wcex))
    return 0;

  /* create main window */
  if (gama->width != gama->height) {
    printf("Non sqare windows not yet supported");
    exit(500);
  }
  hwnd = CreateWindowEx(0, "GLSample", gama->title, WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, gama->width, gama->height,
                        NULL, NULL, hInstance, NULL);

  ShowWindow(hwnd, nCmdShow);

  /* enable OpenGL for the window */
  EnableOpenGL(hwnd, &hDC, &hRC);
  create();

  float ltime = clock();

  /* program main loop */
  while (!bQuit) {
    /* check for messages */
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      /* handle or dispatch messages */
      if (msg.message == WM_QUIT) {
        bQuit = TRUE;
      } else {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    } else {
      /* OpenGL animation code goes here */
      float ntime = clock();
      update((ntime - ltime) / 1000.0f);
      ltime = ntime;
      glClear(GL_COLOR_BUFFER_BIT);

      glPushMatrix();
      // glRotatef(theta, 0.0f, 0.0f, 1.0f);

      render();

      glPopMatrix();

      SwapBuffers(hDC);
      Sleep(1);
    }
  }

  /* shutdown OpenGL */
  DisableOpenGL(hwnd, hDC, hRC);

  /* destroy the window explicitly */
  DestroyWindow(hwnd);

  return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
  case WM_CLOSE:
    PostQuitMessage(0);
    break;

  case WM_DESTROY:
    return 0;

  case WM_KEYDOWN: {
    switch (wParam) {
    case VK_ESCAPE:
      PostQuitMessage(0);

      break;
    }
  } break;

  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }

  return 0;
}

void EnableOpenGL(HWND hwnd, HDC *hDC, HGLRC *hRC) {
  PIXELFORMATDESCRIPTOR pfd;

  int iFormat;

  /* get the device context (DC) */
  *hDC = GetDC(hwnd);

  /* set the pixel format for the DC */
  ZeroMemory(&pfd, sizeof(pfd));

  pfd.nSize = sizeof(pfd);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 24;
  pfd.cDepthBits = 16;
  pfd.iLayerType = PFD_MAIN_PLANE;

  iFormat = ChoosePixelFormat(*hDC, &pfd);

  SetPixelFormat(*hDC, iFormat, &pfd);

  /* create and enable the render context (RC) */
  *hRC = wglCreateContext(*hDC);

  wglMakeCurrent(*hDC, *hRC);
}

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC) {
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(hRC);
  ReleaseDC(hwnd, hDC);
}

#endif // GAMA_WIN32_H_INCLUDED
