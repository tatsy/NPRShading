//���_�V�F�[�_
void main(void)
{
	//���e�ϊ� (���f���r���[ * �v���W�F�N�V����) * ���_���W
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	vec3 N = normalize(gl_NormalMatrix * gl_Normal);
	gl_FrontColor = vec4(N.x, N.y, N.z, 1.0);
}