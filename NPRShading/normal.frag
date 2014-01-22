varying vec3 normal;
varying vec3 vtoL;

void main(void)
{
    vec3 N = normalize(normal);
    gl_FragColor.xyz = N;
    gl_FragColor.a = 0.0;
}
