#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// WithGI
#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"
#include "shared/defren/light/ubo_gi.glsl"
#include "shared/defren/gi/probe_flags.glsl"
#include "shared/octahedral.glsl"

layout(binding=0) uniform mediump sampler2D texPosition;
layout(binding=0) uniform mediump sampler2DArray texPositionArray;

layout(binding=1) uniform lowp sampler2D texDiffuse;
layout(binding=1) uniform lowp sampler2DArray texDiffuseArray;

layout(binding=2) uniform lowp sampler2D texNormal;
layout(binding=2) uniform lowp sampler2DArray texNormalArray;

layout(binding=3) uniform mediump sampler2D texEmissive;
layout(binding=3) uniform mediump sampler2DArray texEmissiveArray;

// WithGI
layout(binding=4) uniform lowp sampler2DArray texGIIrradiance;
layout(binding=5) uniform HIGHP sampler2DArray texGIDistance;

// WithGI
// includes to come after defining fixed position samplers
#define pGIGridProbeCount pGIParams[0].probeCount
#define TEX_GI_PROBE_OFFSET_BINDING 6
#include "shared/defren/gi/probe_offset.glsl"


VARYING_BIND(0) flat in int vLayer;

layout(location=0) out vec4 outColor;


// WithGI
#include "shared/defren/light/gi_illuminate.glsl"
#include "shared/normal/texture.glsl"


void main( void ){
	vec3 position, diffuse, normal;
	
	if(TextureEnvMapEqui){
		ivec2 tc = ivec2(gl_FragCoord);
		
		outColor = texelFetch(texEmissive, tc, 0);
		
		if(WithGI){
			position = vec3(texelFetch(texPosition, tc, 0));
			diffuse = vec3(texelFetch(texDiffuse, tc, 0));
			normal = normalLoadShiftedInt(texNormal, tc);
		}
		
	}else{
		ivec3 tc = ivec3(gl_FragCoord.xy, vLayer);
		
		outColor = texelFetch(texEmissiveArray, tc, 0);
		
		if(WithGI){
			position = vec3(texelFetch(texPositionArray, tc, 0));
			diffuse = vec3(texelFetch(texDiffuseArray, tc, 0));
			normal = normalLoadShiftedInt(texNormalArray, tc);
		}
	}
	
	if(WithGI){
		// if normal is 0 length pixel value has not been written to
		if(length(normal) > 0.001){
			outColor.rgb += giIlluminate(position, normal, normal) * diffuse;
		}
	}
}
