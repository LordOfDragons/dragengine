precision highp float;
precision highp int;

uniform mat4x3 pMatrixMV;
uniform mat4 pMatrixP;

#ifdef FULLSCREENQUAD
	in vec2 inPosition;
#else
	in vec3 inPosition;
#endif

out vec3 vEnvMapDir;



// Main Function
//////////////////

void main( void ){
	#ifdef FULLSCREENQUAD
		gl_Position = vec4( inPosition, 0, 1 );
		vEnvMapDir = vec3( inPosition, 1 );
	#else
		vEnvMapDir = pMatrixMV * vec4( inPosition, 1 );
		gl_Position = pMatrixP * vec4( vEnvMapDir, 1 );
	#endif
}
