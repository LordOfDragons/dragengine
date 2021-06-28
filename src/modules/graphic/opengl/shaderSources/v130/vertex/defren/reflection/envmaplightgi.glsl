precision highp float;
precision highp int;

in vec2 inPosition;

void main( void ){
	gl_Position = vec4( inPosition, 0.0, 1.0 );
}
