#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

uniform vec4 pGamma; // red, green, blue
uniform vec4 pBrightness; // red, green, blue
uniform vec4 pContrast; // red, green, blue

layout(binding=0) uniform lowp sampler2DArray texColor;

#include "shared/interface/2d_fragment.glsl"

#ifdef SPLIT_LAYERS
	layout(location=0) out vec4 outColor1;
	layout(location=1) out vec4 outColor2;
#else
	layout(location=0) out vec4 outColor;
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
