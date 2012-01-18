//// DO NOT FORGET UPDATE ATTRIBUTES AT ////
////  void bindShaderAttr(AppCtx *c) ;  ////
static const char gVertexShader[] =
// Vertex shader Gouraud Shading - Per-vertex lighting
//
"uniform mat4 u_mvpMatrix;\n"
"uniform mat4 u_normalMatrix;\n"
//
// eye pos
"uniform vec3 u_eyePos;\n"
//
"// position and normal of the vertices\n"
"attribute vec4 a_position;\n"
"attribute vec3 a_normal; \n"
//
// lighting
"uniform vec4 u_lightPos;\n"
"uniform vec4 u_lightColor;\n"
//
// material
"uniform vec4 u_matAmbient;\n"
"uniform vec4 u_matDiffuse;\n"
"uniform vec4 u_matSpecular;\n"
"uniform float u_matShininess;\n"
//
// color to pass on
"varying vec4 v_color;\n"
//
"void main() {\n"
    // eye position
"    vec3 eP = u_eyePos;\n"
    //
"    vec4 nm = u_normalMatrix * vec4(a_normal, 1.0);\n"
"    \n"
    // normal
//"    vec3 EyespaceNormal = vec3(u_mvpMatrix * vec4(a_normal, 1.0));\n"
"    vec3 EyespaceNormal = vec3(nm);\n"
//
    // the vertex position
"    vec4 posit = u_mvpMatrix * a_position; \n"
"    \n"
    // light dir
"    vec3 lightDir = u_lightPos.xyz - posit.xyz;\n"
"    vec3 eyeVec = -posit.xyz;\n"
    //
"    vec3 N = normalize(EyespaceNormal);\n"
"    vec3 E = normalize(eyeVec); \n"
    //
"    vec3 L = normalize(lightDir);\n"
    // Reflect the vector. Use this or reflect(incidentV, N);
"    vec3 reflectV = reflect(-L, N);\n"
    // Get lighting terms
"    vec4 ambientTerm;\n"
"    ambientTerm = u_matAmbient * u_lightColor;\n"
    //
"    vec4 diffuseTerm = u_matDiffuse * max(dot(N, L), 0.0);\n"
"    vec4 specularTerm = u_matSpecular * pow(max(dot(reflectV, E), 0.0), u_matShininess);\n"
    //
"    v_color =  ambientTerm + diffuseTerm + specularTerm;\n"
//"    v_color =  ambientTerm ;\n"
    //
"    gl_Position = posit; \n"
"}" ;
