precision highp float;
precision highp int;

uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

out vec2 vTexCoord;
out vec2 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 0, 1 );
	vScreenCoord = inPosition;
	vTexCoord = inPosition * pQuadParams.xy + pQuadParams.zw;
}
