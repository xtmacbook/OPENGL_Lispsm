uniform sampler1D tex1D;
uniform sampler2D texNoise;
uniform sampler2DShadow texShadow;

uniform bool shadowing;
uniform bool directional;
uniform vec2 texMapSize;
uniform vec2 texMapScale;

varying vec3 N;
varying vec3 eyePosHomo;

vec3 calcTexCoord(in vec4 texCoord) {
	return texCoord.xyz/texCoord.w;
}

float lkup(in vec3 coord) {
	float depth = shadow2D(texShadow,coord,-100.0).x; 
	return (depth > coord.z)?1.0:0.0; //for ati we implement the shadow map test in the shader
	//return shadow2D(texShadow,coord).x; for nvidia this should suffice
}

float lkup(in vec3 coord, in float lod) {
	float depth = shadow2D(texShadow,coord,lod).x; 
	return (depth > coord.z)?1.0:0.0; //for ati we implement the shadow map test in the shader
	//return shadow2D(texShadow,coord).x; for nvidia this should suffice
}

float pcfShadow2x2(in vec4 texCoord, in float lod) {
	vec3 homogen = calcTexCoord(texCoord);

	float texel[4];
	vec2 start = homogen.xy;//-texMapScale*0.49;
	texel[0] = lkup(vec3(start,homogen.z),lod);
	texel[1] = lkup(vec3(start+vec2(texMapScale.x,0.0),homogen.z),lod);
	texel[2] = lkup(vec3(start+vec2(0.0,texMapScale.y),homogen.z),lod);
	texel[3] = lkup(vec3(start+texMapScale,homogen.z),lod);

	vec2 bigcoords = homogen.xy*(texMapSize);

/*	vec2 min = floor(bigcoords);
	vec2 max = min+1.0;
	min *= texMapScale;		//min /= texMapSize;
	max *= texMapScale;		//max /= texMapSize;
	float texel[4];
	texel[0] = lkup(vec3(min,homogen.z));
	texel[1] = lkup(vec3(max.x,min.y,homogen.z));
	texel[2] = lkup(vec3(min.x,max.y,homogen.z));
	texel[3] = lkup(vec3(max,homogen.z));
*/
	vec2 f = fract(bigcoords);

	//bilinear interpolation
	return mix(mix(texel[0], texel[1], f.x),
		        mix(texel[2], texel[3], f.x), f.y);
}

void main(void) {
	//do this texture lookup first because GL_TEXTURE_COMPARE_MODE can have problems otherwise
	vec3 texColor = texture1D(tex1D,gl_TexCoord[0].s).rgb;
	vec4 noise = texture2D(texNoise,gl_TexCoord[3].xz*0.3);
	vec3 material = texColor/*+gl_Color.rgb*/;
	material -= noise.a*0.2*gl_TexCoord[0].s+noise.g*0.1;

	float lighting = 1.0;
	vec3 n = normalize(N);
	if(directional) {
		//diffuse component
		vec3 L = vec3(gl_LightSource[0].position);
		lighting = max(dot(n,L),0.0);
	}
	else {
		//diffuse component
		vec3 vp = normalize(vec3(gl_LightSource[0].position)-eyePosHomo);
		float spotDot = dot(-vp,gl_LightSource[0].spotDirection);
		lighting = spotDot < gl_LightSource[0].spotCosCutoff ? 0.0 : max(dot(n,vp),0.0);
	}

	float shadow = shadowing ? pcfShadow2x2(gl_TexCoord[1],-100.0) : 1.0; //with additional pcf
//	float shadow = shadowing ? lkup(calcTexCoord(gl_TexCoord[1])) : 1.0; //just standard filtering*/
    material *= lighting;
	gl_FragColor.rgb = mix(material*0.5,material,shadow);
	gl_FragColor.a = 1.0;

}
