#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/gi/ubo_gi.glsl"
#include "v130/shared/octahedral.glsl"

uniform lowp sampler2D texGIIrradiance;
uniform HIGHP sampler2D texGIDistance;

in flat ivec3 vProbeCoord;
in flat vec3 vProbePosition;
in vec3 vDirection;

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
	vec2 texCoord = giTCFromDirection( normalize( vDirection ),
		vProbeCoord, pGIIrradianceMapScale, pGIIrradianceMapSize );
	vec3 irradiance = texture( texGIIrradiance, texCoord ).rgb;
	irradiance /= vec3( 4.0 ); // squash HDRR a bit
	outColor = irradiance;
}
