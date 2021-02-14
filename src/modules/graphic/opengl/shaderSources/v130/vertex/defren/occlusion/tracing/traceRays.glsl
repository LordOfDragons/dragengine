precision highp float;
precision highp int;

in vec3 inPosition;

void main( void ){
	gl_Position = vec4( vec3( inPosition.xy, 0.0 ), 1.0 );
}
