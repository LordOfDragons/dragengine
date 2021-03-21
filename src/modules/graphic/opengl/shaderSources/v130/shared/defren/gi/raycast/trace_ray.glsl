// Provides ray-casting support. requires this TBO layout:
//
// - tboGIRayCastNodeBox: RGBA16F (stride 2 pixels)
//   stores bounding box of all bvh nodes. each node uses two pixels: minExtend(0:RGB)
//   maxExtend(1:RGB). one component has to be wasted in each pixel due to format
//   restrictions. contains package nodes of all mesh BVH then nodes of instance BVH.
//   
// - tboGIRayCastIndex: RG32UI (stride 1 pixel)
//   stores bvh node indiced.
//   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
//      faces in TBOPrimitiveData (absolute strided index). otherwise points to first child
//      node in tboGIRayCastNodeBox/tboGIRayCastIndex (absolute strided index).
//   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
//      tboGIRayCastNodeBox/tboGIRayCastIndex (absolute strided index). for child nodes stays in instance BVH.
//      for leaf nodes switches to mesh BVH traversal. points into tboGIRayCastInstance and tboGIRayCastMatrix.
// 
// - tboGIRayCastInstance: RG32UI (stride 1 pixel)
//   stores instance offsets. bvhIndex(R) is the absolute strided index into tboGIRayCastNodeBox
//   and tboGIRayCastIndex with the mesh bvh root node.materialIndex(G) is the absolute strided
//   index into TBOMaterial of the first instance material.
//   
// - tboGIRayCastMatrix: RGBA16F (stride 3 pixels)
//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
//   
// - tboGIRayCastFace: RGBA16F (stride 3 pixels)
//   stores mesh faces. vertex1(0:RGB) vertex2(1:RGB) vertex3(2:RGB) materialIndex(0:A).
//   vertices are transformed by "current BVH transformation matrix". materialIndex is
//   relative to instance.
// 
// - tboGIRayCastVertex: RGBA32F (stride 1 pixel)
//   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
// 
// - tboGIRayCastTexCoord: RG16F (stride 3 pixel)
//   stores mesh texture coordinates. same unstrided index as TBOFace.
uniform samplerBuffer tboGIRayCastNodeBox;
uniform usamplerBuffer tboGIRayCastIndex;
uniform usamplerBuffer tboGIRayCastInstance;
uniform samplerBuffer tboGIRayCastMatrix;
uniform usamplerBuffer tboGIRayCastFace;
uniform samplerBuffer tboGIRayCastVertex;
uniform samplerBuffer tboGIRayCastTexCoord;


// ray cast result
struct GIRayCastResult{
	vec3 barycentric;
	float distance;
	vec3 normal;
	int face;
	int material;
};


// Node intersection test
float giRayCastBvhNodeHit( in vec3 minExtend, in vec3 maxExtend, in vec3 rayOrigin, in vec3 invRayDirection ){
	vec3 tbottom = ( minExtend - rayOrigin ) * invRayDirection;
	vec3 ttop = ( maxExtend - rayOrigin ) * invRayDirection;
	vec3 tmin = min( ttop, tbottom );
	vec3 tmax = max( ttop, tbottom );
	vec2 t = max( tmin.xx, tmin.yz );
	float t0 = max( max( t.x, t.y ), 0.0 );
	t = min( tmax.xx, tmax.yz );
	return min( t.x, t.y ) > t0 ? t0 : -1.0;
}


// Calculate texture coordinates of hit face.
vec2 giRayCastFaceTexCoord( in int face, in vec3 barycentric ){
	int baseIndex = face * 3;
	vec2 tc1 = texelFetch( tboGIRayCastTexCoord, baseIndex ).xy;
	vec2 tc2 = texelFetch( tboGIRayCastTexCoord, baseIndex + 1 ).xy;
	vec2 tc3 = texelFetch( tboGIRayCastTexCoord, baseIndex + 2 ).xy;
	return tc1 * barycentric.x + tc2 * barycentric.y + tc3 * barycentric.z;
}


