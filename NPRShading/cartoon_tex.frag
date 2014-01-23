uniform sampler1D nprTex;

const vec4 ambMaterial  = vec4(0.5, 0.5, 0.5, 1.0);
const vec4 specMaterial = vec4(0.7, 0.7,0.7, 1.0);
const float shinMaterial =  51.2 ;

varying vec3 normal;
varying vec3 vtoL;

void main(void)
{
    vec4 ambient = gl_LightSource[0].ambient * ambMaterial;

    vec3 N = normalize(normal);
    vec3 L = normalize(vtoL);
    float NdotL = max(0.0, dot(N, L));
    vec4 diffuse;
    diffuse.xyz = texture1D(nprTex, NdotL);
    diffuse.a = 0.0;

    vec3 H = normalize(gl_LightSource[0].halfVector.xyz);
    float NdotH = dot(N, H);

    float spec = pow(max(0.0, NdotH), shinMaterial);
    if(NdotL <= 0.0) {
        spec = 0.0;
    }

    gl_FragColor = ambient + diffuse;
    if(spec > 0.85) {
        gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}
