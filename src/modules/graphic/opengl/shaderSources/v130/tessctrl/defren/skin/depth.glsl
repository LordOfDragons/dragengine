//#extension GL_ARB_tessellation_shader : enable

// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

// some helper definitions to make the code easier to read
#if defined TEXTURE_SOLIDITY || defined OUTPUT_COLOR || defined TEXTURE_HEIGHT
	#define REQUIRES_TEX_COLOR 1
#endif

#ifdef TEXTURE_HEIGHT
	#define REQUIRES_NORMAL 1
#endif



// definitions
////////////////

layout( vertices=3 ) out;



// Inputs
///////////

#ifdef REQUIRES_TEX_COLOR
	in vec2 vTCSTCColor[];
#endif
#ifdef CLIP_PLANE
	in vec3 vTCSClipCoord[];
#endif
#ifdef DEPTH_DISTANCE
	in vec3 vTCSPosition[];
#endif
#ifdef HEIGHT_MAP
	in float vTCSHTMask[];
#endif
#ifdef PROP_FIELD
	in float vTCSRenderCondition[];
#endif
#ifdef REQUIRES_NORMAL
	in vec3 vTCSNormal[];
	#ifdef TEXTURE_NORMAL
		in vec3 vTCSTangent[];
		in vec3 vTCSBitangent[];
	#endif
#endif



// Outputs
////////////

#ifdef REQUIRES_TEX_COLOR
	out vec2 vTESTCColor[];
#endif
#ifdef CLIP_PLANE
	out vec3 vTESClipCoord[];
#endif
#ifdef DEPTH_DISTANCE
	out vec3 vTESPosition[];
#endif
#ifdef HEIGHT_MAP
	out float vTESHTMask[];
#endif
#ifdef PROP_FIELD
	out float vTESRenderCondition[];
#endif
#ifdef REQUIRES_NORMAL
	out vec3 vTESNormal[];
	#ifdef TEXTURE_NORMAL
		out vec3 vTESTangent[];
		out vec3 vTESBitangent[];
	#endif
#endif




// Main Function
//////////////////

void main(void){
	if( gl_InvocationID == 0 ){
		vec3 edgeLen;
		edgeLen.x = length( gl_in[ 0 ].gl_Position.xyz - gl_in[ 1 ].gl_Position.xyz );
		edgeLen.y = length( gl_in[ 1 ].gl_Position.xyz - gl_in[ 2 ].gl_Position.xyz );
		edgeLen.z = length( gl_in[ 2 ].gl_Position.xyz - gl_in[ 0 ].gl_Position.xyz );
		
		vec3 splits = clamp( floor( edgeLen / vec3( 0.01 ) ), vec3( 1.0 ), vec3( 32.0 ) );
		
		gl_TessLevelOuter[ 0 ] = splits.x;
		gl_TessLevelOuter[ 1 ] = splits.y;
		gl_TessLevelOuter[ 2 ] = splits.z;
		
		gl_TessLevelInner[ 0 ] = floor( dot( splits, vec3( 1.0 / 3.0 ) ) );
	}
	
	gl_out[ gl_InvocationID ].gl_Position = gl_in[ gl_InvocationID ].gl_Position;
	
	#ifdef REQUIRES_TEX_COLOR
		vTESTCColor[ gl_InvocationID ] = vTCSTCColor[ gl_InvocationID ];
	#endif
	#ifdef CLIP_PLANE
		vTESClipCoord[ gl_InvocationID ] = vTCSClipCoord[ gl_InvocationID ];
	#endif
	#ifdef DEPTH_DISTANCE
		vTESPosition[ gl_InvocationID ] = vTCSPosition[ gl_InvocationID ];
	#endif
	#ifdef HEIGHT_MAP
		vTESHTMask[ gl_InvocationID ] = vTCCSHTMask[ gl_InvocationID ];
	#endif
	#ifdef PROP_FIELD
		vTESRenderCondition[ gl_InvocationID ] = vTCSRenderCondition[ gl_InvocationID ];
	#endif
	#ifdef REQUIRES_NORMAL
		vTESNormal[ gl_InvocationID ] = vTCSNormal[ gl_InvocationID ];
		#ifdef TEXTURE_NORMAL
			vTESTangent[ gl_InvocationID ] = vTCSTangent[ gl_InvocationID ];
			vTESBitangent[ gl_InvocationID ] = vTCSBitangent[ gl_InvocationID ];
		#endif
	#endif
}
