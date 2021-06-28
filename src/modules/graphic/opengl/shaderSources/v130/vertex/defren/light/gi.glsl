#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

in vec3 inPosition;

out vec2 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 1.0 );
	vScreenCoord = inPosition.xy;
}
