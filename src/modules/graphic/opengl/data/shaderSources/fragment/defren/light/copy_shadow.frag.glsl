#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout(binding=0) uniform HIGHP sampler2D texShadow;
layout(binding=0) uniform HIGHP samplerCube texShadowCube;

layout(binding=1) uniform mediump sampler2D texColor;
layout(binding=1) uniform mediump samplerCube texColorCube;

VARYING_BIND(0) in vec3 vTexCoord;

// CopyColor
layout(location=0) out vec4 outColor;

void main(){
	if(Shadow1Mode == ShadowModeCube){
		gl_FragDepth = textureLod(texShadowCube, vTexCoord, 0.0).r;
		
		if(CopyColor){
			outColor = textureLod(texColorCube, vTexCoord, 0.0);
		}
		
	}else{
		ivec2 tc = ivec2(gl_FragCoord);
		
		gl_FragDepth = texelFetch(texShadow, ivec2(gl_FragCoord), 0).r;
		
		if(CopyColor){
			outColor = texelFetch(texColor, ivec2(gl_FragCoord), 0);
		}
	}
}
