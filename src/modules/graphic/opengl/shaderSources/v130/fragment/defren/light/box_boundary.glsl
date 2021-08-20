precision highp float;
precision highp int;

#ifdef DEPTH_INPUT
	uniform vec4 pPosTransform;
	uniform mat3 pMatrixRotation;
	uniform vec3 pInitialMinValue;
	uniform vec3 pInitialMaxValue;
	uniform vec3 pClearMinValue;
	uniform vec3 pClearMaxValue;
#else
	uniform int pMipMapLevel;
#endif

#ifdef DEPTH_INPUT
	#ifdef AMBIENT_MAP
		#ifdef DEPTH_CUBEMAP
			uniform HIGHP samplerCube texAmbient;
		#else
			uniform HIGHP sampler2D texAmbient;
		#endif
	#endif
	
	#ifdef DEPTH_CUBEMAP
		uniform HIGHP samplerCube texDepth;
	#else
		uniform HIGHP sampler2D texDepth;
	#endif
#else
	uniform mediump sampler2D texMin;
	uniform mediump sampler2D texMax;
#endif

in vec2 vTexCoord;

out vec3 outMin;
out vec3 outMax;

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif



#ifdef DEPTH_INPUT
#ifdef DEPTH_CUBEMAP
void updateMinMaxFromDepth( in HIGHP samplerCube tex, in vec3 tc ){
	#ifdef DECODE_IN_DEPTH
		float dist = dot( textureLod( tex, tc, 0.0 ).rgb, unpackDepth );
	#else
		float dist = textureLod( tex, tc, 0.0 ).r;
	#endif
	
	dist = dist * pPosTransform.x + pPosTransform.y;
	
	vec3 position = tc * vec3( dist / length( tc ) );
	
	position = pMatrixRotation * position;
	
	outMin = min( outMin, position );
	outMax = max( outMax, position );
}

#else
void updateMinMaxFromDepth( in HIGHP sampler2D tex, in ivec2 tc ){
	#ifdef DECODE_IN_DEPTH
		vec3 position = vec3( dot( texelFetch( tex, tc, 0 ).rgb, unpackDepth ) );
	#else
		vec3 position = vec3( texelFetch( tex, tc, 0 ).r );
	#endif
	
	#ifdef SHADOW_INVERSE_DEPTH
		// clear value is 0 causing inf value. nothing can be 0 except clear value.
		// skipping the value though would result in wrong results if all merged points
		// have are clear values. in this case the initial min/max value is returned
		// which results in a tiny bounding box around the light center. this is obviously
		// wrong. to fix this a clear-value is defined by the light source. this value
		// equals the maximum boundary box and is a safe value
		if( position.z == 0.0 ){
			outMin = min( outMin, pClearMinValue );
			outMax = max( outMax, pClearMaxValue );
			return;
		}
	#endif
	
	position.z = pPosTransform.x / ( pPosTransform.y - position.z );
	position.xy = vTexCoord * pPosTransform.zw * position.zz;
	position = pMatrixRotation * position;
	
	outMin = min( outMin, position );
	outMax = max( outMax, position );
}
#endif // DEPTH_CUBEMAP
#endif // DEPTH_INPUT



#if defined DEPTH_INPUT && defined DEPTH_CUBEMAP
const vec2 tcFaceTransform = vec2( 1.0, -1.0 );
#endif

