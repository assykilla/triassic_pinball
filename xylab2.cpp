//modified by Xander Reyes:
//date:
//
//author: Gordon Griesel
//date: Spring 2022
//purpose: get openGL working on your personal computer
//
#include <iostream>
using namespace std;
#include <stdio.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include "fonts.h"
//some structures

class Global {
    public:
	int xres, yres;
	int n;
	Global();
} g;

const int MAX_PARTICLES = 5000;
class Box {
    public:
	float w;
	float h;
	float pos[2];
	float vel[2];
	unsigned char color[3];
	void set_color ( unsigned char col[3]) {
	    memcpy(color, col, sizeof(unsigned char) *3);

	}
	//float boxr=150.0f/255.0f, boxg=160.0f/255.0f, boxb=220.0f/255.0f;
	Box(){
	    w = 80.0f;
	    h = 15.0f;
	    pos[0] = 90;
	    pos[1] = 290;
	    vel[0] = 0.0f;
	    vel[1] = 0.0f;
	}
	Box(float wid, float hgt, int x, int y, float v0, float v1){
	    w = wid;
	    h = hgt;
	    pos[0] = x;
	    pos[1] = y;
	    vel[0] = v0;
	    vel[1] = v1;
	}
} box[5], particle[MAX_PARTICLES];

class Circle {
    public:
	float r, c[2], x, y;
	unsigned char color[3];
	void set_color ( unsigned char col[3]) {
	    memcpy(color, col, sizeof(unsigned char) *3);

	}
	Circle(){
	    r = 75.0f;
	    c[0] = 600;
	    c[1] = 25;
	}
} circle;

class X11_wrapper {
    private:
	Display *dpy;
	Window win;
	GLXContext glc;
    public:
	~X11_wrapper();
	X11_wrapper();
	void set_title();
	bool getXPending();
	XEvent getXNextEvent();
	void swapBuffers();
	void reshape_window(int width, int height);
	void check_resize(XEvent *e);
	void check_mouse(XEvent *e);
	int check_keys(XEvent *e);
} x11;

//Function prototypes
void init_opengl(void);
void physics(void);
void render(void);



//=====================================
// MAIN FUNCTION IS HERE
//=====================================
int main()
{
    init_opengl();
    //Main loop
    int done = 0;
    while (!done) {
	//Process external events.
	while (x11.getXPending()) {
	    XEvent e = x11.getXNextEvent();
	    x11.check_resize(&e);
	    x11.check_mouse(&e);
	    done = x11.check_keys(&e);
	}
	physics();
	render();
	x11.swapBuffers();
	//usleep(200);
    }
    cleanup_fonts();
    return 0;
}

Global::Global()
{
    xres = 650;
    yres = 450;
    n = 0;
}

X11_wrapper::~X11_wrapper()
{
    XDestroyWindow(dpy, win);
    XCloseDisplay(dpy);
}

X11_wrapper::X11_wrapper()
{
    GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
    int w = g.xres, h = g.yres;
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
	cout << "\n\tcannot connect to X server\n" << endl;
	exit(EXIT_FAILURE);
    }
    Window root = DefaultRootWindow(dpy);
    XVisualInfo *vi = glXChooseVisual(dpy, 0, att);
    if (vi == NULL) {
	cout << "\n\tno appropriate visual found\n" << endl;
	exit(EXIT_FAILURE);
    }  glPushMatrix();
    glColor3ub(255,255, 100);

    Colormap cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);
    XSetWindowAttributes swa;
    swa.colormap = cmap;
    swa.event_mask =
	ExposureMask | KeyPressMask | KeyReleaseMask |
	ButtonPress | ButtonReleaseMask |
	PointerMotionMask |
	StructureNotifyMask | SubstructureNotifyMask;
    win = XCreateWindow(dpy, root, 0, 0, w, h, 0, vi->depth,
	    InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
    set_title();
    glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);

    glXMakeCurrent(dpy, win, glc);
}

void X11_wrapper::set_title()
{
    //Set the window title bar.
    XMapWindow(dpy, win);
    XStoreName(dpy, win, "3350 Lab2");
}

