precision highp float;
precision highp int;

uniform mediump samplerBuffer texWeightMatrices;

in vec3 inPosition;
in vec3 inRealNormal;
in vec3 inNormal;
in vec4 inTangent;
in int inWeight;

out vec3 outPosition;
out vec3 outRealNormal;
out vec3 outNormal;
out vec4 outTangent;

void main( void ){
	gl_Position = vec4( 0, 0, 0, 1 );
	
	// if there is no weight write out all positions untransformed
	if( inWeight == -1 ){
		outPosition = inPosition;
		outRealNormal = inRealNormal;
		outNormal = inNormal;
		outTangent = inTangent;
		return;
	}
	
	// get the weight matrix
	vec4 row1 = texelFetch( texWeightMatrices, inWeight * 3 );
	vec4 row2 = texelFetch( texWeightMatrices, inWeight * 3 + 1 );
	vec4 row3 = texelFetch( texWeightMatrices, inWeight * 3 + 2 );
	mat3x4 matrix = mat3x4( row1, row2, row3 );
	
	// transform the position. this is correct and accurate
	outPosition = vec4( inPosition, 1 ) * matrix;
	
	// transform the normal and tangent. this is not correct and only an approximation
	// 
	// NOTE a quick solution is using if(v==vec3(0)) to check for degenerated case. this is though
	//      dangerous since the normalization is not done in this shader but inside another shader
	//      using the value written to VBO. This can result in values close to zero to become zero
	//      after the transformation. Or some calculation in the other shaders can be suspectible
	//      to values close to zero. Using if(dot(v,v)<0.001) is safer and allows to use a
	//      threshold value to protect against dangerously small vectors.
	mat3 matrixNormal = mat3( matrix );
	
	vec3 realNormal = inRealNormal * matrixNormal;
	outRealNormal = dot( realNormal, realNormal ) > 0.00001 ? realNormal : vec3( 0, 1, 0 );
	
	vec3 normal = inNormal * matrixNormal;
	outNormal = dot( normal, normal ) > 0.00001 ? normal : vec3( 0, 1, 0 );
	
	vec3 tangent = vec3( inTangent ) * matrixNormal;
	outTangent.xyz = dot( tangent, tangent ) > 0.00001 ? tangent : vec3( 1, 0, 0 );
	outTangent.w = inTangent.w;
}
