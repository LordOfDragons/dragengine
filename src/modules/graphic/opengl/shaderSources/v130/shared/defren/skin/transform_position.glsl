// Constants
//////////////

#ifdef BILLBOARD
	const vec3 bbUp = vec3( 0.0, 1.0, 0.0 );
#endif

#if defined HAS_TESSELLATION_SHADER || defined GS_RENDER_CUBE
	#define NO_TRANSFORMATION 1
#endif



// position transformation required by various vertex shaders to achieve the same result.
// for convenience this calculations are located in a function. The camera space position
// is returned in position while the transformed position is stored in gl_Position.
// gl_Position can be equal to position if tessellation is used.
void transformPosition( out vec3 position, in int spbIndex, in vec3 normal ){
	// prop fiels
	#ifdef PROP_FIELD
		// see doc/shader_propfield
		int instanceOffset = gl_InstanceID * 3;
		vec4 instance1 = texelFetch( texSubInstance1, instanceOffset ); // pos.x, pos.y, pos.z, sr.a13
		
		// check if the vertex belongs to a triangle to be culled. if so make it invisible
		#if 0
			#ifdef BILLBOARD
				#define DISCARD_OP <=
			#else
				#define DISCARD_OP >
			#endif
			if( length( pMatrixV * vec4( pMatrixModel * vec4( instance1.xyz, 1.0 ), 1.0 ) ) DISCARD_OP 5.0 ){
				position = vec3( 0.0, 0.0, 2.0 );
				gl_Position = vec4( position, 1.0 );
				return;
			}
		#endif
		
		// get the rest of the parameters
		vec4 instance2 = texelFetch( texSubInstance1, instanceOffset + 1 ); // sr.a11, sr.a21, sr.a31, sr.a23
		vec4 instance3 = texelFetch( texSubInstance1, instanceOffset + 2 ); // sr.a12, sr.a22, sr.a32, sr.a33
		vec4 bendState1 = texelFetch( texSubInstance2, gl_InstanceID );
		
		mat3 pfiRotScale = mat3( instance2.xyz, instance3.xyz, vec3( instance1.w, instance2.w, instance3.w ) );
		vec3 pfiPos = instance1.xyz;
		
		#ifdef BILLBOARD
			// precalculate some matrices used often
			mat4x3 matMV = mat4x3( mat4( pMatrixV ) * mat4( pMatrixModel ) );
			mat4x3 matRSMV = mat4x3( mat4( matMV ) * mat4( pfiRotScale ) );
			
			// calculate new instance matrix
			vec3 bbPos = matMV * vec4( pfiPos, 1.0 );
			mat3 matBBAxis = transpose( mat3( matRSMV ) );
			vec3 bbView = normalize( matBBAxis * bbPos ); // used to select the layer
			//vec3 bbView = normalize( matBBAxis[ 2 ] ); // used to select the layer
			vec3 bbRight = normalize( cross( bbUp, bbView ) );
			bbView = cross( bbRight, bbUp );
			mat3 bbMat = mat3( bbRight, bbUp, bbView );
			
			// this creates a screen aligned rotation which immitates true bending to a certain degree. for this the view
			// x axis is projected back to the bend coordinate frame. the bending is then considered the x and z coordinate
			// and projected onto the transformed x axis
			vec2 bbAxis = normalize( matBBAxis[ 0 ].xz );
			float bbRot = dot( bendState1.xy, bbAxis );
			mat2 matBend = mat2( cos( bbRot ), -sin( bbRot ), sin( bbRot ), cos( bbRot ) );
			position = bbMat * vec3( inPosition.xy * pBillboardPosTransform.xy + pBillboardPosTransform.zw, 0.0 );
			
		#else
			// create bend matrix
			vec2 bend = bendState1.xy * vec2( min( pPropFieldParams * length( inPosition ), 1.0 ) );
			vec4 bs = sin( vec4( bend, -bend ) );
			vec2 bc = cos( bend );
			mat3 matBend = mat3( bc.y, bs.y, 0.0, bc.x * bs.w, bc.x * bc.y, bs.x, bs.x * bs.y, bs.z * bc.y, bc.x );
			
			// calculate vertex position
			position = matBend * ( pfiRotScale * inPosition ) + pfiPos;
		#endif
		
	// height maps
	#elif defined( HEIGHT_MAP )
		position = vec3( inPosition.x, inHeight, inPosition.y );
		
	// everything else
	#else
		position = inPosition;
	#endif
	
	#ifdef BILLBOARD
		#ifdef PROP_FIELD
			position.xyz = matMV * vec4( pfiRotScale * position.xyz + pfiPos, 1.0 ) - bbPos;
			position.xy = matBend * position.xy;
			position.xyz += bbPos;
			
		#else
			vec2 coord = position.xy * pBillboardPosTransform.xy + pBillboardPosTransform.zw;
			vec3 refPos = pMatrixV * vec4( pMatrixModel[ 3 ], 1.0 );
			
			if( pBillboardParams.z ){ // sizeFixedToScreen
				coord *= vec2( refPos.z * pBillboardZScale );
			}
			
			if( pBillboardParams.x ){ // locked
				vec3 up = normalize( mat3( pMatrixV ) * pMatrixModel[ 1 ] );
				vec3 view = pBillboardParams.y ? normalize( refPos ) : vec3( 0.0, 0.0, 1.0 ); // spherical
				vec3 right = normalize( cross( up, view ) );
				
				position = refPos + right * coord.x + up * coord.y;
				
			}else{
				position = refPos + vec3( coord, 0.0 );
			}
		#endif
		
		// tessellation applies projection during evaluation stage so apply it only if not tessellation
		#ifdef NO_TRANSFORMATION
			gl_Position = vec4( position, 1.0 );
			
		#else
			gl_Position = pMatrixP * vec4( position, 1.0 );
		#endif
		
	#else
		position = pMatrixModel * vec4( position, 1.0 );
		
		// outline
		#ifdef WITH_OUTLINE
			position += normal * vec3( pOutlineThickness );
		#endif
		
		// tessellation applies projection during evaluation stage so apply it only if not tessellation
		#ifdef NO_TRANSFORMATION
			gl_Position = vec4( position, 1.0 );
			
		#else
			gl_Position = pMatrixVP * vec4( position, 1.0 );
		#endif
	#endif
}
