precision HIGHP float;
precision HIGHP int;

#ifdef WITH_LAYER
	uniform mediump sampler2DArray texHeight;
	#define SAMPLE_HEIGHT(tc) texture(texHeight, vec3(tc, pLayer))
#else
	uniform mediump sampler2D texHeight;
	#define SAMPLE_HEIGHT(tc) texture(texHeight, tc)
#endif

uniform int pStepCount; // 128 or so
uniform float pStepFactor; // 1.0 / ( float )pStepCount
#ifdef WITH_LAYER
uniform float pLayer;
#endif

in vec2 vTexCoord;
in vec2 vDestTC;

layout(location=0) out float outConeRatio; // use minimum blend function

void main( void ){
	outConeRatio = 1.0;
	
	float destz = SAMPLE_HEIGHT( vDestTC ).r;
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
		if( SAMPLE_HEIGHT( destination.st ).r <= destination.z ){
			break;
		}
	}
	
	// destination has to be above source
	float diffz = destination.z - SAMPLE_HEIGHT( vTexCoord ).r;
	if( diffz > 0.001 ){
		outConeRatio = length( destination.xy - vTexCoord ) / diffz;
	}
}
