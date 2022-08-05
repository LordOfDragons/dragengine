precision highp float;
precision highp int;

uniform vec4 pQuadParams; // scaleX, scaleY, offsetX, offsetY

in vec3 inPosition;

out vec2 vTexCoord;
out vec2 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 1 );
	vScreenCoord = inPosition.xy;
	vTexCoord = inPosition.xy * pQuadParams.xy + pQuadParams.zw;
}
