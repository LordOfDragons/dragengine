#include "shared/preamble.glsl"

precision mediump float;
precision mediump int;

// MipMap
UNIFORM_BIND(3) uniform float pMipMapLevel;

// ClampTC && MipMap
UNIFORM_BIND(4) uniform vec2 pTCClampMipMap;

// ClampTC && !MipMap
UNIFORM_BIND(5) uniform ivec2 pTCClamp;

// InputArrayTextures
layout(binding=0) uniform mediump sampler2DArray texColorArray;

// !InputArrayTextures
layout(binding=0) uniform mediump sampler2D texColor;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out mediump vec4 outColor;


vec4 sampleColor(const in vec2 tc){
	if(InputArrayTextures){
		return textureLod(texColorArray, vec3(tc, vLayer), pMipMapLevel);
		
	}else{
		return textureLod(texColor, tc, pMipMapLevel);
	}
}

vec4 fetchColor(const in ivec2 tc){
	if(InputArrayTextures){
		return texelFetch(texColorArray, ivec3(tc, vLayer), 0);
		
	}else{
		return texelFetch(texColor, tc, 0);
	}
}


void main( void ){
	if(MipMap){
		if(ClampTC){
			outColor = sampleColor(min(vTexCoord, pTCClampMipMap));
			
		}else{
			outColor = sampleColor(vTexCoord);
		}
		
	}else{
		if(ClampTC){
			outColor = fetchColor(min(ivec2(gl_FragCoord), pTCClamp));
			
		}else{
			outColor = fetchColor(ivec2(gl_FragCoord));
		}
	}
}
