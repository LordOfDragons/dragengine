#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

in vec3 inPosition;

#ifdef GS_RENDER_STEREO
	out vec2 vGSScreenCoord;
#else
	out vec2 vScreenCoord;
#endif

void main( void ){
	gl_Position = vec4( inPosition, 1 );
	
	#ifdef GS_RENDER_STEREO
		vGSScreenCoord = inPosition.xy;
	#else
		vScreenCoord = inPosition.xy;
	#endif
}
