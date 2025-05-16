// Provides ray-casting support. requires this TBO layout:
//
// - tboGIRayCastNodeBox: RGBA16F (stride 2 pixels)
//   stores bounding box of all bvh nodes. each node uses two pixels: minExtend(0:RGB)
//   maxExtend(1:RGB). one component has to be wasted in each pixel due to format
//   restrictions. contains package nodes of all mesh BVH then nodes of instance BVH.
//   
// - tboGIRayCastIndex: RG16UI (stride 1 pixel)
//   stores bvh node indiced.
//   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
//      faces in TBOPrimitiveData (relative strided index). otherwise points to first child
//      node in tboGIRayCastNodeBox/tboGIRayCastIndex (relative strided index).
//   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
//      tboGIRayCastNodeBox/tboGIRayCastIndex (relative strided index). for child nodes
//      stays in instance BVH. for leaf nodes switches to mesh BVH traversal. points into
//      tboGIRayCastInstance and tboGIRayCastMatrix.
// 
// - tboGIRayCastInstance: RGBA32UI (stride 1 pixel)
//   stores instance offsets. bvhIndex(R) is the absolute strided index into tboGIRayCastNodeBox
//   and tboGIRayCastIndex with the mesh bvh root node. materialIndex(G) is the absolute strided
//   index into tboGIRayCastMaterial* of the first instance material. vertexIndex(B) is the absolute
//   strided index into tboGIRayCastVertex of the first instance vertex. faceIndex(A) is the absolute
//   strided index into tboGIRayCastFace and tboGIRayCastTexCoord of the first instance face.
//   
// - tboGIRayCastMatrix: RGBA16F (stride 3 pixels)
//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
//   
// - tboGIRayCastFace: RGBA16UI (stride 1 pixel)
//   stores mesh faces. vertex1(R) vertex2(G) vertex3(B) textureIndex(A). indices into
//   tboGIRayCastVertex. textureIndex is relative index into materialIndex.
// 
// - tboGIRayCastVertex: RGBA32F (stride 1 pixel)
//   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
// 
// - tboGIRayCastTexCoord: RG16F (stride 3 pixel)
//   stores mesh texture coordinates. same unstrided index as TBOFace.

/*
	<!-- shared/defren/gi/raycast/trace_ray.glsl -->
	<define>GI_RAYCAST_USE_SSBO</define>
	<define>GI_RAYCAST_DISTANCE_ONLY</define>
	<define>GI_RAYCAST_OCCMESH_ONLY</define>
*/

#ifdef GI_RAYCAST_USE_SSBO
	struct sGIRayCastNode{
		vec3 minExtend;
		int firstIndex;
		vec3 maxExtend;
		int primitiveCount;
	};
	
	struct sGIRayCastInstance{
		mat4x3 matrix;
		ivec4 indices; // nodes, material, vertices, faces
	};
	
	struct sGIRayCastFace{
		ivec4 indices; // rgb=(v1,v2,v3), a{doubleSided flag or materialIndex}
		vec2 tc1, tc2, tc3;
	};
	
	struct sGIRayCastVertex{
		vec3 position;
	};
	
	UBOLAYOUT readonly buffer GIRayCastNodes{
		sGIRayCastNode pGIRayCastNodes[];
	};
	UBOLAYOUT readonly buffer GIRayCastInstances{
		sGIRayCastInstance pGIRayCastInstances[];
	};
	UBOLAYOUT readonly buffer GIRayCastFaces{
		sGIRayCastFace pGIRayCastFaces[];
	};
	UBOLAYOUT readonly buffer GIRayCastVertices{
		sGIRayCastVertex pGIRayCastVertices[];
	};
};
	
