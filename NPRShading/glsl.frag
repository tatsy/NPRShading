const vec4 ambMaterial  = vec4( 0.24725, 0.1995, 0.0745,1);
const vec4 diffMaterial = vec4(0.75164, 0.60648 , 0.22648,1);
const vec4 specMaterial = vec4(0.628281, 0.555802,0.366065,1);
const float shinMaterial =  51.2 ;

varying vec3 normal;
varying vec3 vtoL;

void main(void)
{
    vec4 ambient = gl_LightSource[0].ambient * ambMaterial;

    vec3 N = normalize(normal);
    vec3 L = normalize(vtoL);
    float NdotL = dot(N, L);
    vec4 diffuse = vec4(max(0.0, NdotL)) * gl_LightSource[0].diffuse * diffMaterial;

    vec3 H = normalize(gl_LightSource[0].halfVector.xyz);
    float NdotH = dot(N, H);

    float spec = pow(max(0.0, NdotH), shinMaterial);
    if(NdotL <= 0.0) {
        spec = 0.0;
    }
    vec4 specular = spec * gl_LightSource[0].specular * specMaterial;

    gl_FragColor = ambient + diffuse + specular;
}
