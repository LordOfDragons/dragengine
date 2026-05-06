#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

UNIFORM_BIND(3) uniform vec4 pGamma; // red, green, blue
UNIFORM_BIND(4) uniform vec4 pBrightness; // red, green, blue
UNIFORM_BIND(5) uniform vec4 pContrast; // red, green, blue

layout(binding=0) uniform lowp sampler2DArray texColor;

#include "shared/interface/2d/fragment.glsl"
#include "shared/hdr_output.glsl"

// SplitLayers
layout(location=0) out vec4 outColor1;
layout(location=1) out vec4 outColor2;

// !SplitLayers
// layout(location=0) out vec4 outColor;

void main(void){
	// gamma correction only
	//outColor = pow( texture( texColor, vec3( vTexCoord, vLayer ) ), pGamma );
	
	// gamma, contrast and brightness correction
	if(SplitLayers){
		outColor1 = pow(texture(texColor, vec3(vTexCoord, 0)), pGamma) * pContrast + pBrightness;
		outColor2 = pow(texture(texColor, vec3(vTexCoord, 1)), pGamma) * pContrast + pBrightness;
		
	}else{
		outColor1 = pow(texture(texColor, vec3(vTexCoord, vLayer)), pGamma) * pContrast + pBrightness;
	}
	
	if(HdrOutput){
		outColor1.rgb = encodeHdrOutput(outColor1.rgb);
		if(SplitLayers){
			outColor2.rgb = encodeHdrOutput(outColor2.rgb);
		}
	}
	
	// clamp alpha value to the range from 0 to 1. larger values can happen during
	// rendering of transparent objects and can cause issues in 2d rendering
	if(SplitLayers){
		outColor1.a = clamp(outColor1.a, 0.0, 1.0);
		outColor2.a = clamp(outColor2.a, 0.0, 1.0);
		
	}else{
		outColor1.a = clamp(outColor1.a, 0.0, 1.0);
	}
}
