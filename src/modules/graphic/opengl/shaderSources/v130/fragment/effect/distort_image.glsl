precision mediump float;
precision mediump int;

uniform vec4 pDistortTransform;

uniform mediump sampler2DArray texColor;
uniform lowp sampler2D texDistort;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out mediump vec4 outColor;

void main( void ){
	vec2 distortion = texture( texDistort, vTexCoord ).rg;
	distortion = distortion * pDistortTransform.xy + pDistortTransform.zw;
	
	outColor = texture( texColor, vec3( vTexCoord + distortion, vLayer ) );
}