void main( void ){
	// min-max of depth input
	#ifdef DEPTH_INPUT
		outMin = pInitialMinValue;
		outMax = pInitialMaxValue;
		
		#ifdef DEPTH_CUBEMAP
			vec4 tc = vec4( vTexCoord, 1.0, -1.0 );
			vec3 tcoff = vec3( pPosTransform.z ) * vec3( 1.0, -1.0, 0.0 );
			
			// positive y = (s,-1,t), transform (1,1) same as above
			updateMinMaxFromDepth( texDepth, tc.xwy );
			updateMinMaxFromDepth( texDepth, tc.xwy + tcoff.xzz ); // x=1, y=0
			updateMinMaxFromDepth( texDepth, tc.xwy + tcoff.zzx ); // x=0, y=1
			updateMinMaxFromDepth( texDepth, tc.xwy + tcoff.xzx ); // x=1, y=1
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc.xwy );
				updateMinMaxFromDepth( texAmbient, tc.xwy + tcoff.xzz ); // x=1, y=0
				updateMinMaxFromDepth( texAmbient, tc.xwy + tcoff.zzx ); // x=0, y=1
				updateMinMaxFromDepth( texAmbient, tc.xwy + tcoff.xzx ); // x=1, y=1
			#endif
			
			// negative x = (-1,t,s), transform (1,1) same as above
			updateMinMaxFromDepth( texDepth, tc.wyx );
			updateMinMaxFromDepth( texDepth, tc.wyx + tcoff.zzx ); // x=1, y=0
			updateMinMaxFromDepth( texDepth, tc.wyx + tcoff.zxz ); // x=0, y=1
			updateMinMaxFromDepth( texDepth, tc.wyx + tcoff.zxx ); // x=1, y=1
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc.wyx );
				updateMinMaxFromDepth( texAmbient, tc.wyx + tcoff.zzx ); // x=1, y=0
				updateMinMaxFromDepth( texAmbient, tc.wyx + tcoff.zxz ); // x=0, y=1
				updateMinMaxFromDepth( texAmbient, tc.wyx + tcoff.zxx ); // x=1, y=1
			#endif
			
			// positive z = (s,t,1), transform (1,1) same as above
			updateMinMaxFromDepth( texDepth, tc.xyz );
			updateMinMaxFromDepth( texDepth, tc.xyz + tcoff.xzz ); // x=1, y=0
			updateMinMaxFromDepth( texDepth, tc.xyz + tcoff.zxz ); // x=0, y=1
			updateMinMaxFromDepth( texDepth, tc.xyz + tcoff.xxz ); // x=1, y=1
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc.xyz );
				updateMinMaxFromDepth( texAmbient, tc.xyz + tcoff.xzz ); // x=1, y=0
				updateMinMaxFromDepth( texAmbient, tc.xyz + tcoff.zxz ); // x=0, y=1
				updateMinMaxFromDepth( texAmbient, tc.xyz + tcoff.xxz ); // x=1, y=1
			#endif
			
			// negative z = (-s,t,-1), transform (-1,1)
			tc.xy = vTexCoord * tcFaceTransform.yx;
			updateMinMaxFromDepth( texDepth, tc.xyw );
			updateMinMaxFromDepth( texDepth, tc.xyw + tcoff.yzz ); // x=1, y=0
			updateMinMaxFromDepth( texDepth, tc.xyw + tcoff.zxz ); // x=0, y=1
			updateMinMaxFromDepth( texDepth, tc.xyw + tcoff.yxz ); // x=1, y=1
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc.xyw );
				updateMinMaxFromDepth( texAmbient, tc.xyw + tcoff.yzz ); // x=1, y=0
				updateMinMaxFromDepth( texAmbient, tc.xyw + tcoff.zxz ); // x=0, y=1
				updateMinMaxFromDepth( texAmbient, tc.xyw + tcoff.yxz ); // x=1, y=1
			#endif
			
			// positive x = (1,t,-s), transform (1,1) same as above
			updateMinMaxFromDepth( texDepth, tc.zyx );
			updateMinMaxFromDepth( texDepth, tc.zyx + tcoff.zzy ); // x=1, y=0
			updateMinMaxFromDepth( texDepth, tc.zyx + tcoff.zxz ); // x=0, y=1
			updateMinMaxFromDepth( texDepth, tc.zyx + tcoff.zxy ); // x=1, y=1
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc.zyx );
				updateMinMaxFromDepth( texAmbient, tc.zyx + tcoff.zzy ); // x=1, y=0
				updateMinMaxFromDepth( texAmbient, tc.zyx + tcoff.zxz ); // x=0, y=1
				updateMinMaxFromDepth( texAmbient, tc.zyx + tcoff.zxy ); // x=1, y=1
			#endif
			
			// negative y = (s,1,-t), transform (-1,-1)
			tc.xy = vTexCoord * tcFaceTransform.yy;
			updateMinMaxFromDepth( texDepth, tc.xzy );
			updateMinMaxFromDepth( texDepth, tc.xzy + tcoff.xzz ); // x=1, y=0
			updateMinMaxFromDepth( texDepth, tc.xzy + tcoff.zzy ); // x=0, y=1
			updateMinMaxFromDepth( texDepth, tc.xzy + tcoff.xzy ); // x=1, y=1
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc.xzy );
				updateMinMaxFromDepth( texAmbient, tc.xzy + tcoff.xzz ); // x=1, y=0
				updateMinMaxFromDepth( texAmbient, tc.xzy + tcoff.zzy ); // x=0, y=1
				updateMinMaxFromDepth( texAmbient, tc.xzy + tcoff.xzy ); // x=1, y=1
			#endif
			
		#else
			ivec2 tc = ivec2( gl_FragCoord.xy ) * ivec2( 2 );
			
			updateMinMaxFromDepth( texDepth, tc );
			updateMinMaxFromDepth( texDepth, tc + ivec2( 1, 0 ) );
			updateMinMaxFromDepth( texDepth, tc + ivec2( 0, 1 ) );
			updateMinMaxFromDepth( texDepth, tc + ivec2( 1, 1 ) );
			
			#ifdef AMBIENT_MAP
				updateMinMaxFromDepth( texAmbient, tc );
				updateMinMaxFromDepth( texAmbient, tc + ivec2( 1, 0 ) );
				updateMinMaxFromDepth( texAmbient, tc + ivec2( 0, 1 ) );
				updateMinMaxFromDepth( texAmbient, tc + ivec2( 1, 1 ) );
			#endif
		#endif
		
	#else
		ivec2 tc = ivec2( gl_FragCoord.xy ) * ivec2( 2 );
		
		outMin = texelFetch( texMin, tc, pMipMapLevel ).rgb;
		outMin = min( outMin, texelFetchOffset( texMin, tc, pMipMapLevel, ivec2( 1, 0 ) ).rgb );
		outMin = min( outMin, texelFetchOffset( texMin, tc, pMipMapLevel, ivec2( 0, 1 ) ).rgb );
		outMin = min( outMin, texelFetchOffset( texMin, tc, pMipMapLevel, ivec2( 1, 1 ) ).rgb );
		
		outMax = texelFetch( texMax, tc, pMipMapLevel ).rgb;
		outMax = max( outMax, texelFetchOffset( texMax, tc, pMipMapLevel, ivec2( 1, 0 ) ).rgb );
		outMax = max( outMax, texelFetchOffset( texMax, tc, pMipMapLevel, ivec2( 0, 1 ) ).rgb );
		outMax = max( outMax, texelFetchOffset( texMax, tc, pMipMapLevel, ivec2( 1, 1 ) ).rgb );
	#endif
}
