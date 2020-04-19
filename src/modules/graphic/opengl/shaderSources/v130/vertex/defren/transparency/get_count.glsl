precision mediump float;
precision mediump int;

uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec2 pThresholdTransform; // scaleX, offsetX

in vec2 inPosition;

out float vThreshold;

void main( void ){
	gl_Position = vec4( vec3( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0 ), 1.0 );
	vThreshold = inPosition.x * pThresholdTransform.x + pThresholdTransform.y;
}
