//今回は材質係数をシェーダソースに埋め込んでいる
const vec4 ambMaterial = vec4( 0.24725, 0.1995, 0.0745,1);
const vec4 diffMaterial = vec4(0.75164, 0.60648 , 0.22648,1);
const vec4 specMaterial = vec4(0.628281, 0.555802,0.366065,1);
const float shinMaterial =  51.2 ;


//頂点シェーダ
void main(void)
{
	//投影変換 (モデルビュー * プロジェクション) * 頂点座標
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//ambient計算
	vec4 ambient = gl_LightSource[0].ambient * ambMaterial;//ambientの計算

	//各種基本的なベクトルを計算
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);//視点座標の法線＋正規化
	vec4 V = gl_ModelViewMatrix * gl_Vertex;//視点座標系の頂点位置
	vec3 L = normalize(gl_LightSource[0].position.xyz - V.xyz);//頂点→光源へのベクトル

	//diffuse計算
	float NdotL = dot(N,L);
	vec4 diffuse = vec4( max(0.0,NdotL) ) * gl_LightSource[0].diffuse * diffMaterial;


	//specular計算(Bilnのモデルを使用)
	vec3 H = normalize(gl_LightSource[0].halfVector.xyz);
	float NdotH = dot(N,H);
	float specular = pow(max(0.0, NdotH), shinMaterial);
	if(NdotL <= 0.0)//条件によってはspecular無し
	{
			specular = 0.0;
	}
	vec4 spec = specular * gl_LightSource[0].specular * specMaterial;
        
        
	//出力色
	gl_FrontColor = ambient + diffuse + spec;
}