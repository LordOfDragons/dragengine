#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

uniform mat4x4 pMatrixMVP; // matrix model-view-projection
uniform mat4x3 pMatrixMV; // matrix model-view
uniform vec2 pZTransform;
uniform mat4x4 pMatrixP;

out float vZCoord;


void emitCorner( in vec4 position, in vec3 center ){
	vec3 displacement = vec3( ( vec3( position ) - center ).xy, 0.0 );
	float len = length( displacement );
	if( len > 0.001 ){
		position.xyz += ( displacement / len ) * 0.001;
	}
	
	vZCoord = position.z * pZTransform.x + pZTransform.y;
	
	gl_Position = pMatrixP * position;
	
	gl_Layer = 0;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	EmitVertex();
}

void main( void ){
	vec3 center = ( vec3( gl_in[ 0 ].gl_Position )
		+ vec3( gl_in[ 1 ].gl_Position )
		+ vec3( gl_in[ 2 ].gl_Position ) ) / 3.0;
	
	int i;
	for( i=0; i<3; i++ ){
		emitCorner( gl_in[ i ].gl_Position, center );
	}
	
	EndPrimitive();
}
