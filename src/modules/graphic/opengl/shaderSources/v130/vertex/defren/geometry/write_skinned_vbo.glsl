precision highp float;
precision highp int;

uniform HIGHP samplerBuffer texPositions;
uniform lowp sampler2D texNormalTangent;

uniform int pTexNorTanWidth;
uniform ivec2 pIndexOffset; // normal, tangent

in int inPositionIndex;
in int inNormalIndex;
in int inTangentIndex;
in float inNegateTangent;

out vec3 outPosition;
out vec3 outRealNormal;
out vec3 outNormal;
out vec4 outTangent;

void main( void ){
	outPosition = vec3( texelFetch( texPositions, inPositionIndex ) );
	
	ivec2 tcRealNormal = ivec2( inPositionIndex % pTexNorTanWidth, inPositionIndex / pTexNorTanWidth );
	vec3 realNormal = vec3( texelFetch( texNormalTangent, tcRealNormal, 0 ) );
	if( realNormal == vec3( 0.0 ) ){
		outRealNormal = vec3( 0.0, 1.0, 0.0 );
	}else{
		outRealNormal = realNormal;
	}
	
	int indexNormal = pIndexOffset.x + inNormalIndex;
	ivec2 tcNormal = ivec2( indexNormal % pTexNorTanWidth, indexNormal / pTexNorTanWidth );
	vec3 normal = vec3( texelFetch( texNormalTangent, tcNormal, 0 ) );
	if( normal == vec3( 0.0 ) ){
		outNormal = vec3( 0.0, 1.0, 0.0 );
	}else{
		outNormal = normal;
	}
	
	int indexTangent = pIndexOffset.y + inTangentIndex;
	ivec2 tcTangent = ivec2( indexTangent % pTexNorTanWidth, indexTangent / pTexNorTanWidth );
	vec3 tangent = vec3( texelFetch( texNormalTangent, tcTangent, 0 ) );
	if( tangent == vec3( 0.0 ) ){
		outTangent = vec4( 1.0, 0.0, 0.0, inNegateTangent );
	}else{
		outTangent = vec4( tangent, inNegateTangent );
	}
	
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
}
