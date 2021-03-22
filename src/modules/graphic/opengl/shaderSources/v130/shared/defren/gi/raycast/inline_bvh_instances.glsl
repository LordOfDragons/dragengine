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

			int i, tcMatrix = index.x * 3;
			
			for( i=0; i<index.y; i++ ){
				ivec2 indices = ivec2( texelFetch( tboGIRayCastInstance, index.x + i ).xy );
				
				vec4 mrow1 = texelFetch( tboGIRayCastMatrix, tcMatrix++ );
				vec4 mrow2 = texelFetch( tboGIRayCastMatrix, tcMatrix++ );
				vec4 mrow3 = texelFetch( tboGIRayCastMatrix, tcMatrix++ );
				
				// the inverse matrix needs to be transposed to be usable for right-side
				// multiplication. by switching to left-side multiplication the transpose
				// can be avoided which is faster
				mat4 invMatrix = mat4( mrow1, mrow2, mrow3, vec4( 0.0, 0.0, 0.0, 1.0 ) );
				vec3 meshRayOrigin = vec3( vec4( rayOrigin, 1.0 ) * invMatrix );
				
				// for normal transformation the correct matrix to use is
				// transpose(inverse(mat3(invMatrix))). since we normalize the direction
				// we can avoid doing all this
				vec3 meshRayDirection = vec3( vec4( rayDirection, 0.0 ) * invMatrix );
				
				GIRayCastResult meshResult;
				if( giRayCastTraceMesh( indices.x, indices.y, meshRayOrigin,
						normalize( meshRayDirection ), result.distance, meshResult ) ){
					// we can not use directly the distance since it is possible the matrix
					// contains scaling. we have to calculate the hit point in tracing space
					// and from there the distance can be calculated as difference between the
					// hit point and the ray origin which can be shortened. we just need to
					// know the amount of scaling used since rayDirection has unit length
					meshResult.distance /= length( meshRayDirection );
					
					if( meshResult.distance < result.distance ){
						// transform hit normal back. this requires the following matrix:
						//    matNor = transpose( inverse( mat3( matrix ) ) )
						// we have invMatrix which is inverse(matrix) but without transpose.
						// so invMatrix is actually equal to transpose(inverse(matrix)).
						// this is neat since now mat3(invMatrix) is actually matNor.
						// since the transpose cancelled out it is not required to reverse
						// the operation order to get the transpose
						result.barycentric = meshResult.barycentric;
						result.distance = meshResult.distance;
						result.face = meshResult.face;
						result.material = meshResult.material;
						result.normal = normalize( mat3( invMatrix ) * meshResult.normal );
						hasHit = true;
					}
				}
			}
