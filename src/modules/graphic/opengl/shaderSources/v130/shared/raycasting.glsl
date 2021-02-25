// // Provides ray-casting support. requires this TBO layout:
//
// - tboNodeBox: RGBA16F (stride 2 pixels)
//   stores bounding box of all bvh nodes. each node uses two pixels: minExtend(0:RGB)
//   maxExtend(1:RGB). one component has to be wasted in each pixel due to format
//   restrictions. contains package nodes of all mesh BVH then nodes of instance BVH.
//   
// - tboIndex: RG32UI (stride 1 pixel)
//   stores bvh node indiced.
//   1) all mesh bvh indices. firstIndex(R) primitiveCount(G). if leaf node points to mesh
//      faces in TBOPrimitiveData (absolute strided index). otherwise points to first child
//      node in tboNodeBox/tboIndex (absolute strided index).
//   2) all instance bvh indices. firstIndex(R) primitiveCount(G). points to next node in
//      tboNodeBox/tboIndex (absolute strided index). for child nodes stays in instance BVH.
//      for leaf nodes switches to mesh BVH traversal. points into tboInstance and tboMatrix.
// 
// - tboInstance: R32UI (stride 1 pixel)
//   stores instance offsets. bvhIndex(R) is the absolute strided index into tboNodeBox
//   and tboIndex with the mesh bvh root node.
//   
// - tboMatrix: RGBA16F (stride 3 pixels)
//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
//   
// - tboFace: RGBA16F (stride 3 pixels)
//   stores mesh faces. vertex1(0:RGB) vertex2(1:RGB) vertex3(2:RGB) doubleSided(0:A).
//   vertices are transformed by "current BVH transformation matrix". face is doubleSided
//   if doubleSided has value 1 or single sided if value is 0.
uniform samplerBuffer tboNodeBox;
uniform usamplerBuffer tboIndex;
uniform usamplerBuffer tboInstance;
uniform samplerBuffer tboMatrix;
uniform usamplerBuffer tboFace;
uniform samplerBuffer tboVertex;


// ray cast result
struct RayCastResult{
	//vec3 hitPoint;
	float distance;
	int face;
	vec3 tc;
};


// Node intersection test
float bvhNodeHit( in vec3 minExtend, in vec3 maxExtend, in vec3 rayOrigin, in vec3 invRayDirection ){
	vec3 tbottom = ( minExtend - rayOrigin ) * invRayDirection;
	vec3 ttop = ( maxExtend - rayOrigin ) * invRayDirection;
	vec3 tmin = min( ttop, tbottom );
	vec3 tmax = max( ttop, tbottom );
	vec2 t = max( tmin.xx, tmin.yz );
	float t0 = max( max( t.x, t.y ), 0.0 );
	t = min( tmax.xx, tmax.yz );
	return min( t.x, t.y ) > t0 ? t0 : -1.0;
}


// Perform ray cast against mesh BVH starting at absolute strided index.
// 
// Returns true if hit is found otherwise false.
bool rayCastMesh( in int rootNode, in vec3 rayOrigin, in vec3 rayDirection, out RayCastResult result ){
	vec3 invRayDirection = 1.0 / rayDirection;
	
	// early exit. if the ray misses the root box node skip the mesh entirely.
	// we do the test here and not in rayCastInstance since otherwise we have
	// to calculate invRayDirection twice
	int tcNodeBox = rootNode * 2;
	vec3 minExtend = texelFetch( tboNodeBox, tcNodeBox ).xyz;
	vec3 maxExtend = texelFetch( tboNodeBox, tcNodeBox + 1 ).xyz;
	if( bvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection ) < 0.0 ){
		return false;
	}
	
	// continue ray casting against mesh faces
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	result.distance = 1000000.0;
	bool hasHit = false;
	
	while( curNode > -1 ){
		ivec2 index = ivec2( texelFetch( tboIndex, curNode ).xy ); // firstIndex, primitiveCount
		
		if( index.y == 0 ){ // node
			tcNodeBox = index.x * 2;
			
			minExtend = texelFetch( tboNodeBox, tcNodeBox++ ).xyz;
			maxExtend = texelFetch( tboNodeBox, tcNodeBox++ ).xyz;
			float leftHit = bvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			minExtend = texelFetch( tboNodeBox, tcNodeBox++ ).xyz;
			maxExtend = texelFetch( tboNodeBox, tcNodeBox ).xyz;
			float rightHit = bvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			if( leftHit >= 0.0 && rightHit >= 0.0 ){
				if( leftHit > rightHit ){
					curNode = index.x + 1; // right node
					stack[ stackPosition++ ] = index.x; // left node
					
				}else{
					curNode = index.x; // left node
					stack[ stackPosition++ ] = index.x + 1; // right node
				}
				continue;
				
			}else if( leftHit >= 0.0 ){
				curNode = index.x; // left node
				continue;
				
			}else if( rightHit >= 0.0 ){
				curNode = index.x + 1; // right node
				continue;
			}
			
		}else{ // leaf
			int i, tcFace = index.x;
			
			for( i=0; i<index.y; i++ ){
				ivec4 corners = ivec4( texelFetch( tboFace, tcFace++ ) );
				
				vec3 v1 = texelFetch( tboVertex, corners.x ).xyz;
				vec3 v2 = texelFetch( tboVertex, corners.y ).xyz;
				vec3 v3 = texelFetch( tboVertex, corners.z ).xyz;
				
				vec3 e0 = v2 - v1;
				vec3 e1 = v3 - v1;
				vec3 pv = cross( rayDirection, e1 );
				float det = dot( e0, pv );
				
				// check single sided and parallelity. parallelity check (det != 0) has to be
				// present always or division fails. single sided is tested using (det < 0).
				// if corners.w is 1 the face is double sided. if corners is 0 the face is single sided.
				//if( det < 0.0 || ( corners.w == 1 && det > 0.0 ) ){
				//if( any( lessThan( vec2( det, -det ) * vec2( 1, corners.w ), vec2( 0.0 ) ) ) ){
				if( det < 0.0 || det * corners.w > 0.0 ){
					vec3 tv = rayOrigin - v1;
					vec3 qv = cross( tv, e0 );
					
					vec4 uvt; // barycentric coordinates
					uvt.x = dot( tv, pv );
					uvt.y = dot( rayDirection, qv );
					uvt.z = dot( e1, qv );
					uvt.xyz /= det;
					uvt.w = 1.0 - uvt.x - uvt.y;
					
					if( all( greaterThanEqual( uvt, vec4( 0.0 ) ) ) && uvt.z < result.distance ){
						result.distance = uvt.z;
						result.face = index.x + i;
						result.tc = uvt.wxy;
						hasHit = true;
					}
				}
			}
		}
		
		curNode = stack[ --stackPosition ];
	}
	
	//result.hitPoint = rayOrigin + rayDirection * result.distance;
	return hasHit;
}


