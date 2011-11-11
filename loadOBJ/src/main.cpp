#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>

#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header File For sleeping.

#include "objload.h"


/* ASCII code for the escape key. */
#define ESCAPE 27

/* The number of our GLUT window */
int window; 
std::string fName="cube.obj";

/*
struct Triangle {
       int v1;
       int v2;
       int v3;
};

struct Vertex {
      float x;
      float y;
      float z;
};

Vertex v[50000];
Triangle t[50000];
*/
int vertexCount = 0;
int triangleCount = 0;

std::vector<Obj::Vertex> v;
std::vector<Obj::Face> t;

struct RGB{
 float r;
 float g;
 float b;
 RGB(float r_,float g_, float b_) {
   r = r_;
   g = g_;
   b = b_;
 }
};

class ObjLoader: public Obj::File {
 public:
  virtual bool PostLoad(std::ostream& os){
    os<<"loaded"<<std::endl;
    // TODO  how to export data
    return true;
  }

  void GetVertices(std::vector<Obj::Vertex> & v) {
    v = m_Vertices;
  }

  void GetFaces(std::vector<Obj::Face> & t) {
    t = m_Triangles; 
  }
};

static void loadOBJ(const std::string & fName) {
  ObjLoader f;
  f.Load(fName.c_str());
  f.PostLoad(std::cout);
  f.GetFaces(t);
  f.GetVertices(v);
}

static void initColors(std::vector<RGB> &rgbs) {
  RGB rgbRed(1.0f,0.0f,0.0f);
  RGB rgbGreen(0.0f,1.0f,0.0f);
  RGB rgbBlue(0.0f,0.0f,1.0f);
  rgbs.push_back(rgbRed);
  rgbs.push_back(rgbBlue);
  rgbs.push_back(rgbGreen);
}

static void DrawOBJ()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    
  glLoadIdentity();				// Reset The View
  glTranslatef(0.0f,0.0f,-6.0f);		// Move Left 1.5 Units And Into The Screen 6.0
  glBegin(GL_TRIANGLES);

  std::vector<RGB> rgbs;
  initColors(rgbs);

  for (size_t i=0; i< t.size(); ++i)
  {
    for (int j = 0; j < 3; ++j) { 
      glColor3f(rgbs[j].r, rgbs[j].g, rgbs[j].b);	
      glVertex3f(v[t[i].v[j]].x * 0.25, v[t[i].v[j]].y * 0.25, v[t[i].v[j]].z * 0.25);
    }
  }
  
  glEnd();
  
// glFlush();
}



/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
  glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
  glDepthFunc(GL_LESS);				// The Type Of Depth Test To Do
  glEnable(GL_DEPTH_TEST);			// Enables Depth Testing
  glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();				// Reset The Projection Matrix

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

  glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
  if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
    Height=1;

  glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	
  DrawOBJ();
  // swap buffers to display, since we're double buffered.
  glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
    /* avoid thrashing this procedure */
    usleep(100);

    /* If escape is pressed, kill everything. */
    if (key == ESCAPE) 
    { 
	/* shut down our window */
	glutDestroyWindow(window); 
	
	/* exit the program...normal termination. */
	exit(0);                   
    }
}

int main(int argc, char **argv) 
{  
  if(argc < 2) {
    printf("usage: %s your_model.OBJ\n",argv[0]);
    exit(1);
  } 
  else  {
    fName = argv[1];
    printf("Parsing file %s\n", fName.c_str());
    // load Vertexes and faces stored in OBJ file  
    loadOBJ(fName);
  }
  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
     X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */  
  glutInit(&argc, argv);  

  /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffer */  
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

  /* get a 640 x 480 window */
  glutInitWindowSize(640, 480);  

  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);  

  /* Open a window */  
  window = glutCreateWindow("Jeff Molofee's GL Code Tutorial ... NeHe '99");  

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);  

  /* Go fullscreen.  This is the soonest we could possibly go fullscreen. */
  glutFullScreen();

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  /* Initialize our window. */
  InitGL(640, 480);
  
  /* Start Event Processing Engine */  
  glutMainLoop();  

  return 1;
}
