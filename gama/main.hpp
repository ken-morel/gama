#include <windows.h>
//subliminal:headers
int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow
);
LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT uMsg,
    WPARAM wParam,
    LPARAM lParam
);
void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC);
void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC);
void jump();
int render(Gama *gama);
//endsubliminal:headers
int load(Gama* gama);
int init(Gama* gama);
int update(Gama* gama, double theta);
int render(Gama* gama);

Gama* gama;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    gama = new Gama();
    load(gama);
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
    init(gama);
    hwnd = CreateWindowEx(
        0,
        "GLSample",
        gama -> title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        gama -> width,
        gama -> height,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    ShowWindow(hwnd, nCmdShow);

    /* enable OpenGL for the window */
    EnableOpenGL(hwnd, &hDC, &hRC);
    double last_t = (double)clock() / 1000;
    double ct;
    /* program main loop */
    while (!bQuit)
    {
        /* check for messages */
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            /* handle or dispatch messages */
            if (msg.message == WM_QUIT)
            {
                bQuit = TRUE;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            /* OpenGL animation code goes here */
            ct = (double)clock() / 1000;
            update(gama, ct - last_t);
            gama->add_fps_interval(ct-last_t);
            last_t = ct;


            //wglMakeCurrent(hDC, hrc);
            float* clear = gama->clearColor->array();
            glClearColor(clear[0], clear[1], clear[2], 0.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glMatrixMode(GL_MODELVIEW);
            glEnable(GL_DEPTH_TEST);
                glPushMatrix();


                    //Enable the first light and the lighting mode

                    if(gama->lights.size() > 0) {
                        for(auto light: gama -> lights)
                            light->enable();
                        glEnable(GL_LIGHTING);
                    }

                        render(gama);

                    if(gama->lights.size() > 0) {
                        for(auto light: gama -> lights)
                            light->disable();
                        glDisable(GL_LIGHTING);
                    }

                glPopMatrix();

            glDisable(GL_DEPTH_TEST);

            SwapBuffers(hDC);
        }
    }

    /* shutdown OpenGL */
    DisableOpenGL(hwnd, hDC, hRC);

    /* destroy the window explicitly */
    DestroyWindow(hwnd);

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE:
            PostQuitMessage(0);
        break;

        case WM_DESTROY:
            return 0;

        case WM_KEYDOWN:
            switch (wParam)
            {
                case VK_UP:
                    gama->keyboardHandler->on_arrowkeydown(ArrowKey::UP);
                    break;
                case VK_DOWN:
                    gama->keyboardHandler->on_arrowkeydown(ArrowKey::DOWN);
                    break;
                case VK_LEFT:
                    gama->keyboardHandler->on_arrowkeydown(ArrowKey::LEFT);
                    break;
                case VK_RIGHT:
                    gama->keyboardHandler->on_arrowkeydown(ArrowKey::RIGHT);
                    break;
            }
        break;

        //PostQuitMessage(0);
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

void EnableOpenGL(HWND hwnd, HDC* hDC, HGLRC* hRC)
{
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

void DisableOpenGL(HWND hwnd, HDC hDC, HGLRC hRC)
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hwnd, hDC);
}
















































/*
#define GRAVITY -20

#include "gama/gama.hpp"
#include "gama/Shape.hpp"
#include "gama/Bitmap.hpp"
#include <iostream>
//glBitmap( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap )
Cube* ground;
Sphere* ball;
Cylinder *pipete;
Bitmap* grass;


//subliminal:headerse
//endsubliminal:headers

void jump()
{
    ball->pos->vel->y = 5;
}

class KeyHandler: public KeyBoardEventHandler {
public:
    void on_arrowkeydown(ArrowKey key) {
        switch(key) {
        case ArrowKey::UP:
            jump();
            break;
        default:
            break;
        }
    };
};


int load(Gama* gama)//
{
    gama->keyboardHandler = new KeyHandler();
    return 1;
}

int init(Gama* gama)//
{
    gluPerspective(2, 5, 0.5f, 10.0f);
    grass = new Bitmap("robot.png");
    gama->title = "Jumpy ball";
    gama->width = 700;
    gama->height = 700;
    pipete = new Cylinder(
        new Vector(0, 0, 0),
        new Color(BLUE),
        1,
        1
    );

    ground = new Cube(new Vector(0, -0.9, 0), new Vector(1, 0, 1));
    ground->color->set(DARKGREEN);
    ground->rotation->x = 350;
    gama->clearColor->set(GRAY);

    ball = new Sphere(new Vector(-0.5, 0, 0), new Color(ORANGE), 0.5);
    ball->tesselation = 1000;
    //ball->pos->acc->y = GRAVITY;


    gama->lights.push_back(new Light(
        0,                  // Light id from 0-7
        LightType::DIFFUSE, // LightType enum: DIFFUSE|AMBIENT|SPECULAR
        255, 255, 255,      // light color
        0.01f,              // intensity
        0.0, 0.0, -1.0      // light position
    ));
    ball->rotation->vel->x = 0.1;
    ball->rotation->vel->y = 0.1;
    return 1;
}
int update(Gama* gama, double theta)//
{
    if((ball->pos->y-ball->radius+ball->radius/10) < ground->pos->y) {
        if(ball->pos->vel->y < 0)
            ball->pos->vel->y *= -0.6;
    }
    ball->update(theta);
    ground->update(theta);

    return 1;
}

int render(Gama *gama)
{
    //ground->render();
    //ball->render();
    //grass -> render();
    //pipete -> render();
    std::cout << gama->fps() << std::endl;
    return 1;
}
*/
