precision highp float;
precision highp int;

layout( triangles ) in;
layout( triangle_strip, max_vertices=3 ) out;

uniform mat4x4 pMatrixMVP; // matrix model-view-projection
uniform mat4x3 pMatrixMV; // matrix model-view
uniform vec2 pZTransform;
uniform mat4x4 pMatrixP;

out float vZCoord;

void main( void ){
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = 0;
	
	vec3 center = ( vec3( gl_in[ 0 ].gl_Position ) + vec3( gl_in[ 1 ].gl_Position ) + vec3( gl_in[ 2 ].gl_Position ) ) / 3.0;
	vec3 displacement;
	float len;
	int i;
	
	for( i=0; i<3; i++ ){
		gl_Position = gl_in[ i ].gl_Position;
		
		displacement = vec3( vec2( vec3( gl_Position ) - center ), 0.0 );
		len = length( displacement );
		if( len > 0.0 ){
			gl_Position.xyz += ( displacement / len ) * 0.001;
		}
		
		vZCoord = gl_Position.z * pZTransform.x + pZTransform.y;
		gl_Position = pMatrixP * gl_Position;
		
		EmitVertex();
	}
	
	EndPrimitive();
}
