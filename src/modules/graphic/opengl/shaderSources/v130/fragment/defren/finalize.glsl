precision lowp float;
precision lowp int;

uniform vec4 pGamma; // red, green, blue
uniform vec4 pBrightness; // red, green, blue
uniform vec4 pContrast; // red, green, blue

uniform lowp sampler2DArray texColor;

in vec2 vTexCoord;

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out vec4 outColor;

void main( void ){
	// gamma correction only
	//outColor = pow( texture( texColor, vec3( vTexCoord, vLayer ) ), pGamma );
	
	// gamma, contrast and brightness correction
	outColor = pow( texture( texColor, vec3( vTexCoord, vLayer ) ), pGamma ) * pContrast + pBrightness;
}
