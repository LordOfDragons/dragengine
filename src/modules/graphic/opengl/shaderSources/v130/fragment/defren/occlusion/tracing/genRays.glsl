precision highp float;
precision highp int;

uniform int pRaysPerProbe;
uniform ivec3 pProbeCount;
uniform vec3 pProbeSpacing;
uniform int pProbesPerLine;
uniform mat3 pRandomOrientation;


out vec4 outRayOrigin;
out vec4 outRayDirection;


// NOTE findMSB exists since GLSL 4.0
#if __VERSION__ == 330 || __VERSION__ == 150 || __VERSION__ == 140 || __VERSION__ == 130
int findMSB( in int x ){
	int i;
	int mask;
	int res = -1;
	if( x < 0 ){
		x ~= x;
	}
	for( i=0; i<32; i++ ){
		mask = 0x80000000 >> i;
		if( x & mask ){
			res = 31 - i;
			break;
		}
	}
	return res;
}
#endif

// quasi uniform distribution of points on unit sphere
const float PI = 3.14159265;

vec3 sphericalFibonacci( in float i, in float n ){
	const float PHI = sqrt( 5 ) * 0.5 + 0.5;
	#define madfrac(A, B) ((A)*(B)-floor((A)*(B)))
	float phi = 2.0 * PI * madfrac( i, PHI - 1 );
	float cosTheta = 1.0 - ( 2.0 * i + 1.0 ) * ( 1.0 / n );
	float sinTheta = sqrt( clamp( 1.0 - cosTheta * cosTheta, 0.0, 1.0 ) );
	
	return vec3( cos( phi ) * sinTheta, sin( phi ) * sinTheta, cosTheta );
	#undef madfrac
}

vec3 gridCoordToPosition( ivec3 c ){
	return pProbeSpacing * vec3( c );
}

ivec3 probeIndexToGridCoord( in int index ){
	return ivec3( index & ( pProbeCount.x - 1 ),
		( index & ( ( pProbeCount.x * pProbeCount.y ) - 1 ) ) >> findMSB( pProbeCount.x ),
		index >> findMSB( pProbeCount.x * pProbeCount.y ) );
}

vec3 probeLocation( in int index ){
	return gridCoordToPosition( probeIndexToGridCoord( index ) );
}


void main( void ){
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
	outRayOrigin = vec4( probeLocation( tc.y / pProbesPerLine ), 0.08 /* bias */ );
	outRayDirection = vec4( pRandomOrientation * sphericalFibonacci( tc.x, pRaysPerProbe ), 100000.0 );
}
