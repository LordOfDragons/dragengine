uniform vec4 pPosTransform; // scaleX, scaleY, offsetX, offsetY
uniform vec4 pTCTransform;

in vec2 inPosition;

out vec2 vTC;

void main( void ){
	gl_Position = vec4( inPosition * pPosTransform.xy + pPosTransform.zw, 0.0, 1.0 );
	vTC = inPosition * pTCTransform.xy + pTCTransform.zw;
}
