// normal transformation. the result is written to vNormal. if a normal map exists vTangent and vBitangent
// are also calculated and written.
#if defined HAS_TESSELLATION_SHADER || defined GS_RENDER_CUBE
	#define NO_TRANSFORMATION 1
#endif

void transformNormal( in int spbIndex ){
	#ifdef BILLBOARD
		#ifdef PROP_FIELD
			vNormal = normalize( mat3( matRSMV ) * -bbMat[ 2 ] );
			#ifdef TEXTURE_NORMAL
				vTangent = normalize( mat3( matRSMV ) * bbMat[ 0 ] );
				vBitangent = cross( vNormal, vTangent );
			#endif
		#else
			vNormal = vec3( 0.0, 0.0, 1.0 );
			#ifdef TEXTURE_NORMAL
				vTangent = vec3( 1.0, 0.0, 0.0 );
				vBitangent = vec3( 0.0, 1.0, 0.0 );
			#endif
		#endif
		
	#else
		#ifdef HEIGHT_MAP
			vNormal = normalize( fract( vec3( 1.0, 256.0, 65536.0 ) * vec3( inNormal ) )
				* vec3( 1.9921569 ) + vec3( -0.9921722 ) );
		#else
			vNormal = normalize( inNormal );
		#endif
		#ifdef TEXTURE_NORMAL
			#ifdef HEIGHT_MAP
				vTangent = normalize( vec3( pMatrixTexCoord[0][0], 0.0, pMatrixTexCoord[0][1] ) );
				vBitangent = cross( vNormal, vTangent );
			#else
				vTangent = normalize( vec3( inTangent ) );
				vBitangent = cross( vNormal, vTangent );
				vTangent = cross( vBitangent, vNormal ) * vec3( inTangent.w );
			#endif
		#endif
		
		// the normalize is currently required since the pMatrixNor can contain scaling from a component. using the scaling though
		// "normal" is suddenly no more normalized resulting in render artifacts. there are only two solutions. one is to use
		// normalize as done here. the other solution is to calculate a normal matrix not containing any scaling. this solution
		// though is not so easy and has its own problems
		mat3 matrixNormal = pMatrixNormal;
		
		#ifdef PROP_FIELD
			// see doc/shader_propfield
			int instanceOffset = gl_InstanceID * 3;
			vec4 instance1 = texelFetch( texSubInstance1, instanceOffset ); // pos.x, pos.y, pos.z, sr.a13
			
			// get parameters. this is shared with position so it might be a good idea to somehow share it if possible
			vec4 instance2 = texelFetch( texSubInstance1, instanceOffset + 1 ); // sr.a11, sr.a21, sr.a31, sr.a23
			vec4 instance3 = texelFetch( texSubInstance1, instanceOffset + 2 ); // sr.a12, sr.a22, sr.a32, sr.a33
			vec4 bendState1 = texelFetch( texSubInstance2, gl_InstanceID );
			
			mat3 pfiRotScale = mat3( instance2.xyz, instance3.xyz, vec3( instance1.w, instance2.w, instance3.w ) );
			
			// create bend matrix
			vec2 bend = bendState1.xy * vec2( min( pPropFieldParams * length( inPosition ), 1.0 ) );
			vec4 bs = sin( vec4( bend, -bend ) );
			vec2 bc = cos( bend );
			mat3 matBend = mat3( bc.y, bs.y, 0.0, bc.x * bs.w, bc.x * bc.y, bs.x, bs.x * bs.y, bs.z * bc.y, bc.x );
			
			// calculate normal matrix
			matrixNormal = transpose( matBend * pfiRotScale ) * matrixNormal; // ~= inverted( matBend * pMatrixPropField )
		#endif
		
		#ifndef NO_TRANSFORMATION
			matrixNormal *= pMatrixVn;
		#endif
		
		vNormal = normalize( vNormal * matrixNormal );
		
		#ifdef TEXTURE_NORMAL
			vTangent = vTangent * matrixNormal;
			vBitangent = vBitangent * matrixNormal;
			
			#ifndef HEIGHT_MAP
				// using static or dynamic models it is possible cross(normal,tangent) becomes 0-vector.
				// doing normalize would result in inf values in the interpolation unit resulting in
				// all kinds of nasty problems on different gpu drivers. better use a degenerated vector.
				// this results in wrong lighting calculations but it does not blow up the gpu driver
				if( dot( vTangent, vTangent ) > 0.00001 ){
					vTangent = normalize( vTangent );
				}
				if( dot( vBitangent, vBitangent ) > 0.00001 ){
					vBitangent = normalize( vBitangent );
				}
			#endif
		#endif
	#endif
}
