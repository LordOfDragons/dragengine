precision HIGHP float;
precision HIGHP int;

uniform vec4 pGamma;
uniform vec4 pColor;

uniform mediump sampler2D texColor;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out mediump vec4 outColor;

void main( void ){
	outColor = pow( texture( texColor, vTexCoord ), pGamma ) * pColor;
}
