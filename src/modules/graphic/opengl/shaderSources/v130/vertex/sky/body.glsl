precision highp float;
precision highp int;

uniform mat4 pMatrixMVP;
uniform vec2 pScalePosition;

in vec2 inPosition;

out vec2 vTexCoord;

const vec2 tcScale = vec2( 0.5, -0.5 );
const vec2 tcOffset = vec2( 0.5, 0.5 );

void main( void ){
	gl_Position = pMatrixMVP * vec4( vec3( inPosition * pScalePosition, 1.0 ), 1.0 );
	vTexCoord = inPosition * tcScale + tcOffset;
}
