precision highp float;
precision highp int;

uniform HIGHP samplerBuffer texPositions;

uniform vec4 pTCTransform; // offsetX, scaleX, offsetY, scaleY
uniform int pOutputWidth;
uniform int pNormalCount;

in ivec3 inPositionIndices;
in ivec3 inNormalIndices;
in ivec3 inTangentIndices;
in vec2 inTangentFactors;

out vec3 vFaceNormal;
out vec3 vFaceTangent;
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
	
	ivec3 normalU = inNormalIndices % ivec3( pOutputWidth );
	ivec3 normalV = inNormalIndices / ivec3( pOutputWidth );
	ivec3 tangentU = ( inTangentIndices + ivec3( pNormalCount ) ) % ivec3( pOutputWidth );
	ivec3 tangentV = ( inTangentIndices + ivec3( pNormalCount ) ) / ivec3( pOutputWidth );
	
	vTCNormal1 = vec2( normalU.x, normalV.x ) * pTCTransform.xy + pTCTransform.zw;
	vTCNormal2 = vec2( normalU.y, normalV.y ) * pTCTransform.xy + pTCTransform.zw;
	vTCNormal3 = vec2( normalU.z, normalV.z ) * pTCTransform.xy + pTCTransform.zw;
	
	vTCTangent1 = vec2( tangentU.x, tangentV.x ) * pTCTransform.xy + pTCTransform.zw;
	vTCTangent2 = vec2( tangentU.y, tangentV.y ) * pTCTransform.xy + pTCTransform.zw;
	vTCTangent3 = vec2( tangentU.z, tangentV.z ) * pTCTransform.xy + pTCTransform.zw;
	
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
}
