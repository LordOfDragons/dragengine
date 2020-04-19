precision highp float;
precision highp int;

uniform float pPositionZ;

in vec2 inPosition;

out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( vec3( inPosition, pPositionZ ), 1.0 );
	vTexCoord = inPosition;
}
