//// DO NOT FORGET UPDATE ATTRIBUTES AT ////
////  void bindShaderAttr(AppCtx *c) ;  ////
static const char gVertexShader[] =
    "uniform mat4 u_C;                         \n" // matrix for centering model
    "uniform mat4 u_R;                          \n" // matrix for rotating model
    "uniform mat4 u_S;                          \n" // matrix for scaling model 
    "uniform mat4 u_P;                          \n" // matrix for positioning model
    "uniform mat4 c_Perspective;                \n" // TODO should be CONSTANT perspective matrix
    "attribute vec4 a_position;					\n"
    "attribute vec4 a_color;					\n"
    "varying vec4   v_color;					\n"
    "void main() {						        \n"
	"  v_color = a_color;				        \n"
    // important is order we can think of 1.action centering_res = u_C * a_position, 2.action rotating_res = u_R * centering_res,..
// 	"  gl_Position = c_Perspective * u_P * u_S * u_R * u_C * a_position;  \n" 
 	"  gl_Position = c_Perspective * u_P * a_position;  \n" 
    "}								            \n";

static const char gFragmentShader[] = 
    "precision mediump float;		            \n"
	"varying vec4 v_color;			            \n"
    "void main() {					            \n"
    "  gl_FragColor = v_color;                  \n"
    "}                                          \n";

