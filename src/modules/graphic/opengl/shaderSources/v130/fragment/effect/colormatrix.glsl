precision mediump float;
precision mediump int;

uniform mat4 pColorMatrix;
uniform vec4 pColorOffset;

uniform mediump sampler2DArray texColor;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	in flat int vLayer;
#else
	const int vLayer = 0;
#endif

out mediump vec4 outColor;

void main( void ){
	outColor = pColorMatrix * texelFetch( texColor, ivec3( gl_FragCoord.xy, vLayer ), 0 ) + pColorOffset;
}
