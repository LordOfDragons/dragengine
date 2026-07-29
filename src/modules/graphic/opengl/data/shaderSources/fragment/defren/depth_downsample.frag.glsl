#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec2 pTCClamp;
UNIFORM_BIND(4) uniform float pMipMapLevel;
UNIFORM_BIND(5) uniform vec2 pPixelSize;

layout(binding=0) uniform HIGHP sampler2DArray texDepth;

#include "shared/interface/2d/fragment.glsl"

// !UseMinFunction
const vec4 weights = vec4(0.25);

void main(void){
	vec4 tc = vec4(vTexCoord, min(vTexCoord + pPixelSize, pTCClamp));
	vec4 depth;
	
	depth.x = textureLod(texDepth, vec3(tc.xy, vLayer), pMipMapLevel).r;
	depth.y = textureLod(texDepth, vec3(tc.zy, vLayer), pMipMapLevel).r;
	depth.z = textureLod(texDepth, vec3(tc.xw, vLayer), pMipMapLevel).r;
	depth.w = textureLod(texDepth, vec3(tc.zw, vLayer), pMipMapLevel).r;
	
	if(UseMinFunction){
		if(InverseDepth){
			depth.xy = max(depth.xy, depth.zw);
			gl_FragDepth = max(depth.x, depth.y);
			
		}else{
			depth.xy = min(depth.xy, depth.zw);
			gl_FragDepth = min(depth.x, depth.y);
		}
		
	}else{
		gl_FragDepth = dot(depth, weights);
	}
}
