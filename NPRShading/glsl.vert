varying vec3 normal;
varying vec3 vtoL;

void main(void)
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    normal = gl_NormalMatrix * gl_Normal;
    vec4 V = gl_ModelViewMatrix * gl_Vertex;
    vtoL = gl_LightSource[0].position.xyz - V.xyz;
}
