precision highp float;
precision highp int;

uniform vec4 pQuadTCTransform;

in vec3 inPosition;

out vec4 vScreenCoord;

void main( void ){
	gl_Position = vec4( inPosition, 1.0 );
	vScreenCoord = inPosition.xyxy;
	vScreenCoord.xy = vScreenCoord.xy * pQuadTCTransform.xy + pQuadTCTransform.zw;
}
