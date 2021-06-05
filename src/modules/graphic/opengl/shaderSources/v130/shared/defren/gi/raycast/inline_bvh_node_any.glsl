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
			bool leftHit = giRayCastBvhNodeHitAny( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			#ifdef GI_RAYCAST_USE_SSBO
				minExtend = pGIRayCastNodes[ index.x + 1 ].minExtend;
				maxExtend = pGIRayCastNodes[ index.x + 1 ].maxExtend;
			#else
				minExtend = texelFetch( tboGIRayCastNodeBox, index.x * 2 + 2 ).xyz;
				maxExtend = texelFetch( tboGIRayCastNodeBox, index.x * 2 + 3 ).xyz;
			#endif
			bool rightHit = giRayCastBvhNodeHitAny( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			if( leftHit ){
				curNode = index.x; // left node
				if( rightHit ){
					stack[ stackPosition++ ] = index.x + 1; // right node
				}
				continue;
				
			}else if( rightHit ){
				curNode = index.x + 1; // right node
				continue;
			}
