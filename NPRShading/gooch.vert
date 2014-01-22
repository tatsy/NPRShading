// npr info
const float  b      = 0.4;
const float  y      = 0.4;
const float  alpha  = 0.2;
const float  beta   = 0.6;
const vec4 k_blue = vec4(0.0, 0.0, b, 1.0);
const vec4 k_yell = vec4(y, y, 0.0, 1.0);
const vec4 k_diff = vec4(0.5, 0.5, 0.5, 1.0);

const vec4 k_cool = k_blue + alpha * k_diff;
const vec4 k_warm = k_yell + beta  * k_diff;

// material info
const vec4 ambMaterial = vec4(0.5, 0.5, 0.5, 1.0);
const vec4 diffMaterial = vec4(0.8, 0.2, 0.2, 1.0);
const vec4 specMaterial = vec4(0.7, 0.7, 0.7, 1.0);
const float shinMaterial =  51.2;

void main(void)
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	vec4 V = gl_ModelViewMatrix * gl_Vertex;
	vec3 L = normalize(gl_LightSource[0].position.xyz - V.xyz);

    // npr
    float LdotN = dot(L, N);
    vec4 coolColor = (1.0 - (1.0 + LdotN) * 0.5) * k_cool;
    vec4 warmColor = (1.0 + LdotN) * 0.5 * k_warm;

    // ambient
	vec4 ambient = gl_LightSource[0].ambient * ambMaterial;

    // diffuse
	float NdotL = dot(N, L);
    vec4 diffuse = vec4(max(0.0, NdotL)) * gl_LightSource[0].diffuse * diffMaterial;

    // specular
    vec3 H = normalize(gl_LightSource[0].halfVector.xyz);
    float NdotH = dot(N, H);
    float spec = pow(max(0.0, NdotH), shinMaterial);
    if(NdotL <= 0.0) {
        spec = 0.0;
    }
	if(spec > 0.01) {
		spec = 1.0;
	}
    vec4 specular = spec * gl_LightSource[0].specular * specMaterial;

    vec4 shading = ambient + diffuse + specular;
    vec4 npr     = coolColor + warmColor;
	gl_FrontColor  = 0.5 * shading + 0.5 * npr;
	// gl_FrontColor = npr;
}
