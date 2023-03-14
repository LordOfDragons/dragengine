
layout (binding=0, std430, row_major) readonly buffer Input{
	uint pInput[];
};

layout (binding=1, std430, row_major) restrict writeonly buffer Output{
	uint pOutput[];
};

layout( local_size_x=64 ) in;

void main( void ){
	pOutput[ gl_GlobalInvocationID.x ] = pInput[ gl_GlobalInvocationID.x ] + uint( 800 );
}
