void main(void) {
//	gl_FragColor = pack(depth); //wenn alpha test sollte alpha > 0.1 sein
//	gl_FragDepth = gl_FragCoord.z; //needed for geforce (can't compile empty shader) //makes polygon offset on ati turn to software :)
	gl_FragColor = vec4(1.0,1.0,1.0,1.0); //so use dummy
}
