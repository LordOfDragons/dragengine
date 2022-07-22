precision mediump float;
precision mediump int;

uniform vec4 pDistortTransform;

uniform mediump sampler2DArray texColor;
uniform lowp sampler2D texDistort;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out mediump vec4 outColor;

void main( void ){
	vec3 tc( vTexCoord, vLayer );
	tc.xy += texture( texDistort, vTexCoord ).rg * pDistortTransform.xy + pDistortTransform.zw;
	
	outColor = texture( texColor, tc );
}
