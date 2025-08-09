#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// InputArrayTextures
layout(binding=0) uniform mediump sampler2DArray texHeightArray;

// !InputArrayTextures
layout(binding=0) uniform mediump sampler2D texHeight;

UNIFORM_BIND(3) uniform int pStepCount; // 128 or so
UNIFORM_BIND(4) uniform float pStepFactor; // 1.0 / ( float )pStepCount

// InputArrayTextures
UNIFORM_BIND(5) uniform float pLayer;

#include "shared/interface/2d/fragment.glsl"
VARYING_BIND(3) in vec2 vDestTC;

layout(location=0) out float outConeRatio; // use minimum blend function

vec4 sampleHeight(const in vec2 tc){
	if(InputArrayTextures){
		return texture(texHeightArray, vec3(tc, pLayer));
		
	}else{
		return texture(texHeight, tc);
	}
}

void main( void ){
	outConeRatio = 1.0;
	
	float destz = sampleHeight(vDestTC).r;
	if( destz > 0.999 ){ // prevent div-by-zero if destination is at the top
		return;
	}
	
	vec3 source = vec3( vTexCoord, 1.0 ); // start from the top
	vec3 destination = vec3( vDestTC, destz );
	vec3 direction = destination - source;
	
	// scale direction so it runs from destination to bottom and split it into the appropriate step size
	direction *= vec3( pStepFactor * destination.z / -direction.z );
	
	int i;
	for( i=0; i<pStepCount; i++ ){
		destination += direction;
		if( sampleHeight(vec2(destination)).r <= destination.z ){
			break;
		}
	}
	
	// destination has to be above source
	float diffz = destination.z - sampleHeight(vTexCoord).r;
	if( diffz > 0.001 ){
		outConeRatio = length( destination.xy - vTexCoord ) / diffz;
	}
}
