// inline code processing a BVH node with primitives
// 
// samples required to be present:
// - samplerBuffer tboGIRayCastInstance
// - samplerBuffer tboGIRayCastMatrix
// 
// variables required to be present in scope before inline:
// - vec3 rayOrigin
// - vec3 rayDirection
// - ivec2 index: sampled node index (firstIndex, primitiveCount)
// - GIRayCastResult result
// - bool hasHit

			#ifndef GI_RAYCAST_USE_SSBO
				int tcMatrix = index.x * 3;
			#endif
			
			int i;
			for( i=0; i<index.y; i++ ){
				#ifndef GI_RAYCAST_USE_SSBO
					vec4 mrow1 = texelFetch( tboGIRayCastMatrix, tcMatrix++ );
					vec4 mrow2 = texelFetch( tboGIRayCastMatrix, tcMatrix++ );
					vec4 mrow3 = texelFetch( tboGIRayCastMatrix, tcMatrix++ );
				#endif
				
				// the inverse matrix needs to be transposed to be usable for right-side
				// multiplication. by switching to left-side multiplication the transpose
				// can be avoided which is faster
				#ifdef GI_RAYCAST_USE_SSBO
					mat4 invMatrix = mat4( pGIRayCastInstances[ index.x ].matrix, vec4( 0.0, 0.0, 0.0, 1.0 ) );
				#else
					mat4 invMatrix = mat4( mrow1, mrow2, mrow3, vec4( 0.0, 0.0, 0.0, 1.0 ) );
				#endif
				vec3 meshRayOrigin = vec3( vec4( rayOrigin, 1.0 ) * invMatrix );
				
				// for normal transformation the correct matrix to use is
				// transpose(inverse(mat3(invMatrix))). since we normalize the direction
				// we can avoid doing all this
				vec3 meshRayDirection = vec3( vec4( rayDirection, 0.0 ) * invMatrix );
				
				#ifdef GI_RAYCAST_USE_SSBO
					ivec4 instIndices = pGIRayCastInstances[ index.x ].indices;
				#else
					ivec4 instIndices = ivec4( texelFetch( tboGIRayCastInstance, index.x + i ) );
				#endif
				if( giRayCastTraceMeshAny( instIndices, meshRayOrigin, normalize( meshRayDirection ) ) ){
					return true;
				}
			}
