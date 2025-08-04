#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec4 pColor;
UNIFORM_BIND(1) uniform vec4 pGamma;
// TextureLevel
UNIFORM_BIND(2) uniform float pLevel;
// InputArrayTextures
UNIFORM_BIND(3) uniform float pLayer;

// InputArrayTextures
layout(binding=0) uniform mediump sampler2DArray texTextureArray;

// !InputArrayTextures
layout(binding=0) uniform mediump sampler2D texTexture;

VARYING_BIND(0) in vec2 vTexCoord;


layout(location=0) out vec4 outColor;


void main( void ){
	vec4 color;
	
	if(InputArrayTextures){
		if(TextureLevel){
			color = textureLod(texTextureArray, vec3(vTexCoord, pLayer), pLevel);
		}else{
			color = textureLod(texTextureArray, vec3(vTexCoord, pLayer), 0.0);
		}
		
	}else{
		if(TextureLevel){
			color = textureLod(texTexture, vTexCoord, pLevel);
		}else{
			color = textureLod(texTexture, vTexCoord, 0.0);
		}
	}
	
	if(OutDataSwizzle == 1){
		color = color.gggg;
	}
	
	if(OutDataSize == 1){
		outColor.x = pow(color.x * pColor.x, pGamma.x);
	}else if(OutDataSize == 2){
		outColor.xy = pow(vec2(color) * vec2(pColor), vec2(pGamma));
	}else if(OutDataSize == 3){
		outColor.xyz = pow(vec3(color) * vec3(pColor), vec3(pGamma));
	}else{
		outColor = pow(color * pColor, pGamma);
	}
}
