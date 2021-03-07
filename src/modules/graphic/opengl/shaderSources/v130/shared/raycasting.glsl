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
// - tboInstance: RG32UI (stride 1 pixel)
//   stores instance offsets. bvhIndex(R) is the absolute strided index into tboNodeBox
//   and tboIndex with the mesh bvh root node.materialIndex(G) is the absolute strided
//   index into TBOMaterial of the first instance material.
//   
// - tboMatrix: RGBA16F (stride 3 pixels)
//   stores instance matrixes. row1(0:RGBA) row2(1:RGBA) row3(2:RGBA).
//   
// - tboFace: RGBA16F (stride 3 pixels)
//   stores mesh faces. vertex1(0:RGB) vertex2(1:RGB) vertex3(2:RGB) materialIndex(0:A).
//   vertices are transformed by "current BVH transformation matrix". materialIndex is
//   relative to instance.
// 
// - tboVertex: RGBA32F (stride 1 pixel)
//   stores mesh vertices. vertices are transformed by "current BVH transformation matrix"
// 
// - tboTexCoord: RG16F (stride 3 pixel)
//   stores mesh texture coordinates. same unstrided index as TBOFace.
//
uniform samplerBuffer tboNodeBox;
uniform usamplerBuffer tboIndex;
uniform usamplerBuffer tboInstance;
uniform samplerBuffer tboMatrix;
uniform usamplerBuffer tboFace;
uniform samplerBuffer tboVertex;
uniform samplerBuffer tboTexCoord;

// - TBOMaterial: RGBA32UI (stride 1 pixel)
//   stores the instance material parameters in bit-field packed form. each pixel stores one
//   texture material. hence multiple consequitive pixels belong to one instance and the
//   number of textures per instance is variable.
//   
//   stores these material properties (bits required for each component and total bits):
//   - color.tint:              8 8 8     => 24
//   - color.gamma:             8         => 8
//   - roughness.remap.*:       8 8       => 16
//   - roughness.gamma:         8         => 8
//   - reflectivity.multiplier: 8         => 8
//   - emissivity.intensity:    16 16 16  => 48
//   - ignore:                  1         => 1
//   
//   also store the index of the material in the material texture atlas as 14 bits value.
//   this allows to combine it with variation.* bits to fill up 16 bits. 14 bits allows
//   for a maximum of 16383 materials. this equals a 128x128 atlas size or 16 pixel material
//   maps as the worst case
//   
//   gamma is limited to the range from 0.4 to 2.2 mapped to 8 bits. this maps pixel value
//   85 precisely to gamma value 1.
//   
//   pixel has a total of 128 bits. this leads to the following possible layout.
//   - red:
//     - 8: color.tint.r
//     - 8: roughness.remap.lower
//     - 16: emissivity.intensity.r
//   - green:
//     - 8: color.tint.g
//     - 8: roughness.remap.upper
//     - 16: emissivity.intensity.g
//   - blue:
//     - 8: color.tint.b
//     - 8: roughness.gamma
//     - 16: emissivity.intensity.b
//   - alpha:
//     - 8: color.gamma
//     - 8: reflectivity.multiplier
//     - 1: ignore material
//     - 1: (8reserved)
//     - 14: material atlas index
uniform usamplerBuffer tboMaterial;

// material atlas textures
uniform sampler2D texMaterialDiffuse; // diffuse.rgb, tintMask
uniform sampler2D texMaterialReflectivity; // reflectivity.rgb, roughness
uniform sampler2D texMaterialEmissivity; // emissivity.rgb


