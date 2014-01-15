//����͍ގ��W�����V�F�[�_�\�[�X�ɖ��ߍ���ł���
const vec4 ambMaterial = vec4( 0.24725, 0.1995, 0.0745,1);
const vec4 diffMaterial = vec4(0.75164, 0.60648 , 0.22648,1);
const vec4 specMaterial = vec4(0.628281, 0.555802,0.366065,1);
const float shinMaterial =  51.2 ;


//���_�V�F�[�_
void main(void)
{
	//���e�ϊ� (���f���r���[ * �v���W�F�N�V����) * ���_���W
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

	//ambient�v�Z
	vec4 ambient = gl_LightSource[0].ambient * ambMaterial;//ambient�̌v�Z

	//�e���{�I�ȃx�N�g�����v�Z
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);//���_���W�̖@���{���K��
	vec4 V = gl_ModelViewMatrix * gl_Vertex;//���_���W�n�̒��_�ʒu
	vec3 L = normalize(gl_LightSource[0].position.xyz - V.xyz);//���_�������ւ̃x�N�g��

	//diffuse�v�Z
	float NdotL = dot(N,L);
	vec4 diffuse = vec4( max(0.0,NdotL) ) * gl_LightSource[0].diffuse * diffMaterial;


	//specular�v�Z(Biln�̃��f�����g�p)
	vec3 H = normalize(gl_LightSource[0].halfVector.xyz);
	float NdotH = dot(N,H);
	float specular = pow(max(0.0, NdotH), shinMaterial);
	if(NdotL <= 0.0)//�����ɂ���Ă�specular����
	{
			specular = 0.0;
	}
	vec4 spec = specular * gl_LightSource[0].specular * specMaterial;
        
        
	//�o�͐F
	gl_FrontColor = ambient + diffuse + spec;
}