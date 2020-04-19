precision highp float;
precision highp int;

uniform mat4x3 pMatrixMV;
uniform mat4 pMatrixP;

in vec3 inPosition;

out vec3 vEnvMapDir;



// Main Function
//////////////////

void main( void ){
	#ifdef FULLSCREENQUAD
		gl_Position = vec4( inPosition, 1.0 );
		vEnvMapDir = vec3( inPosition.xy, 1.0 );
	#else
		vEnvMapDir = pMatrixMV * vec4( inPosition, 1.0 );
		gl_Position = pMatrixP * vec4( vEnvMapDir, 1.0 );
	#endif
}
