precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/octahedral.glsl"
#include "shared/defren/light/ubo_gi.glsl"

uniform mat4x3 pMatrixNormal;
uniform int pGIDebugCascade;

#define pGIGridProbeCount pGIParams[pGIDebugCascade].probeCount
#include "shared/defren/gi/probe_offset.glsl"
#include "shared/defren/gi/probe_flags.glsl"

uniform lowp sampler2DArray texGIIrradiance;

flat in ivec3 vProbeCoord;
flat in vec3 vProbePosition;
in vec2 vTexCoord;
flat in float vRimThickness;

layout(location=0) out vec3 outColor;

vec2 giTCFromDirection( in vec3 dir, in ivec3 probeCoord, in vec2 mapScale, in int mapSize ){
	vec2 tc = octahedralEncode( dir ); // range [-1..1]
	tc = ( tc + vec2( 1.0 ) ) * 0.5; // range [0..1]
	tc *= vec2( mapSize ); // range [0..mapSize] (left border of left pixel to right border of right pixel)
	tc += vec2( 2 ); // offset by full map border and probe map border
	tc += vec2( pGIParams[pGIDebugCascade].probeCount.x * probeCoord.y
		+ probeCoord.x, probeCoord.z ) * vec2( mapSize + 2 );
	return tc * mapScale;
}

void main( void ){
	float radius = length( vTexCoord );
	if( radius > 1.0 ){
		discard; // outside sphere
	}
	
	if( radius > vRimThickness ){
		const float pi = 3.1415926538;
		float angle = 0.5 - atan( vTexCoord.x, -vTexCoord.y ) / ( pi * 2.0 );
		
		uint flags = gipoProbeFlags( vProbeCoord, pGIDebugCascade );
		
		if( angle < 0.35 || angle > 0.65 ){
			const vec3 colorEnabled = vec3( 0, 1, 0 );
			const vec3 colorDisabled = vec3( 1, 0, 0 );
			outColor = mix( colorEnabled, colorDisabled, bvec3( ( flags & gipfDisabled ) == gipfDisabled ) );
			
		}else{
			const vec3 colorOn = vec3( 0.5, 0, 0.5 );
			const vec3 colorOff = vec3( 0, 0, 0 );
			outColor = mix( colorOff, colorOn, bvec3( ( flags & gipfNearGeometry ) == gipfNearGeometry ) );
		}
		
	}else{
		vec3 direction = vec3( vTexCoord, sqrt( 1.0 - dot( vTexCoord, vTexCoord ) ) );
		direction.z = -direction.z; // make Z point away from camera
		direction = vec3( direction * pMatrixNormal ); // inverse order does transpose();
		
		vec3 texCoord = vec3( giTCFromDirection( normalize( direction ), vProbeCoord,
			pGIParams[pGIDebugCascade].irradianceMapScale,
			pGIParams[pGIDebugCascade].irradianceMapSize ), pGIDebugCascade );
		vec3 irradiance = texture( texGIIrradiance, texCoord ).rgb;
		irradiance = pow( irradiance, vec3( pGIParams[pGIDebugCascade].irradianceGamma ) );
		irradiance /= vec3( 4.0 ); // squash HDRR a bit
		outColor = irradiance;
	}
}
