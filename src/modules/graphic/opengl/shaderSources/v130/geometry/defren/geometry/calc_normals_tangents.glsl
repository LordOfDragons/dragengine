precision highp float;
precision highp int;

layout( points ) in;

//#define HACK
#ifdef HACK
	layout( triangle_strip, max_vertices=18 ) out;
	const vec2 o1 = vec2( -0.001, 0.001 );
	const vec2 o2 = vec2(  0.001, 0.001 );
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


void emitCorner( in vec2 position, in vec3 vector ){
	vVector = vector;
	gl_Position = vec4( position, 0.0, 1.0 );
	gl_Layer = 0;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	EmitVertex();
}

void emitTriangle( in vec2 position, in vec3 vector ){
	emitCorner( position, vector );
	#ifdef HACK
	emitCorner( position + o1, vector );
	emitCorner( position + o2, vector );
	#endif
	EndPrimitive();
}

void main( void ){
	// emit 3 points covering the 3 corner real normals to update
	emitTriangle( vTCRealNormal1[ 0 ], vFaceNormal[ 0 ] );
	emitTriangle( vTCRealNormal2[ 0 ], vFaceNormal[ 0 ] );
	emitTriangle( vTCRealNormal3[ 0 ], vFaceNormal[ 0 ] );
	
	// emit 3 points covering the 3 corner normals to update
	emitTriangle( vTCNormal1[ 0 ], vFaceNormal[ 0 ] );
	emitTriangle( vTCNormal2[ 0 ], vFaceNormal[ 0 ] );
	emitTriangle( vTCNormal3[ 0 ], vFaceNormal[ 0 ] );
	
	// emit 3 points covering the 3 corner tangents to update
	emitTriangle( vTCTangent1[ 0 ], vFaceTangent[ 0 ] );
	emitTriangle( vTCTangent2[ 0 ], vFaceTangent[ 0 ] );
	emitTriangle( vTCTangent3[ 0 ], vFaceTangent[ 0 ] );
}
