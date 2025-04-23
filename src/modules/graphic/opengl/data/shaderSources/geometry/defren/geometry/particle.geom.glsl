#include "shared/defren/skin/macros_geometry.glsl"

layout( points ) in;
layout( triangle_strip, max_vertices=4 ) out;

#if defined PARTICLE_RIBBON || defined PARTICLE_BEAM
	#defined HAS_PARTICLE_SHEET_COUNT
#endif

uniform mat4 pMatrixProj;

in vec3 vParticle0[ 1 ]; // size, emissivity, rotation
in vec4 vParticle1[ 1 ]; // red, green, blue, transparency

#ifdef HAS_PARTICLE_SHEET_COUNT
	flat in int vParticleSheetCount[1];
#endif

out vec2 vTCDiffuse;
out vec2 vTCNormal;
#ifdef HAS_MAP_EMISSIVE
out vec2 vTCEmissive;
#endif
out vec2 vTCAO;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
#ifdef WITH_REFLECT_DIR
out vec3 vReflectDir;
#endif
out vec4 vColor;
out float vEmissivity;

const vec3 particleNormal = vec3( 0.0, 0.0, 1.0 );
const vec3 particleTangent = vec3( 1.0, 0.0, 0.0 );
const vec3 particleBitangent = vec3( 0.0, 1.0, 0.0 );

const vec2 bc1 = vec2( -0.5, -0.5 );
const vec2 bc2 = vec2( -0.5,  0.5 );
const vec2 bc3 = vec2(  0.5, -0.5 );
const vec2 bc4 = vec2(  0.5,  0.5 );

const vec2 tc1 = vec2( 0.0, 0.0 );
const vec2 tc2 = vec2( 0.0, 1.0 );
const vec2 tc3 = vec2( 1.0, 0.0 );
const vec2 tc4 = vec2( 1.0, 1.0 );


void emitCorner( in vec4 position, in vec2 tc ){
	vNormal = particleNormal;
	vTangent = particleTangent;
	vBitangent = particleBitangent;
	
	vColor = vParticle1[ 0 ];
	vEmissivity = vParticle0[ 0 ].y;
	
	vTCDiffuse = tc;
	vTCNormal = tc;
	#ifdef HAS_MAP_EMISSIVE
		vTCEmissive = tc;
	#endif
	vTCAO = tc;
	#ifdef WITH_REFLECT_DIR
		vReflectDir = position;
	#endif
	
	gl_Position = pMatrixProj * position;
	
	gl_Layer = 0;
	gl_PrimitiveID = gl_PrimitiveIDIn;
	
	//EmitVertex();
}

void main( void ){
	// NOTE: quest requires EmitVertex to be called in main()
	
	// generate billboard
	mat2 rotmat = mat2(
		cos( vParticle0[ 0 ].z ), -sin( vParticle0[ 0 ].z ),
		sin( vParticle0[ 0 ].z ),  cos( vParticle0[ 0 ].z ) );
	vec4 position = gl_in[ 0 ].gl_Position; // z and w stays the same for all vertices
	vec2 size = vec2( vParticle0[ 0 ].x );
	
	position.xy += rotmat * ( bc1 * size );
	emitCorner(position, tc1);
	EmitVertex();
	
	position.xy = gl_in[ 0 ].gl_Position.xy + rotmat * ( bc2 * size );
	emitCorner(position, tc2);
	EmitVertex();
	
	position.xy = gl_in[ 0 ].gl_Position.xy + rotmat * ( bc3 * size );
	emitCorner(position, tc3);
	EmitVertex();
	
	position.xy = gl_in[ 0 ].gl_Position.xy + rotmat * ( bc4 * size );
	emitCorner(position, tc4);
	EmitVertex();
	
	EndPrimitive();
}
