precision highp float;
precision highp int;

uniform vec4 pQuadTCTransform; // scaleX, scaleY, offsetX, offsetY
uniform mat4 pMatrixMVP;
uniform mat4x3 pMatrixMV;

in vec3 inPosition;

#ifdef FULLSCREENQUAD
	out vec4 vScreenCoord;
#else
	out vec3 vVolumePos;
#endif



void main( void ){
	#ifdef FULLSCREENQUAD
		gl_Position = vec4( inPosition, 1.0 );
		vScreenCoord = inPosition.xyxy;
		vScreenCoord.xy = vScreenCoord.xy * pQuadTCTransform.xy + pQuadTCTransform.zw;
	#else
		gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
		vVolumePos = pMatrixMV * vec4( inPosition, 1.0 );
	#endif
}