#else
	uniform HIGHP samplerBuffer tboGIRayCastNodeBox;
	uniform HIGHP usamplerBuffer tboGIRayCastIndex;
	uniform HIGHP usamplerBuffer tboGIRayCastInstance;
	uniform HIGHP samplerBuffer tboGIRayCastMatrix;
	uniform HIGHP usamplerBuffer tboGIRayCastFace;
	uniform HIGHP samplerBuffer tboGIRayCastVertex;

	#ifndef GI_RAYCAST_DISTANCE_ONLY
		uniform HIGHP samplerBuffer tboGIRayCastTexCoord;
	#endif
#endif

// ray cast result
#ifdef GI_RAYCAST_DISTANCE_ONLY
	struct GIRayCastResult{
		float distance;
	};
	
#else
	struct GIRayCastResult{
		vec3 barycentric;
		float distance;
		vec3 normal;
		int face;
		int material;
	};
#endif


// constants
const float giRayCastNoHitDistance = 1000000.0;


// Node intersection test
float giRayCastBvhNodeHit( in vec3 minExtend, in vec3 maxExtend, in vec3 rayOrigin, in vec3 invRayDirection ){
	vec3 tbottom = ( minExtend - rayOrigin ) * invRayDirection;
	vec3 ttop = ( maxExtend - rayOrigin ) * invRayDirection;
	vec3 tmin = min( ttop, tbottom );
	vec3 tmax = max( ttop, tbottom );
	vec2 t = max( tmin.xx, tmin.yz );
	float t0 = max( max( t.x, t.y ), 0.0 );
	t = min( tmax.xx, tmax.yz );
	return min( t.x, t.y ) > t0 ? t0 : giRayCastNoHitDistance;
}


// Calculate texture coordinates of hit face.
#ifndef GI_RAYCAST_DISTANCE_ONLY
vec2 giRayCastFaceTexCoord( in int face, in vec3 barycentric ){
	#ifdef GI_RAYCAST_USE_SSBO
		return pGIRayCastFaces[ face ].tc1 * barycentric.x
			+ pGIRayCastFaces[ face ].tc2 * barycentric.y
			+ pGIRayCastFaces[ face ].tc3 * barycentric.z;
	#else
		int baseIndex = face * 3;
		vec2 tc1 = texelFetch( tboGIRayCastTexCoord, baseIndex ).xy;
		vec2 tc2 = texelFetch( tboGIRayCastTexCoord, baseIndex + 1 ).xy;
		vec2 tc3 = texelFetch( tboGIRayCastTexCoord, baseIndex + 2 ).xy;
		return tc1 * barycentric.x + tc2 * barycentric.y + tc3 * barycentric.z;
	#endif
}
#endif


// Perform ray cast against mesh BVH starting at absolute strided index.
// 
// Parameter indices contains indices required to trace rays in the model:
// - indices.x = root node index
// - indices.y = root material index
// - indices.z = root vertex index
// - indices.w = root face index
// 
// Returns true if hit is found otherwise false.
bool giRayCastTraceMesh( in ivec4 indices, in vec3 rayOrigin, in vec3 rayDirection,
in float distanceLimit, out GIRayCastResult result ){
	#define rootNode indices.x
	#define rootMaterial indices.y
	#define rootVertex indices.z
	#define rootFace indices.w
	vec3 invRayDirection = vec3( 1.0 ) / rayDirection;
	
	// early exit. if the ray misses the root box node skip the mesh entirely.
	// we do the test here and not in giRayCastTraceInstance since otherwise we have
	// to calculate invRayDirection twice
	{
	#ifdef GI_RAYCAST_USE_SSBO
		vec3 minExtend = pGIRayCastNodes[ rootNode ].minExtend;
		vec3 maxExtend = pGIRayCastNodes[ rootNode ].maxExtend;
	#else
		vec3 minExtend = texelFetch( tboGIRayCastNodeBox, rootNode * 2 ).xyz;
		vec3 maxExtend = texelFetch( tboGIRayCastNodeBox, rootNode * 2 + 1 ).xyz;
	#endif
	if( giRayCastBvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection ) >= distanceLimit ){
		return false;
	}
	}
	
	// continue ray casting against mesh faces
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	result.distance = distanceLimit;
	bool hasHit = false;
	
	// see giRayCastTraceInstance() for the information on why the code looks like this here
	
	/*
	while( curNode > -1 ){
		ivec2 index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy ); // firstIndex, primitiveCount
		
		if( index.y == 0 ){
#include "shared/defren/gi/raycast/inline_bvh_node.glsl"
			
		}else{
#include "shared/defren/gi/raycast/inline_bvh_model.glsl"
		}
		
		curNode = stack[ --stackPosition ];
	}
	*/
	
	while( curNode > -1 ){
		// node pass
		ivec2 index;
		while( curNode > -1 ){
			#ifdef GI_RAYCAST_USE_SSBO
				index = ivec2( pGIRayCastNodes[ curNode ].firstIndex, pGIRayCastNodes[ curNode ].primitiveCount );
			#else
				index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy ); // firstIndex, primitiveCount
			#endif
			if( index.y > 0 ){
				break;
			}
			index.x += rootNode;
#include "shared/defren/gi/raycast/inline_bvh_node.glsl"
			curNode = stack[ --stackPosition ];
		}
		
		// face pass
		if( curNode > -1 ){
			index.x += rootFace;
#include "shared/defren/gi/raycast/inline_bvh_model.glsl"
			curNode = stack[ --stackPosition ];
		}
	}
	
	//result.hitPoint = rayOrigin + rayDirection * result.distance;
	return hasHit;
	#undef rootNode
	#undef rootMaterial
	#undef rootVertex
	#undef rootFace
}