bool X11_wrapper::getXPending()
{
    //See if there are pending events.
    return XPending(dpy);
}

XEvent X11_wrapper::getXNextEvent()
{
    //Get a pending event.
    XEvent e;
    XNextEvent(dpy, &e);
    return e;
}

void X11_wrapper::swapBuffers()
{
    glXSwapBuffers(dpy, win);
}

void X11_wrapper::reshape_window(int width, int height)
{
    //window has been resized.
    g.xres = width;
    g.yres = height;
    //
    glViewport(0, 0, (GLint)width, (GLint)height);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
}

void X11_wrapper::check_resize(XEvent *e)
{
    //The ConfigureNotify is sent by the
    //server if the window is resized.
    if (e->type != ConfigureNotify)
	return;
    XConfigureEvent xce = e->xconfigure;
    if (xce.width != g.xres || xce.height != g.yres) {
	//Window size did change.
	reshape_window(xce.width, xce.height);
    }
}
//-----------------------------------------------------------------------------
float rnd()
{
    float r = ((float)rand() / (float)RAND_MAX);
    return r;
}

void make_particle(int x, int y)
{
    if ( g.n < MAX_PARTICLES) {
	for (int j = 0; j < 5; j++) {
		if (y <= box[j].pos[1]+box[j].h && 
			x >= box[j].pos[0]-box[j].w &&
			x <= box[j].pos[0]+box[j].w &&
			y >= box[j].pos[1]-box[j].h) {
	    		return;	    
		}
    	}
	particle[g.n].w=4;
	particle[g.n].h=4;
	particle[g.n].pos[0] = x;
	particle[g.n].pos[1] = y;
	particle[g.n].vel[0] = rnd() * 0.2 - 0.1;
	particle[g.n].vel[1] = rnd() * 0.2 - 0.1;
	++g.n;
	
    }
}

void default_make_particle()
{
    if ( g.n < MAX_PARTICLES) {
	particle[g.n].w=4;
	particle[g.n].h=4;
	particle[g.n].pos[0] = 50;
	particle[g.n].pos[1] = 390;
	particle[g.n].vel[0] = rnd() * 0.2 - 0.1;
	particle[g.n].vel[1] = rnd() * 0.2 - 0.1;
	++g.n;
    }
}

void X11_wrapper::check_mouse(XEvent *e)
{
    static int savex = 0;
    static int savey = 0;

    //Weed out non-mouse events
    if (e->type != ButtonRelease &&
	    e->type != ButtonPress &&
	    e->type != MotionNotify) {
	//This is not a mouse event that we care about.
	return;
    }
    //
    if (e->type == ButtonRelease) {
	return;
    }
    if (e->type == ButtonPress) {
	if (e->xbutton.button==1) {
	    //Left button was pressed.
	    //int y = g.yres - e->xbutton.y;
	    make_particle(e->xbutton.x, g.yres - e->xbutton.y);
	    return;
	}
	if (e->xbutton.button==3) {
	    //Right button was pressed.
	    return;
	}
    }
    if (e->type == MotionNotify) {
	//The mouse moved!
	if (savex != e->xbutton.x || savey != e->xbutton.y) {
	    savex = e->xbutton.x;
	    savey = e->xbutton.y;
	    //Code placed here will execute whenever the mouse moves.
	    for (int i = 0; i < 5; i++) {	
		make_particle(e->xbutton.x, g.yres - e->xbutton.y);
	    }

	}
    }
}

int X11_wrapper::check_keys(XEvent *e)
{
    if (e->type != KeyPress && e->type != KeyRelease)
	return 0;
    int key = XLookupKeysym(&e->xkey, 0);
    if (e->type == KeyPress) {
	switch (key) {
	    case XK_1:
		//Key 1 was pressed
		break;
	    case XK_Escape:
		//Escape key was pressed
		return 1;
	}
    }
    return 0;
}

void init_opengl(void)
{
    //OpenGL initialization
    glViewport(0, 0, g.xres, g.yres);
    //Initialize matrices
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    //Set 2D mode (no perspective)
    glOrtho(0, g.xres, 0, g.yres, -1, 1);
    //Set the screen background color
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glEnable(GL_TEXTURE_2D);
    initialize_fonts();
    //set box color
    unsigned char c[3] = {25,200,25};
    for (int i=0; i < 5; i++) {
	box[i].set_color(c);
    }
}
float Gravity = 0.05;

