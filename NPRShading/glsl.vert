varying vec3 normal;
varying vec3 vtoL;
varying vec3 vtoL2;

void main(void)
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    normal = gl_NormalMatrix * gl_Normal;
    vec4 V = gl_ModelViewMatrix * gl_Vertex;
    vtoL   = gl_LightSource[0].position.xyz - V.xyz;
    vtoL2  = gl_LightSource[1].position.xyz - V.xyz;
}