// Perform ray cast against mesh BVH starting at absolute strided index.
// 
// Parameter indices contains indices required to trace rays in the model:
// - indices.x = root node index
// - indices.y = root material index
// - indices.z = root vertex index
// - indices.w = root face index
// 
// Returns true if any hit is found otherwise false.
bool giRayCastTraceMeshAny( in ivec4 indices, in vec3 rayOrigin, in vec3 rayDirection, in float distanceLimit ){
	#define rootNode indices.x
	#define rootMaterial indices.y
	#define rootVertex indices.z
	#define rootFace indices.w
	vec3 invRayDirection = vec3( 1.0 ) / rayDirection;
	
	// early exit. if the ray misses the root box node skip the mesh entirely.
	// we do the test here and not in giRayCastTraceInstance since otherwise we have
	// to calculate invRayDirection twice
	{
	#ifdef GI_RAYCAST_USE_SSBO
		vec3 minExtend = pGIRayCastNodes[ rootNode ].minExtend;
		vec3 maxExtend = pGIRayCastNodes[ rootNode ].maxExtend;
	#else
		vec3 minExtend = texelFetch( tboGIRayCastNodeBox, rootNode * 2 ).xyz;
		vec3 maxExtend = texelFetch( tboGIRayCastNodeBox, rootNode * 2 + 1 ).xyz;
	#endif
	if( giRayCastBvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection ) >= distanceLimit ){
		return false;
	}
	}
	
	// continue ray casting against mesh faces
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	while( curNode > -1 ){
		// node pass
		ivec2 index;
		while( curNode > -1 ){
			#ifdef GI_RAYCAST_USE_SSBO
				index = ivec2( pGIRayCastNodes[ curNode ].firstIndex, pGIRayCastNodes[ curNode ].primitiveCount );
			#else
				index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy ); // firstIndex, primitiveCount
			#endif
			if( index.y > 0 ){
				break;
			}
			index.x += rootNode;
#include "shared/defren/gi/raycast/inline_bvh_node_any.glsl"
			curNode = stack[ --stackPosition ];
		}
		
		// face pass
		if( curNode > -1 ){
			index.x += rootFace;
#include "shared/defren/gi/raycast/inline_bvh_model_any.glsl"
			curNode = stack[ --stackPosition ];
		}
	}
	
	return false;
	#undef rootNode
	#undef rootMaterial
	#undef rootVertex
	#undef rootFace
}


