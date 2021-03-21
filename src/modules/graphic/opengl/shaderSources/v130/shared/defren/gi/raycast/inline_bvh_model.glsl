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
