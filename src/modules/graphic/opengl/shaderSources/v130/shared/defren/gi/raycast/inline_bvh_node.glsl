// inline code processing a BVH node with no primitives
// 
// samples required to be present:
// - samplerBuffer tboGIRayCastNodeBox
// 
// variables required to be present in scope before inline:
// - vec3 rayOrigin
// - vec3 invRayDirection
// - ivec2 index: sampled node index (firstIndex, primitiveCount)
// - int curNode
// - int stack[13]
// - int stackPosition

			#ifdef GI_RAYCAST_USE_SSBO
				vec3 minExtend = pGIRayCastNodes[ index.x ].minExtend;
				vec3 maxExtend = pGIRayCastNodes[ index.x ].maxExtend;
			#else
				vec3 minExtend = texelFetch( tboGIRayCastNodeBox, index.x * 2 ).xyz;
				vec3 maxExtend = texelFetch( tboGIRayCastNodeBox, index.x * 2 + 1 ).xyz;
			#endif
			float leftHit = giRayCastBvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			#ifdef GI_RAYCAST_USE_SSBO
				minExtend = pGIRayCastNodes[ index.x + 1 ].minExtend;
				maxExtend = pGIRayCastNodes[ index.x + 1 ].maxExtend;
			#else
				minExtend = texelFetch( tboGIRayCastNodeBox, index.x * 2 + 2 ).xyz;
				maxExtend = texelFetch( tboGIRayCastNodeBox, index.x * 2 + 3 ).xyz;
			#endif
			float rightHit = giRayCastBvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			if( leftHit < result.distance && rightHit < result.distance ){
				/*
				if( leftHit > rightHit ){
					curNode = index.x + 1; // right node
					stack[ stackPosition++ ] = index.x; // left node
					
				}else{
					curNode = index.x; // left node
					stack[ stackPosition++ ] = index.x + 1; // right node
				}
				*/
				curNode = leftHit > rightHit ? index.x + 1 : index.x;
				stack[ stackPosition++ ] = leftHit > rightHit ? index.x : index.x + 1;
				continue;
				
			}else if( leftHit < result.distance ){
				curNode = index.x; // left node
				continue;
				
			}else if( rightHit < result.distance ){
				curNode = index.x + 1; // right node
				continue;
			}
