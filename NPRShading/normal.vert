//頂点シェーダ
void main(void)
{
	//投影変換 (モデルビュー * プロジェクション) * 頂点座標
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	gl_FrontColor = vec4(N.x, N.y, N.z, 1.0);
}