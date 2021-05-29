#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/octahedral.glsl"
#include "v130/shared/defren/gi/probe_offset.glsl"
#include "v130/shared/defren/gi/probe_flags.glsl"

uniform lowp sampler2D texGIIrradiance;
uniform HIGHP sampler2D texGIDistance;

flat in ivec3 vProbeCoord;
flat in vec3 vProbePosition;
in vec3 vDirection;
in vec3 vCameraDirection;

out vec3 outColor;

vec2 giTCFromDirection( in vec3 dir, in ivec3 probeCoord, in vec2 mapScale, in int mapSize ){
	vec2 tc = octahedralEncode( dir ); // range [-1..1]
	tc = ( tc + vec2( 1.0 ) ) * 0.5; // range [0..1]
	tc *= vec2( mapSize ); // range [0..mapSize] (left border of left pixel to right border of right pixel)
	tc += vec2( 2 ); // offset by full map border and probe map border
	tc += vec2( pGIGridProbeCount.x * probeCoord.y + probeCoord.x, probeCoord.z ) * vec2( mapSize + 2 );
	return tc * mapScale;
}

void main( void ){
	#ifdef DEBUG_FLAGS
		const float pi = 3.1415926538;
		vec3 direction = normalize( vCameraDirection );
		float angle = 0.5 - atan( direction.x, -direction.y ) / ( pi * 2.0 );
		
		uint flags = gipoProbeFlags( vProbeCoord );
		
		if( angle < 0.35 || angle > 0.65 ){
			const vec3 colorEnabled = vec3( 0.0, 1.0, 0.0 );
			const vec3 colorDisabled = vec3( 1.0, 0.0, 0.0 );
			outColor = mix( colorEnabled, colorDisabled, bvec3( ( flags & gipfDisabled ) == gipfDisabled ) );
			
		}else{
			const vec3 colorOn = vec3( 0.0, 0.0, 1.0 );
			const vec3 colorOff = vec3( 0.0, 0.0, 0.0 );
			outColor = mix( colorOff, colorOn, bvec3( ( flags & gipfSmoothUpdate ) == gipfSmoothUpdate ) );
		}
		
	#else
		vec2 texCoord = giTCFromDirection( normalize( vDirection ),
			vProbeCoord, pGIIrradianceMapScale, pGIIrradianceMapSize );
		vec3 irradiance = texture( texGIIrradiance, texCoord ).rgb;
		irradiance = pow( irradiance, vec3( pGIIrradianceGamma ) );
		irradiance /= vec3( 4.0 ); // squash HDRR a bit
		outColor = irradiance;
	#endif
}