// Perform ray cast against instance BVH starting at absolute strided index.
// 
// Returns true if hit is found otherwise false.
bool rayCastInstance( in int rootNode, in vec3 rayOrigin, in vec3 rayDirection, out RayCastResult result ){
	vec3 invRayDirection = 1.0 / rayDirection;
	
	int stack[ 13 ];
	stack[ 0 ] = -1;
	int stackPosition = 1;
	
	int curNode = rootNode;
	
	result.distance = 1000000.0;
	bool hasHit = false;
	
	while( curNode > -1 ){
		ivec2 index = ivec2( texelFetch( tboIndex, curNode ).xy ); // firstIndex, primitiveCount
		
		if( index.y == 0 ){ // node
			int tcNodeBox = index.x * 2;
			
			vec3 minExtend = texelFetch( tboNodeBox, tcNodeBox++ ).xyz;
			vec3 maxExtend = texelFetch( tboNodeBox, tcNodeBox++ ).xyz;
			float leftHit = bvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			minExtend = texelFetch( tboNodeBox, tcNodeBox++ ).xyz;
			maxExtend = texelFetch( tboNodeBox, tcNodeBox ).xyz;
			float rightHit = bvhNodeHit( minExtend, maxExtend, rayOrigin, invRayDirection );
			
			if( leftHit >= 0.0 && rightHit >= 0.0 ){
				if( leftHit > rightHit ){
					curNode = index.x + 1; // right node
					stack[ stackPosition++ ] = index.x; // left node
					
				}else{
					curNode = index.x; // left node
					stack[ stackPosition++ ] = index.x + 1; // right node
				}
				continue;
				
			}else if( leftHit >= 0.0 ){
				curNode = index.x; // left node
				continue;
				
			}else if( rightHit >= 0.0 ){
				curNode = index.x + 1; // right node
				continue;
			}
			
		}else{ // leaf
			int tcMatrix = index.x * 3;
			int tcInstance = index.x;
			RayCastResult meshResult;
			int i;
			
			for( i=0; i<index.y; i++ ){
				int bvhIndex = int( texelFetch( tboInstance, tcInstance++ ).x );
				
				vec4 mrow1 = texelFetch( tboMatrix, tcMatrix++ );
				vec4 mrow2 = texelFetch( tboMatrix, tcMatrix++ );
				vec4 mrow3 = texelFetch( tboMatrix, tcMatrix++ );
				
				// the inverse matrix needs to be transposed to be usable for right-side
				// multiplication. by switching to left-side multiplication the transpose
				// can be avoided which is faster
				mat4 invMatrix = mat4( mrow1, mrow2, mrow3, vec4( 0.0, 0.0, 0.0, 1.0 ) );
				vec3 meshRayOrigin = vec3( vec4( rayOrigin, 1.0 ) * invMatrix );
				
				// for normal transformation the correct matrix to use is
				// transpose(inverse(mat3(invMatrix))). since we normalize the direction
				// we can avoid doing all this
				vec3 meshRayDirection = vec3( vec4( rayDirection, 0.0 ) * invMatrix );
				
				if( rayCastMesh( bvhIndex, meshRayOrigin, normalize( meshRayDirection ), meshResult ) ){
					// we can not use directly the distance since it is possible the matrix
					// contains scaling. we have to calculate the hit point in tracing space
					// and from there the distance can be calculated as difference between the
					// hit point and the ray origin which can be shortened. we just need to
					// know the amount of scaling used since rayDirection has unit length
					meshResult.distance /= length( meshRayDirection );
					
					if( meshResult.distance < result.distance ){
						result = meshResult;
						hasHit = true;
					}
				}
			}
		}
		
		curNode = stack[ --stackPosition ];
	}
	
	return hasHit;
}
