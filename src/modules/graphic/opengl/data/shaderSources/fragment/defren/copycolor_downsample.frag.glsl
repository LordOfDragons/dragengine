#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

UNIFORM_BIND(3) uniform float pMipMapLevel;
UNIFORM_BIND(4) uniform vec2 pTCClamp;
UNIFORM_BIND(5) uniform vec2 pPixelSize; // 1/size

layout(binding=0) uniform mediump sampler2DArray texColor;

#include "shared/interface/2d/fragment.glsl"
#include "shared/defren/downsample.glsl"

layout(location=0) out mediump vec4 outColor;

void main(void){
	// 13-tap produces slightly better results with negligible extra cost
	outColor = downsampleColorNpot13Tap(texColor, vTexCoord, pPixelSize, pTCClamp, vLayer, pMipMapLevel);
	//outColor = downsampleColorNpot4Tap(texColor, vTexCoord, pPixelSize, pTCClamp, vLayer, pMipMapLevel);
}