// Perform ray cast against mesh BVH starting at absolute strided index.
// 
// Returns true if hit is found otherwise false.
bool giRayCastTraceMesh( in int rootNode, in int rootMaterial,
in vec3 rayOrigin, in vec3 rayDirection, out GIRayCastResult result ){
	vec3 invRayDirection = 1.0 / rayDirection;
	
	// early exit. if the ray misses the root box node skip the mesh entirely.
	// we do the test here and not in giRayCastTraceInstance since otherwise we have
	// to calculate invRayDirection twice
	{
	vec3 minExtend = texelFetch( tboGIRayCastNodeBox, rootNode * 2 ).xyz;
	vec3 maxExtend = texelFetch( tboGIRayCastNodeBox, rootNode * 2 + 1 ).xyz;
	if( giRayCastBvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection ) < 0.0 ){
		return false;
	}
	}
	
	// continue ray casting against mesh faces
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	result.distance = 1000000.0;
	bool hasHit = false;
	
	while( curNode > -1 ){
		ivec2 index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy ); // firstIndex, primitiveCount
		
		if( index.y == 0 ){
#include "v130/shared/defren/gi/raycast/inline_bvh_node.glsl"
			
		}else{
#include "v130/shared/defren/gi/raycast/inline_bvh_model.glsl"
			/*
			int i, tcFace = index.x;
			
			for( i=0; i<index.y; i++ ){
				ivec4 corners = ivec4( texelFetch( tboGIRayCastFace, tcFace++ ) );
				
				vec3 v1 = texelFetch( tboGIRayCastVertex, corners.x ).xyz;
				vec3 v2 = texelFetch( tboGIRayCastVertex, corners.y ).xyz;
				vec3 v3 = texelFetch( tboGIRayCastVertex, corners.z ).xyz;
				
				vec3 e0 = v2 - v1;
				vec3 e1 = v3 - v1;
				vec3 pv = cross( rayDirection, e1 );
				float det = dot( e0, pv );
				
				// check single sided and parallelity. parallelity check (det != 0) has to be
				// present always or division fails. single sided is tested using (det < 0).
				// if corners.w is 1 the face is double sided. if corners is 0 the face is single sided.
				// //if( det < 0.0 || ( corners.w == 1 && det > 0.0 ) ){
				// //if( any( lessThan( vec2( det, -det ) * vec2( 1, corners.w ), vec2( 0.0 ) ) ) ){
				// if( det < 0.0 || det * corners.w > 0.0 ){
				if( det != 0.0 ){
					vec3 tv = rayOrigin - v1;
					vec3 qv = cross( tv, e0 );
					
					vec4 uvt; // barycentric coordinates
					uvt.x = dot( tv, pv );
					uvt.y = dot( rayDirection, qv );
					uvt.z = dot( e1, qv );
					uvt.xyz /= det;
					uvt.w = 1.0 - uvt.x - uvt.y;
					
					if( all( greaterThanEqual( uvt, vec4( 0.0 ) ) ) && uvt.z < result.distance ){
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
						result.distance = uvt.z;
						result.face = face;
						result.material = material;
						result.normal = cross( e0, e1 ); // no normalize here since caller needs to do this anyways
						hasHit = true;
					}
				}
			}
			*/
		}
		
		curNode = stack[ --stackPosition ];
	}
	
	//result.hitPoint = rayOrigin + rayDirection * result.distance;
	return hasHit;
}


// Perform ray cast against instance BVH starting at absolute strided index.
// 
// Returns true if hit is found otherwise false.
bool giRayCastTraceInstance( in int rootNode, in vec3 rayOrigin, in vec3 rayDirection, out GIRayCastResult result ){
	vec3 invRayDirection = 1.0 / rayDirection;
	
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	result.distance = 1000000.0;
	bool hasHit = false;
	
	while( curNode > -1 ){
		ivec2 index = ivec2( texelFetch( tboGIRayCastIndex, curNode ).xy ); // firstIndex, primitiveCount
		
		if( index.y == 0 ){
#include "v130/shared/defren/gi/raycast/inline_bvh_node.glsl"
			
		}else{
#include "v130/shared/defren/gi/raycast/inline_bvh_instances.glsl"
		}
		
		curNode = stack[ --stackPosition ];
	}
	
	return hasHit;
}
