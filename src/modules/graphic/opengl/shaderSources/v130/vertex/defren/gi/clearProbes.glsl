precision highp float;
precision highp int;

in vec3 inPosition;

void main( void ){
	gl_Position = vec4( inPosition.xy, 0, 1 );
}
