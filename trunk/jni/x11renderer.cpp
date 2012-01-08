#include <stdlib.h>
#include "esUtils.h"
#include "esX11Util.h"
#include "natRenderer.h"
#include "def.h"
#include "genCube.h"


void _escontext::setWidth(GLint w) {
        this->c->width = w;
}
void _escontext::setHeight(GLint h) {
        this->c->height = h;
}
GLint _escontext::height() {
        return this->c->height;
}
GLint _escontext::width() {
        return this->c->width;
}

void Init ( AppCtx * c ) {
    // Generate the vertex data
    genCube1(1.0f, c); 

    // set initial state for OpenGL
    setupGraphics(c);

    // TODO should be called after resizing window
    viewValuesSetUp(c);
   printf("Setup Finish\n");
}


void Update ( AppCtx * c, float deltaTime ) {
   // TODO
//   rotateAnchor(c, dx, dy);
//   zoom(c,z);
}

void Draw ( AppCtx * c ) {
   renderFrame(c);
}

void  keyPressed( AppCtx *c , unsigned char key, int x, int y) {
    LOGI("key: %c , unknown x:%d , y:%d", key, x, y);
    switch(key) {
        case 'i':
            zoom(c,0.1f);
            break;
        case 'o':
            zoom(c,-0.1f);
            break;
        case 'e':
            rotateAnchor(c, 5.0f, 0.0f);
            break;
        case 'd':
            rotateAnchor(c, -5.0f, 0.0f);
            break;
        case 's':
            rotateAnchor(c, 0.0f, 5.0f);
            break;
        case 'f':
            rotateAnchor(c, 0.0f, -5.0f);
            break;
    }
}


int main ( int argc, char *argv[] ) {
   ESContext esContext;
   struct AppCtx c;

   esInitContext ( &esContext );
   esContext.c = &c;

   esCreateWindow ( &esContext, "Simple Texture 2D", 320, 240, ES_WINDOW_RGB );

   Init ( &c );

   esRegisterDrawFunc ( &esContext, Draw );
   esRegisterUpdateFunc ( &esContext, Update );
   esRegisterKeyFunc ( &esContext, keyPressed );

   esMainLoop ( &esContext );

}

