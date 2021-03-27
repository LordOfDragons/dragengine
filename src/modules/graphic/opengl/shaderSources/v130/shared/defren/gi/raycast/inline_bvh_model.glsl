// inline code processing a BVH node with primitives
// 
// samples required to be present:
// - samplerBuffer tboGIRayCastFace
// - samplerBuffer tboGIRayCastVertex
// - samplerBuffer tboGIRayCastMaterialEmissivity
// 
// variables required to be present in scope before inline:
// - vec3 rayOrigin
// - vec3 rayDirection
// - ivec2 index: sampled node index (firstIndex, primitiveCount)
// - GIRayCastResult result
// - bool hasHit

			int i;
			for( i=0; i<index.y; i++ ){
				ivec4 corners = ivec4( texelFetch( tboGIRayCastFace, index.x + i ) );
				
				vec3 v1 = texelFetch( tboGIRayCastVertex, corners.x ).xyz;
				vec3 v2 = texelFetch( tboGIRayCastVertex, corners.y ).xyz;
				vec3 v3 = texelFetch( tboGIRayCastVertex, corners.z ).xyz;
				
				vec3 e0 = v2 - v1;
				vec3 e1 = v3 - v1;
				vec3 pv = cross( rayDirection, e1 );
				float det = dot( e0, pv );
				
				#if defined GI_RAYCAST_DISTANCE_ONLY && defined GI_RAYCAST_OCCMESH_ONLY
					// check single sided and parallelity. parallelity check (det != 0) has to be
					// present always or division fails. single sided is tested using (det < 0).
					// if corners.w is 1 the face is double sided. if corners is 0 the face is
					// single sided:
					// 
					//   if( det < 0.0 || ( corners.w == 1 && det > 0.0 ) )
					// 
					// this can be simplified (if corners.w == 0 second part is always false):
					// 
					//   if( det < 0.0 || det * corners.w > 0.0 )
					//   
					// another version is this one here, but this is not much faster:
					//   
					//   if( any( lessThan( vec2( det, -det ) * vec2( 1.0, corners.w ), vec2( 0.0 ) ) ) )
					// 
					// the meaning of corners.w is different in the position only case to deal
					// with double sided faces
					if( det < 0.0 || det * corners.w > 0.0 )
				
				#else
					// check parallelity
					if( det != 0.0 )
				#endif
				{
					vec3 tv = rayOrigin - v1;
					vec3 qv = cross( tv, e0 );
					
					vec4 uvt; // barycentric coordinates
					uvt.x = dot( tv, pv );
					uvt.y = dot( rayDirection, qv );
					uvt.z = dot( e1, qv );
					uvt.xyz /= det;
					uvt.w = 1.0 - uvt.x - uvt.y;
					
					if( all( greaterThanEqual( uvt, vec4( 0.0 ) ) ) && uvt.z < result.distance ){
						#ifdef GI_RAYCAST_DISTANCE_ONLY
							#ifndef GI_RAYCAST_OCCMESH_ONLY
								// in the distance only case materials with solidity are ignored.
								// this is done like this because position only is used for ray
								// limit optimization and there we want stable limits that do not
								// change if texture coordinates are potentially transformed
								// 
								// for occlusion meshes this is not required but instead we need
								// to deal with double sided faces. this can be done using the
								// code above by changing the meaning of corners.w
								int material = rootMaterial + corners.w;
								uint params = giRayCastMaterialCastParams( material );
								
								if( ( params & ( giRayCastMatFlagIgnore | giRayCastMatFlagHasSolidity ) ) != 0 ){
									continue;
								}
							#endif
							
						#else
							int material = rootMaterial + corners.w;
							uint params = giRayCastMaterialCastParams( material );
							
							if( ( params & giRayCastMatFlagIgnore ) != 0 ){
								continue;
							}
							
							int face = index.x + i;
							
							if( ( params & giRayCastMatFlagHasSolidity ) != 0 ){
								ivec2 matTC = giRayCastMaterialTC( params, giRayCastTCTransform(
									material, giRayCastFaceTexCoord( face, uvt.wxy ) ) );
								
								if( texelFetch( tboGIRayCastMaterialEmissivity, matTC, 0 ).a < 0.35 ){ // solidity
									continue;
								}
							}
							
							result.barycentric = uvt.wxy;
							result.face = face;
							result.material = material;
							result.normal = cross( e0, e1 ); // no normalize here since caller needs to do this anyways
						#endif
						
						result.distance = uvt.z;
						hasHit = true;
					}
				}
			}