// ray cast result
struct RayCastResult{
	float distance;
	int face;
	int material;
	uvec4 materialParams; // encoded
	vec3 tc;
	vec3 normal;
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
bool rayCastMesh( in int rootNode, in int rootMaterial,
in vec3 rayOrigin, in vec3 rayDirection, out RayCastResult result ){
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
						int materialIndex = rootMaterial + corners.w;
						uvec4 materialParams = texelFetch( tboMaterial, materialIndex );
						
						if( ( materialParams.a & uint( 0x8000 ) ) != 0 ){
							continue; // ignore this material
						}
						
						result.distance = uvt.z;
						result.face = index.x + i;
						result.material = materialIndex;
						result.materialParams = materialParams;
						result.tc = uvt.wxy;
						result.normal = cross( e0, e1 ); // no normalize here since caller needs to do this anyways
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
				ivec2 indices = ivec2( texelFetch( tboInstance, tcInstance++ ).xy );
				
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
				
				if( rayCastMesh( indices.x, indices.y, meshRayOrigin, normalize( meshRayDirection ), meshResult ) ){
					// we can not use directly the distance since it is possible the matrix
					// contains scaling. we have to calculate the hit point in tracing space
					// and from there the distance can be calculated as difference between the
					// hit point and the ray origin which can be shortened. we just need to
					// know the amount of scaling used since rayDirection has unit length
					meshResult.distance /= length( meshRayDirection );
					
					if( meshResult.distance < result.distance ){
						// transform hit normal back. this requires the following matrix:
						//    matNor = transpose( inverse( mat3( matrix ) ) )
						// we have inverse(mat3(matrix)) already and the transpose we can also
						// skip by changing order
						result.distance = meshResult.distance;
						result.normal = normalize( meshResult.normal * mat3( invMatrix ) );
						result.material = meshResult.material;
						result.materialParams = meshResult.materialParams;
						result.face = meshResult.face;
						result.tc = meshResult.tc;
						
						hasHit = true;
					}
				}
			}
		}
		
		curNode = stack[ --stackPosition ];
	}
	
	return hasHit;
}

// Calculate texture coordinates of hit face.
vec2 rayCastFaceTexCoord( in RayCastResult result ){
	int baseIndex = result.face * 3;
	vec2 tc1 = texelFetch( tboTexCoord, baseIndex ).xy;
	vec2 tc2 = texelFetch( tboTexCoord, baseIndex + 1 ).xy;
	vec2 tc3 = texelFetch( tboTexCoord, baseIndex + 2 ).xy;
	return tc1 * result.tc.x + tc2 * result.tc.y + tc3 * result.tc.z;
}

// Sample material parameters using result
void rayCastSampleMaterial( in RayCastResult result, out vec3 diffuse, out float tintMask,
out vec3 reflectivity, out float roughness, out vec3 emissivity ){
	// calculate texture coordinates and material index from hit face
	vec2 texCoord = rayCastFaceTexCoord( result );
	
	// get material parameters
	uvec4 matParams = result.materialParams; //texelFetch( tboMaterial, result.material );
	
	vec4 colorGamma = vec4( matParams >> 24 )
		* vec4( 1.0 / 255.0, 1.0 / 255.0, 1.0 / 255.0, 1.8 / 255.0 )
		+ vec4( 0.0, 0.0, 0.0, 0.4 );
	vec4 roughnessGammaReflMul = vec4( ( matParams >> 16 ) & uvec4( 0xff ) )
		* vec4( 1.0 / 255.0, 1.0 / 255.0, 1.8 / 255.0, 1.0 / 255.0 )
		+ vec4( 0.0, 0.0, 0.4, 0.0 );
	vec3 emissivityIntensity = vec3( matParams.rgb & uvec3( 0xffff ) ) * vec3( 1.0 / 65535.0 );
	//bvec2 variation = notEqual( matParams.aa & uvec2( 0x8000, 0x4000 ), uvec2( 0 ) );
	int materialMapIndex = int( matParams.a & uint( 0x3fff ) );
	
	// sample material map from atlas
	ivec2 matMapTC = ivec2( materialMapIndex % pGIMaterialMapsPerRow, materialMapIndex / pGIMaterialMapsPerRow );
	matMapTC *= ivec2( pGIMaterialMapSize ); // base coord of material map
	matMapTC += ivec2( texCoord * vec2( pGIMaterialMapSize ) ) % ivec2( pGIMaterialMapSize );
	
	vec4 temp = texelFetch( texMaterialDiffuse, matMapTC, 0 );
	diffuse = temp.rgb;
	tintMask = temp.a;
	
	temp = texelFetch( texMaterialReflectivity, matMapTC, 0 );
	reflectivity = temp.rgb;
	roughness = temp.a;
	
	emissivity = texelFetch( texMaterialEmissivity, matMapTC, 0 ).rgb;
}
