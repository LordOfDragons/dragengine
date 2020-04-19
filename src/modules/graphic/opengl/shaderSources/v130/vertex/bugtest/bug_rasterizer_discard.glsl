precision highp float;
precision highp int;

in vec2 inPosition;

out float outResult;

void main( void ){
	outResult = 1.0;
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 ); // keep broken compilers happy
}
