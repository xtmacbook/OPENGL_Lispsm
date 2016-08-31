uniform sampler2D tex2D;

void main (void) {
	vec4 color = gl_Color;
	float intensity = texture2D(tex2D,vec2(gl_TexCoord[0])).a;
	color.a = intensity;

//	float fog = (gl_Fog.end-gl_FogFragCoord)*gl_Fog.scale;
//	fog = clamp(fog,0.0,1.0);
//	color = mix(gl_Fog.color,color,fog);
	gl_FragColor = color;
}
