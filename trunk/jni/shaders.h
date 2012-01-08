//// DO NOT FORGET UPDATE ATTRIBUTES AT ////
////  void bindShaderAttr(AppCtx *c) ;  ////
static const char gVertexShader[] =
//"const float cf_zero = 0.0f;               \n"
"uniform mat4 u_C;                         \n" // matrix for centering model
"uniform mat4 u_R;                          \n" // matrix for rotating model
"uniform mat4 u_S;                          \n" // matrix for scaling model 
"uniform mat4 u_P;                          \n" // matrix for positioning model
"uniform mat4 c_Perspective;                \n" // TODO should be CONSTANT perspective matrix
"uniform vec3 u_dirToLight;                   \n" 
"attribute vec4 a_position;					\n" //
"attribute vec4 a_color;					\n"
"attribute vec3 a_normal;                   \n"  
"varying vec4   interpColor;				\n"
"void main() {						        \n"
"   interpColor = a_color;				        \n"
// important is order we can think of 1.action centering_res = u_C * a_position, 2.action rotating_res = u_R * centering_res,..
"   mat4 modelToCameraMatrix = u_P * u_S * u_R * u_C; \n"  	
"   gl_Position = c_Perspective * (modelToCameraMatrix * a_position);  \n"  
"   vec4 normal = vec4(a_normal.x, a_normal.y, a_normal.z, 0.0); \n"
// TODO I suppose that normalized matrix is not crucial -> I hope that it is meant normalized by rows see original vertex shader below
"   vec4 normCamSpace4 = normalize(modelToCameraMatrix * normal);   \n" 
"   vec3 normCamSpace = vec3(normCamSpace4.x, normCamSpace4.y, normCamSpace4.z);   \n" 
"   float cosAngIncidence = dot(normCamSpace, u_dirToLight);   \n" 
"   cosAngIncidence = clamp(cosAngIncidence, 0.0, 1.0); \n"
"   interpColor = a_color * cosAngIncidence; \n" // TODO we can add lightIntensity
"}								            \n";

// vertex shader from /opengl-tutorial/Tut 09 Lights on/data/DirVertexLighting_PCN.vert
/*
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 diffuseColor;
layout(location = 2) in vec3 normal;

//smooth out vec4 interpColor;
out vec4 interpColor;

uniform vec3 dirToLight;
uniform vec4 lightIntensity;

uniform mat4 modelToCameraMatrix;
uniform mat3 normalModelToCameraMatrix;

layout(std140) uniform Projection
{
	mat4 cameraToClipMatrix;
};

void main()
{
	gl_Position = cameraToClipMatrix * (modelToCameraMatrix * vec4(position, 1.0));

	vec3 normCamSpace = normalize(normalModelToCameraMatrix * normal);
	
	float cosAngIncidence = dot(normCamSpace, dirToLight);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);
	
	interpColor = lightIntensity * diffuseColor * cosAngIncidence;
}
*/

// our fragment shader match  /opengl-tutorial/Tut 09 Lights on/data/ColorPassthrough.frag
static const char gFragmentShader[] = 
"precision mediump float;		            \n"
"varying vec4 interpColor;  	            \n"
"void main() {					            \n"
"  gl_FragColor = interpColor;              \n"
"}                                          \n";

