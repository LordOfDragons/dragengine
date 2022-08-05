precision highp float;
precision highp int;

uniform ivec2 pTCClamp;
uniform int pMipMapLevel;

uniform HIGHP sampler2DArray texDepth;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

#ifdef DECODE_IN_DEPTH
	out vec4 outDepth;
#endif

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
	const vec3 packShift = vec3( 1.0, 256.0, 65536.0 );
	const vec3 packMask = vec3( 1.0 / 256.0, 1.0 / 256.0, 0.0 );
#endif
const ivec4 tcScale = ivec4( 2 );
const ivec4 tcOffset = ivec4( 0, 0, 1, 1 );
#ifndef USE_MIN_FUNCTION
	const vec4 weights = vec4( 0.25 );
#endif

void main( void ){
	ivec4 tc = min( ivec4( gl_FragCoord.xyxy ) * tcScale + tcOffset, pTCClamp.xyxy ); // s*2, t*2, s*2+1, t*2+1
	vec4 depth;
	
	#ifdef DECODE_IN_DEPTH
		depth.x = dot( texelFetch( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2, t*2)
		depth.y = dot( texelFetch( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2+1, t*2)
		depth.z = dot( texelFetch( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2, t*2+1)
		depth.w = dot( texelFetch( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ).rgb, unpackDepth ); // (s*2+1, t*2+1)
		
		#ifdef USE_MIN_FUNCTION
			#ifdef INVERSE_DEPTH
				depth.xy = max( depth.xy, depth.zw );
				vec3 encoded = fract( packShift * vec3( max( depth.x, depth.y ) ) );
			#else
				depth.xy = min( depth.xy, depth.zw );
				vec3 encoded = fract( packShift * vec3( min( depth.x, depth.y ) ) );
			#endif
		#else
			vec3 encoded = fract( packShift * vec3( dot( depth, weights ) ) );
		#endif
		outDepth = vec4( encoded - ( encoded.yzz * packMask ), 1.0 );
		
	#else
		depth.x = texelFetch( texDepth, ivec3( tc.xy, vLayer ), pMipMapLevel ).r; // (s*2, t*2)
		depth.y = texelFetch( texDepth, ivec3( tc.zy, vLayer ), pMipMapLevel ).r; // (s*2+1, t*2)
		depth.z = texelFetch( texDepth, ivec3( tc.xw, vLayer ), pMipMapLevel ).r; // (s*2, t*2+1)
		depth.w = texelFetch( texDepth, ivec3( tc.zw, vLayer ), pMipMapLevel ).r; // (s*2+1, t*2+1)
		
		#ifdef USE_MIN_FUNCTION
			#ifdef INVERSE_DEPTH
				depth.xy = max( depth.xy, depth.zw );
				gl_FragDepth = max( depth.x, depth.y );
			#else
				depth.xy = min( depth.xy, depth.zw );
				gl_FragDepth = min( depth.x, depth.y );
			#endif
		#else
			gl_FragDepth = dot( depth, weights );
		#endif
	#endif
}
