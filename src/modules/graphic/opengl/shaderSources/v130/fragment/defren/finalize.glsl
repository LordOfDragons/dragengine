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

#ifdef SPLIT_LAYERS
	out vec4 outColor1;
	out vec4 outColor2;
#else
	out vec4 outColor;
#endif

void main( void ){
	// gamma correction only
	//outColor = pow( texture( texColor, vec3( vTexCoord, vLayer ) ), pGamma );
	
	// gamma, contrast and brightness correction
	#ifdef SPLIT_LAYERS
		outColor1 = pow( texture( texColor, vec3( vTexCoord, 0 ) ), pGamma ) * pContrast + pBrightness;
		outColor2 = pow( texture( texColor, vec3( vTexCoord, 1 ) ), pGamma ) * pContrast + pBrightness;
	#else
		outColor = pow( texture( texColor, vec3( vTexCoord, vLayer ) ), pGamma ) * pContrast + pBrightness;
	#endif
}
