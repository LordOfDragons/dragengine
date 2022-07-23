precision highp float;
precision highp int;

uniform vec4 pQuadTCTransform;

in vec3 inPosition;

#ifdef GS_RENDER_STEREO
	out vec4 vGSScreenCoord;
	#define vScreenCoord vGSScreenCoord
#else
	out vec4 vScreenCoord;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 1 );
	vScreenCoord = inPosition.xyxy;
	vScreenCoord.xy = vScreenCoord.xy * pQuadTCTransform.xy + pQuadTCTransform.zw;
}
