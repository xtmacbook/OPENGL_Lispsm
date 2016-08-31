uniform mat4 texMatrix;

varying vec3 N;
varying vec3 eyePosHomo;

void main(void) {
	gl_Position = ftransform();
	vec4 eyePos = gl_ModelViewMatrix*gl_Vertex;

	eyePosHomo = vec3(eyePos)/eyePos.w;
	N = gl_NormalMatrix*gl_Normal;

	//base texture coordinates (1-dim)
	gl_TexCoord[0] = gl_MultiTexCoord0;

	//shadow texture coordinates
	gl_TexCoord[1] = texMatrix*eyePos;

	//vertex coordinates in objectspace
	gl_TexCoord[3] = gl_Vertex;

	gl_FrontColor = gl_Color;
	gl_FogFragCoord = abs(eyePos.z);
}
