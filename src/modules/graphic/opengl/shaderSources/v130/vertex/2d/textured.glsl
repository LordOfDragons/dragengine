precision highp float;
precision highp int;

uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform; // scaleU, scaleV, offsetU, offsetV

in vec2 inPosition;

out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
	vTexCoord = inPosition * pTCTransform.xy + pTCTransform.zw;
}