// Perform ray cast against instance BVH starting at absolute strided index.
// 
// Returns true if hit is found otherwise false.
bool giRayCastTraceInstance( in int rootNode, in vec3 rayOrigin, in vec3 rayDirection,
in float distanceLimit, out GIRayCastResult result ){
	vec3 invRayDirection = vec3( 1.0 ) / rayDirection;
	
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	result.distance = distanceLimit;
	result.barycentric = vec3(0); // silence overzealous compiler warnings
	bool hasHit = false;
	
	// shaders run in wavefronts. each wavefront contains a group of threads. all these
	// threads are processed in lock-step. for branches this means first all threads
	// run through the if-branch where the condition is true then all threads run through
	// the else-branch where the condition is false. in the naive implementation in the
	// commented out part it takes only one thread to end up in the leaf-branch (which
	// is expensive) to block all other threads in the node-branch (which is cheap).
	// 
	// to improve the performance the branch processing has to be decoupled. in the code
	// used below a while-loop is used to add a kind of "soft-barrier". all threads have
	// to process nodes until they have found a node containing instances. this will
	// cause threads having found such a node quicker than others to wait until the last
	// one of them found a node but this is a small wait since node processing is fast.
	// once the last thread exited the while loop the "soft-barrier" finishes and all
	// thread continue to the next step.
	// 
	// in this step all threads having found a node with instance processe the instances.
	// here too some threads will be finished faster than others and have to wait for
	// the last one. altghough this part of the processing is expensive all threads
	// usually have instances to process and thus the bad situation of primitive processing
	// threads blocking node processing threads is reduced.
	// 
	// this modification improves performance by factor 2 or more depending on how the
	// BVH is structured
	
	/*
	while( curNode > -1 ){
		ivec2 index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy ); // firstIndex, primitiveCount
		
		if( index.y == 0 ){
#include "shared/defren/gi/raycast/inline_bvh_node.glsl"
			
		}else{
#include "shared/defren/gi/raycast/inline_bvh_instances.glsl"
		}
		
		curNode = stack[ --stackPosition ];
	}
	*/
	
	while( curNode > -1 ){
		// node pass
		ivec2 index;
		while( curNode > -1 ){
			#ifdef GI_RAYCAST_USE_SSBO
				index = ivec2( pGIRayCastNodes[ curNode ].firstIndex, pGIRayCastNodes[ curNode ].primitiveCount );
			#else
				index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy );
			#endif
			if( index.y > 0 ){
				break;
			}
			index.x += rootNode;
#include "shared/defren/gi/raycast/inline_bvh_node.glsl"
			curNode = stack[ --stackPosition ];
		}
		
		// instance pass
		if( curNode > -1 ){
#include "shared/defren/gi/raycast/inline_bvh_instances.glsl"
			curNode = stack[ --stackPosition ];
		}
	}
	
	return hasHit;
}


// Perform ray cast against instance BVH starting at absolute strided index.
// 
// Returns true if any hit is found otherwise false.
bool giRayCastTraceInstanceAny( in int rootNode, in vec3 rayOrigin, in vec3 rayDirection,
in float distanceLimit ){
	vec3 invRayDirection = vec3( 1.0 ) / rayDirection;
	
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	while( curNode > -1 ){
		// node pass
		ivec2 index;
		while( curNode > -1 ){
			#ifdef GI_RAYCAST_USE_SSBO
				index = ivec2( pGIRayCastNodes[ curNode ].firstIndex, pGIRayCastNodes[ curNode ].primitiveCount );
			#else
				index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy );
			#endif
			if( index.y > 0 ){
				break;
			}
			index.x += rootNode;
#include "shared/defren/gi/raycast/inline_bvh_node_any.glsl"
			curNode = stack[ --stackPosition ];
		}
		
		// instance pass
		if( curNode > -1 ){
#include "shared/defren/gi/raycast/inline_bvh_instances_any.glsl"
			curNode = stack[ --stackPosition ];
		}
	}
	
	return false;
}
