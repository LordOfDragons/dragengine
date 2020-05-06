precision highp float;
precision highp int;

uniform HIGHP samplerBuffer texPositions;

uniform vec4 pTCTransform; // offsetX, scaleX, offsetY, scaleY
uniform int pOutputWidth;
uniform ivec2 pIndexOffset; // normal, tangent

in ivec3 inPositionIndices;
in ivec3 inNormalIndices;
in ivec3 inTangentIndices;
in vec2 inTangentFactors;

out vec3 vFaceNormal;
out vec3 vFaceTangent;
out vec2 vTCRealNormal1;
out vec2 vTCRealNormal2;
out vec2 vTCRealNormal3;
out vec2 vTCNormal1;
out vec2 vTCNormal2;
out vec2 vTCNormal3;
out vec2 vTCTangent1;
out vec2 vTCTangent2;
out vec2 vTCTangent3;

void main( void ){
	vec3 position1 = vec3( texelFetch( texPositions, inPositionIndices.x ) );
	vec3 position2 = vec3( texelFetch( texPositions, inPositionIndices.y ) );
	vec3 position3 = vec3( texelFetch( texPositions, inPositionIndices.z ) );
	
	vec3 edge1 = position2 - position1;
	vec3 edge2 = position3 - position1;
	
	vec3 faceNormal = cross( edge1, edge2 );
	float faceNormalLen = length( faceNormal );
	if( faceNormalLen < 0.001 ){
		vFaceNormal = vec3( 0.0, 1.0, 0.0 );
		
	}else{
		vFaceNormal = faceNormal / vec3( faceNormalLen );
	}
	
	vec3 faceTangent = edge1 * vec3( inTangentFactors.x ) - edge2 * vec3( inTangentFactors.y );
	float faceTangentLen = length( faceTangent );
	if( faceTangentLen < 0.001 ){
		vFaceTangent = vec3( 1.0, 0.0, 0.0 );
		
	}else{
		vFaceTangent = faceTangent / vec3( faceTangentLen );
	}
	
	ivec3 realNormalU = inPositionIndices % ivec3( pOutputWidth );
	ivec3 realNormalV = inPositionIndices / ivec3( pOutputWidth );
	vTCRealNormal1 = vec2( realNormalU.x, realNormalV.x ) * pTCTransform.xy + pTCTransform.zw;
	vTCRealNormal2 = vec2( realNormalU.y, realNormalV.y ) * pTCTransform.xy + pTCTransform.zw;
	vTCRealNormal3 = vec2( realNormalU.z, realNormalV.z ) * pTCTransform.xy + pTCTransform.zw;
	
	ivec3 indexNormals = ivec3( pIndexOffset.x ) + inNormalIndices;
	ivec3 normalU = indexNormals % ivec3( pOutputWidth );
	ivec3 normalV = indexNormals / ivec3( pOutputWidth );
	vTCNormal1 = vec2( normalU.x, normalV.x ) * pTCTransform.xy + pTCTransform.zw;
	vTCNormal2 = vec2( normalU.y, normalV.y ) * pTCTransform.xy + pTCTransform.zw;
	vTCNormal3 = vec2( normalU.z, normalV.z ) * pTCTransform.xy + pTCTransform.zw;
	
	ivec3 indexTangents = ivec3( pIndexOffset.y ) + inTangentIndices;
	ivec3 tangentU = indexTangents % ivec3( pOutputWidth );
	ivec3 tangentV = indexTangents / ivec3( pOutputWidth );
	vTCTangent1 = vec2( tangentU.x, tangentV.x ) * pTCTransform.xy + pTCTransform.zw;
	vTCTangent2 = vec2( tangentU.y, tangentV.y ) * pTCTransform.xy + pTCTransform.zw;
	vTCTangent3 = vec2( tangentU.z, tangentV.z ) * pTCTransform.xy + pTCTransform.zw;
	
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
}
