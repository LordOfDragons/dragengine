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
				ivec4 corners = ivec4( texelFetch( tboGIRayCastFace, index.x + i ) );
				corners.xyz += ivec3( rootVertex );
				
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
						vec3 normal = cross( e0, e1 ); // no normalize here since caller needs to do this anyways
						bool frontface = dot( rayDirection, normal ) < 0.0;
						
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
								uint flagsIgnore = frontface
									? giRayCastMatFlagIgnore | giRayCastMatFlagHasSolidity
									: giRayCastMatFlagIgnore | giRayCastMatFlagHasSolidity | giRayCastMatFlagIgnoreBackFace;
								
								if( ( params & flagsIgnore ) != uint( 0 ) ){
									continue;
								}
							#endif
							
						#else
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
								
								if( texelFetch( tboGIRayCastMaterialEmissivity, matTC, 0 ).a < 0.35 ){ // solidity
									continue;
								}
							}
							
							/*
							TODO fix backface after frontface hit problem
							
							add to GIRayCastResult value "distanceBack". this is set to maxHitDistance
							for GI_RAYCAST_DISTANCE_ONLY otherwise sample(texDistance).g
							
							on hit face check if face is frontface. if frontface set "distance" value.
							if backface set "distanceBack" to minimum of hit distance and "distanceBack".
							
							at end of ray cast store result of hit frontface. if GI_RAYCAST_DISTANCE_ONLY
							write out vec2(result.distance, result.distanceBack). otherwise write hit
							position as it is done now. if result.distance is less than or equal to
							result.distanceBack write result.distance as it is now otherwise -result.distanceBack.
							
							the cached ray thus writes now RG16F instead of R16F with the distance
							of the closest hit front face and the distance of the closest hit back
							face in front of the closest front face.
							
							for regular ray casting the written out result is always the front face
							result but with the distance of either the front or back face. since the
							normal is front face always the back face hit is indicated by negative
							distance. this has two uses.
							
							first the lighting of GI rays can skip now back-face hits too by checking
							if distance > 9999 or distance < 0 . this avoids spending time on lighting
							back faces as previously done
							
							second irradiance/distance map updating knows if backface is hit by
							just checking the sign of the samples distance. this avoids needing
							to sample the normal to figure this out. furthermore the actual
							distance to use is now abs(distance)
							
							this all allows to detect the situation of a solid front face with
							holes letting through rays which then hit a back face. the ray continues
							and records the hit with the next front face. 
							
							=> actually what we need is to store the distance of the first hit
							   geometry even if the ray passed through it. the distance has to
							   be negative if a back face is hit so we can later on detect if
							   the first encountered surface is front or back facing. the result
							   always stores the first hit front face result this way but keeping
							   track of the distance and type of the first encountered surface.
							   
							   this should be enough to properly apply the back-face probe disabling
							   while allowing rays to pass through holed and back facing geometry
							   to properly detect the distance.
							   
							=> we also need to support double-sided model faces. for this add a
							   new flag to the material parameters. treat hit face as front facing
							   if dot(rayDirection,normal)<0 or flag is set. this should have no
							   performance impact
							*/
							result.barycentric = uvt.wxy;
							result.face = face;
							result.material = material;
							result.normal = normal;
						#endif
						
						result.distance = uvt.z;
						hasHit = true;
					}
				}
			}
