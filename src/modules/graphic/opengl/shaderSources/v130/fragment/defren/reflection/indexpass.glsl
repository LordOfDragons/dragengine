precision highp float;
precision highp int;



uniform vec4 pPosTransform;
uniform vec2 pPosTransform2;
uniform float pScaleDistance;

uniform vec3 pEnvMapPosition;
uniform int pEnvMapIndex;



uniform HIGHP sampler2DArray texDepth;
uniform mediump sampler2DArray texDistance;

#ifdef FULLSCREENQUAD
	in vec4 vScreenCoord;
#else
	in vec3 vVolumePos;
#endif

#ifdef GS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out ivec4 outIndices;



#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif



void main( void ){
	// determine position of fragment
	vec3 tcScreen( vScreenCoord.xy, vLayer );
	
	#ifdef DECODE_IN_DEPTH
		vec3 position = vec3( dot( texture( texDepth, tcScreen ).rgb, unpackDepth ) );
	#else
		//vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
		vec3 position = vec3( texture( texDepth, tcScreen ).r );
	#endif
	if( position.z == 1.0 ) discard; // temporary hack for not lit sky light
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	#ifdef FULLSCREENQUAD
		position.xy = ( vScreenCoord.zw + pPosTransform2 ) * pPosTransform.zw * position.zz;
	#else
		position.xy = vVolumePos.xy * position.zz / vVolumePos.zz;
	#endif
	
	// calculate the distance to the environment map position
	vec3 fragDir = position - pEnvMapPosition;
	float fragDist = length( fragDir ) * pScaleDistance;
	
	// clamp against the previous distance
	#ifdef CLAMP_AGAINST_DISTANCE
		ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
		if( fragDist <= texelFetch( texDistance, tc, 0 ).r ) discard;
	#endif
	
	// write out the results
	gl_FragDepth = fragDist;
	outIndices = ivec4( pEnvMapIndex );
}
