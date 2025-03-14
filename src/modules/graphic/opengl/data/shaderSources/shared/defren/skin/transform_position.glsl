// Constants
//////////////

#ifdef BILLBOARD
	const vec3 bbUp = vec3( 0.0, 1.0, 0.0 );
#endif

#if defined HAS_TESSELLATION_SHADER || defined GS_RENDER_CUBE || defined GS_RENDER_CASCADED || defined GS_RENDER_STEREO
	#define NO_TRANSFORMATION 1
#endif



// position transformation required by various vertex shaders to achieve the same result.
// for convenience this calculations are located in a function. The camera space position
// is returned in position while the transformed position is stored in gl_Position.
// gl_Position can be equal to position if tessellation is used.
#if defined PROP_FIELD && defined BILLBOARD
#define REQUIRES_TRANSFORM_TRANSFER 1
#endif

#ifdef REQUIRES_TRANSFORM_TRANSFER
struct sTransformTransfer{
	mat4x3 matRSMV;
	mat3 bbMat;
};
#endif

#ifdef REQUIRES_TRANSFORM_TRANSFER
void transformPosition( out vec3 position, in int spbIndex, out sTransformTransfer transformTransfer )
#else
void transformPosition( out vec3 position, in int spbIndex )
#endif
{
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
			if( length( pMatrixV[ inLayer ] * vec4( pMatrixModel * vec4( instance1.xyz, 1 ), 1 ) ) DISCARD_OP 5 ){
				position = vec3( 0, 0, 2 );
				gl_Position = vec4( position, 1 );
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
			mat4x3 matMV = mat4x3( mat4( pMatrixV[ inLayer ] ) * mat4( pMatrixModel ) );
			mat4x3 matRSMV = mat4x3( mat4( matMV ) * mat4( pfiRotScale ) );
			
			// calculate new instance matrix
			vec3 bbPos = matMV * vec4( pfiPos, 1 );
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
			position = bbMat * vec3( inPosition.xy * pBillboardPosTransform.xy + pBillboardPosTransform.zw, 0 );
			
			transformTransfer.matRSMV = matRSMV;
			transformTransfer.bbMat = bbMat;
			
		#else
			// create bend matrix
			vec2 bend = bendState1.xy * vec2( min( pPropFieldParams * length( inPosition ), 1.0 ) );
			vec4 bs = sin( vec4( bend, -bend ) );
			vec2 bc = cos( bend );
			mat3 matBend = mat3( bc.y, bs.y, 0, bc.x * bs.w, bc.x * bc.y, bs.x, bs.x * bs.y, bs.z * bc.y, bc.x );
			
			// calculate vertex position
			position = matBend * ( pfiRotScale * inPosition ) + pfiPos;
		#endif
		
	// height maps
	#elif defined HEIGHT_MAP
		position = vec3( inPosition.x, inHeight, inPosition.y );
		
	// everything else
	#else
		position = inPosition;
	#endif
	
	#ifdef BILLBOARD
		#ifdef PROP_FIELD
			position.xyz = matMV * vec4( pfiRotScale * position.xyz + pfiPos, 1 ) - bbPos;
			position.xy = matBend * position.xy;
			position.xyz += bbPos;
			
		#else
			// using stereo rendering the left and right view have to use the same billboard
			// orientation or the rendering breaks. for this reason the left view camera matrix
			// is used. during geometry shader the position has to be fixed for the right view
			// otherwise rendering breaks
			vec2 coord = position.xy * pBillboardPosTransform.xy + pBillboardPosTransform.zw;
			vec3 refPos = pMatrixV[ inLayer ] * vec4( pMatrixModel[ 3 ], 1 );
			
			if( pBillboardParams.z ){ // sizeFixedToScreen
				coord *= vec2( refPos.z * pBillboardZScale );
			}
			
			if( pBillboardParams.x ){ // locked
				vec3 up = normalize( mat3( pMatrixV[ inLayer ] ) * pMatrixModel[ 1 ] );
				vec3 view = pBillboardParams.y ? normalize( refPos ) : vec3( 0, 0, 1 ); // spherical
				vec3 right = normalize( cross( up, view ) );
				
				position = refPos + right * coord.x + up * coord.y;
				
			}else{
				position = refPos + vec3( coord, 0 );
			}
		#endif
		
		// tessellation applies projection during evaluation stage so apply it only if not tessellation
		position = sanitizePosition( position );
		
		#ifdef NO_TRANSFORMATION
			gl_Position = vec4( position, 1 );
			
		#else
			gl_Position = pMatrixP[ inLayer ] * vec4( position, 1 );
		#endif
		
	#else
		position = pMatrixModel * vec4( position, 1 );
		
		// outline. done after matrix model to avoid scaling affecting the result
		#ifdef WITH_OUTLINE
			float outlineThickness = pOutlineThickness;
			#ifdef WITH_OUTLINE_THICKNESS_SCREEN
				// we can use pBillboardZScale since this is tan(camera.fov / 2)
				// using the first camera matrix since the stereo camera matrix produces the
				// same result. even if slightly different both views have to use the same
				// thickness or it looks wrong
				outlineThickness *= ( pMatrixV[ inLayer ] * vec4( position, 1 ) ).z * pBillboardZScale;
			#endif
			position.xyz += normalize( inRealNormal * pMatrixNormal ) * vec3( outlineThickness );
		#endif
		
		// tessellation applies projection during evaluation stage so apply it only if not tessellation
		position = sanitizePosition( position );
		
		#ifdef NO_TRANSFORMATION
			gl_Position = vec4( position, 1 );
			
		#else
			gl_Position = pMatrixVP[ inLayer ] * vec4( position, 1 );
		#endif
	#endif
}