void physics()
{

    for (int i = 0; i < g.n; i++) {
	particle[i].pos[0] += particle[i].vel[0];
	particle[i].pos[1] += particle[i].vel[1];
	particle[i].vel[1] -= Gravity;
	//check if particle went off screen
	if (particle[i].pos[1] < 0.0 || particle[i].pos[0] > g.xres) {
	    // optimizing this code below
	    // particle[i] = particle [g.n-1];
	    // g.n= g.n-1;
	    // optimized
	    particle[i] = particle [--g.n];
	}
	
	

	//check for collision between particles and boxes
	for (int j = 0; j < 5; j++) {
	    if (particle[i].pos[1] < box[j].pos[1]+box[j].h && 
		    particle[i].pos[0] > box[j].pos[0]-box[j].w &&
		    particle[i].pos[0] < box[j].pos[0]+box[j].w &&
		    particle[i].pos[1] > box[j].pos[1]-box[j].h)
	    {
		particle[i].vel[1] =  -particle[i].vel[1] * 0.3; 
		particle[i].vel[0] += 0.01; 
	    }

	}

	int dist,xd,yd;
	xd = particle[i].pos[0] - circle.c[0];
	yd = particle[i].pos[1] - circle.c[1];
	dist = sqrt((xd*xd)+(yd*yd));
	if (dist <= circle.r) {
	    if (particle[i].pos[0] < circle.c[0]) {
		particle[i].vel[1] =  -particle[i].vel[1] * 0.03; 
		particle[i].vel[0] = -dist * 0.02 ; 
	    } else {
		particle[i].vel[1] =  -particle[i].vel[1] * 0.03; 
		particle[i].vel[0] = dist * 0.02 ; 
	    }
	}
    }
}
void render()
{
    //

    float positionx = box[0].pos[0];
    float positiony = box[0].pos[1];
    Rect r[5];
    string waterfall[5] = {"Requirements", "Design", "Coding", "Testing", "Maintenance" };
    char text[100];
    glClear(GL_COLOR_BUFFER_BIT);
    //Draw box.
    for (int i = 0; i < 5; i++) {
	glPushMatrix();
	glColor3ubv(box[i].color);
	box[i].pos[0] = positionx;
	box[i].pos[1] = positiony;	
	glTranslatef(box[i].pos[0], box[i].pos[1], 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(-box[i].w, -box[i].h);
	glVertex2f(-box[i].w,  box[i].h);
	glVertex2f( box[i].w,  box[i].h);
	glVertex2f( box[i].w, -box[i].h);
	glEnd();
	glPopMatrix();
	r[i].bot = box[i].pos[1];
	r[i].left = box[i].pos[0];
	r[i].center = -5;
	strcpy(text, waterfall[i].c_str());
	ggprint8b(&r[i], 16, 0x00fffff0, text);
	positionx = positionx + 92;
	positiony = positiony - 45;
    }
    //Draw particle.
    for (int i = 0; i < g.n; i++) {
	glPushMatrix();
	glColor3ub(150, 160, 255);
	glTranslatef(particle[i].pos[0], particle[i].pos[1], 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(-particle[i].w, -particle[i].h);
	glVertex2f(-particle[i].w,  particle[i].h);
	glVertex2f( particle[i].w,  particle[i].h);
	glVertex2f( particle[i].w, -particle[i].h);
	glEnd();
	glPopMatrix();
    }
    default_make_particle(); // default way of making particles. 
			     //Draw Circle
    int n = 20; 
    double angle = 0.0;
    double inc = (2.0*3.14)/n;
    //glVertex2f(x, y);
    glColor3ub(255,255, 100);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n; i++) {
	circle.x = circle.r*cos(angle);
	circle.y = circle.r*sin(angle);
	glVertex2f(circle.x+circle.c[0],circle.y + circle.c[1]);
	angle += inc;
    }
    glEnd();

}






