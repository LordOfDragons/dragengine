#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

uniform mat4 pMatrixMVP;

in vec3 inPosition;

out vec3 vDirection;

void main( void ){
	gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
	vDirection = inPosition;
}
