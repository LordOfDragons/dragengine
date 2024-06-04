// inline code processing a BVH node with primitives
// 
// samplers required to be present:
// - samplerBuffer tboGIRayCastFace
// - samplerBuffer tboGIRayCastVertex
// - samplerBuffer tboGIRayCastMaterialEmissivity
// 
// variables required to be present in scope before inline:
// - vec3 rayOrigin
// - vec3 rayDirection
// - ivec2 index: sampled node index (firstIndex absolute, primitiveCount)
// - GIRayCastResult result
// - bool hasHit
// - int rootVertex: absolute index of first vertex in TBO

			int i;
			for( i=0; i<index.y; i++ ){
				#ifdef GI_RAYCAST_USE_SSBO
					ivec4 corners = pGIRayCastFaces[ index.x ].indices;
				#else
					ivec4 corners = ivec4( texelFetch( tboGIRayCastFace, index.x + i ) );
				#endif
				corners.xyz += ivec3( rootVertex );
				
				#ifdef GI_RAYCAST_USE_SSBO
					vec3 v1 = pGIRayCastVertices[ corners.x ].position;
					vec3 v2 = pGIRayCastVertices[ corners.y ].position;
					vec3 v3 = pGIRayCastVertices[ corners.z ].position;
				#else
					vec3 v1 = texelFetch( tboGIRayCastVertex, corners.x ).xyz;
					vec3 v2 = texelFetch( tboGIRayCastVertex, corners.y ).xyz;
					vec3 v3 = texelFetch( tboGIRayCastVertex, corners.z ).xyz;
				#endif
				
				vec3 e0 = v2 - v1;
				vec3 e1 = v3 - v1;
				vec3 pv = cross( rayDirection, e1 );
				float det = dot( e0, pv );
				
				// check parallelity
				if( det != 0.0 ){
					vec3 tv = rayOrigin - v1;
					vec3 qv = cross( tv, e0 );
					
					vec4 uvt; // barycentric coordinates
					uvt.x = dot( tv, pv );
					uvt.y = dot( rayDirection, qv );
					uvt.z = dot( e1, qv );
					uvt.xyz /= det;
					uvt.w = 1.0 - uvt.x - uvt.y;
					
					if( all( greaterThanEqual( uvt, vec4( 0.0 ) ) ) && uvt.z < distanceLimit ){
						vec3 normal = cross( e0, e1 ); // no normalize here since caller needs to do this anyways
						bool frontface = dot( rayDirection, normal ) < 0.0;
						
						int material = rootMaterial + corners.w;
						uint params = giRayCastMaterialCastParams( material );
						uint flagsIgnore = frontface
							? giRayCastMatFlagIgnore
							: giRayCastMatFlagIgnore | giRayCastMatFlagIgnoreBackFace;
						
						if( ( params & flagsIgnore ) != uint( 0 ) ){
							continue;
						}
						
						int face = index.x + i;
						
						if( ( params & giRayCastMatFlagHasSolidity ) != uint( 0 ) ){
							ivec2 matTC = giRayCastMaterialTC( params, giRayCastTCTransform(
								material, giRayCastFaceTexCoord( face, uvt.wxy ) ) );
							
							if( texelFetch( tboGIRayCastMaterialEmissivity, matTC, 0 ).a < 0.5 /*0.35*/ ){ // solidity
								continue;
							}
						}
						return true;
					}
				}
			}
