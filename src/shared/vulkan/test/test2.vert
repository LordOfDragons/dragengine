#version 450

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec4 inPacked;

layout (location = 0) out vec3 vColor;

void main(){
	/*
	const vec3 positions[ 3 ] = vec3[ 3 ](
		vec3( 1, 1, 0 ),
		vec3( -1, 1, 0 ),
		vec3( 0, -1, 0 )
	);
	
	gl_Position = vec4( positions[ gl_VertexIndex ], 1 );
	*/
	
	gl_Position = vec4( inPosition, 1 );
	vColor = inColor;
}
