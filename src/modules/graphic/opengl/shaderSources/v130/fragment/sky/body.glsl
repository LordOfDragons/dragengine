precision highp float;
precision highp int;

uniform vec4 pColor; // TODO is now split into color/transparency...
uniform vec4 pMaterialGamma;

uniform lowp sampler2D texColor;
uniform lowp sampler2D texTransparency;

in vec2 vTexCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	in flat int vLayer;
#else
	const int vLayer = 0;
#endif

out vec4 outColor;

void main( void ){
	outColor.rgb = texture( texColor, vTexCoord ).rgb;
	outColor.a = texture( texTransparency, vTexCoord ).r;
	outColor = pow( outColor, pMaterialGamma ) * pColor;
}
