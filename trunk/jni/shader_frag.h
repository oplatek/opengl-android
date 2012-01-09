// our fragment shader match  /opengl-tutorial/Tut 09 Lights on/data/ColorPassthrough.frag
static const char gFragmentShader[] = 
"precision mediump float;		            \n"
"varying vec4 v_color;  	            \n"
"void main() {					            \n"
"  gl_FragColor = v_color;              \n"
"}                                          \n";
