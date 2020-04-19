precision highp float;
precision highp int;

uniform HIGHP samplerBuffer texWeightMatrices;

in vec3 inPosition;
in int inWeight;

out vec3 outPosition;

void main( void ){
	if( inWeight == -1 ){
		outPosition = inPosition;
		
	}else{
		vec4 row1 = texelFetch( texWeightMatrices, inWeight * 3 );
		vec4 row2 = texelFetch( texWeightMatrices, inWeight * 3 + 1 );
		vec4 row3 = texelFetch( texWeightMatrices, inWeight * 3 + 2 );
		mat3x4 matrix = mat3x4( row1, row2, row3 );
		
		outPosition = vec4( inPosition, 1.0 ) * matrix;
	}
	
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 ); // keep broken compilers happy
}
