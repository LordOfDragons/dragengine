precision highp float;
precision highp int;

layout( points ) in;
//#define HACK
#ifdef HACK
layout( triangle_strip, max_vertices=18 ) out;
const vec2 o1 = vec2(-0.001,0.001);
const float o2 = 0.002;
#else
layout( points, max_vertices=6 ) out;
#endif

in vec3 vFaceNormal[ 1 ];
in vec3 vFaceTangent[ 1 ];
in vec2 vTCRealNormal1[ 1 ];
in vec2 vTCRealNormal2[ 1 ];
in vec2 vTCRealNormal3[ 1 ];
in vec2 vTCNormal1[ 1 ];
in vec2 vTCNormal2[ 1 ];
in vec2 vTCNormal3[ 1 ];
in vec2 vTCTangent1[ 1 ];
in vec2 vTCTangent2[ 1 ];
in vec2 vTCTangent3[ 1 ];

out vec3 vVector;

void main( void ){
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = 0;
	
	gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
	
	// emit 3 points covering the 3 corner real normals to update
	vVector = vFaceNormal[ 0 ];
	
	gl_Position.xy = vTCRealNormal1[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	gl_Position.xy = vTCRealNormal2[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	gl_Position.xy = vTCRealNormal3[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	// emit 3 points covering the 3 corner normals to update
	vVector = vFaceNormal[ 0 ];
	
	gl_Position.xy = vTCNormal1[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	gl_Position.xy = vTCNormal2[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	gl_Position.xy = vTCNormal3[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	// emit 3 points covering the 3 corner tangents to update
	vVector = vFaceTangent[ 0 ];
	
	gl_Position.xy = vTCTangent1[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	gl_Position.xy = vTCTangent2[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
	
	gl_Position.xy = vTCTangent3[ 0 ];
	EmitVertex();
	#ifdef HACK
	gl_Position.xy+=o1; EmitVertex(); gl_Position.x+=o2; EmitVertex();
	#endif
	EndPrimitive();
}
