#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#ifdef SHADOW_CUBE
	#define SAMPLER samplerCube
#else
	#define SAMPLER sampler2D
#endif

uniform HIGHP SAMPLER texShadow;
uniform mediump SAMPLER texColor;

VARYING_BIND(0) in vec3 vTexCoord;

#ifdef COPY_COLOR
	layout(location=0) out vec4 outColor;
#endif

void main(){
	#ifdef SHADOW_CUBE
		gl_FragDepth = textureLod(texShadow, vTexCoord, 0.0).r;
		
		#ifdef COPY_COLOR
			outColor = textureLod(texColor, vTexCoord, 0.0);
		#endif
		
	#else
		ivec2 tc = ivec2(gl_FragCoord);
		
		gl_FragDepth = texelFetch(texShadow, ivec2(gl_FragCoord), 0).r;
		
		#ifdef COPY_COLOR
			outColor = texelFetch(texColor, ivec2(gl_FragCoord), 0);
		#endif
	#endif
}
