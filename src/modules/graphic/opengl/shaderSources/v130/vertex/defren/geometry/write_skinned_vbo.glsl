precision highp float;
precision highp int;

uniform HIGHP samplerBuffer texPositions;
uniform lowp sampler2D texNormalTangent;

uniform int pTexNorTanWidth;
uniform int pNormalCount;

in int inPositionIndex;
in int inNormalIndex;
in int inTangentIndex;
in float inNegateTangent;

out vec3 outPosition;
out vec3 outNormal;
out vec4 outTangent;

void main( void ){
	outPosition = vec3( texelFetch( texPositions, inPositionIndex ) );
	
	ivec2 tcNormal = ivec2( inNormalIndex % pTexNorTanWidth, inNormalIndex / pTexNorTanWidth );
	vec3 normal = vec3( texelFetch( texNormalTangent, tcNormal, 0 ) );
	if( normal == vec3( 0.0 ) ){
		outNormal = vec3( 0.0, 1.0, 0.0 );
	}else{
		outNormal = normal;
	}
	
	ivec2 tcTangent = ivec2( ( pNormalCount + inTangentIndex ) % pTexNorTanWidth, ( pNormalCount + inTangentIndex ) / pTexNorTanWidth );
	vec3 tangent = vec3( texelFetch( texNormalTangent, tcTangent, 0 ) );
	if( tangent == vec3( 0.0 ) ){
		outTangent = vec4( 1.0, 0.0, 0.0, inNegateTangent );
	}else{
		outTangent = vec4( tangent, inNegateTangent );
	}
	
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
}
