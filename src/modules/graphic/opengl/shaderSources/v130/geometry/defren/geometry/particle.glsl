precision highp float;
precision highp int;

layout( points ) in;
layout( triangle_strip, max_vertices=4 ) out;

uniform mat4 pMatrixProj;

in vec3 vParticle0[ 1 ]; // size, emissivity, rotation
in vec4 vParticle1[ 1 ]; // red, green, blue, transparency

out vec2 vTCDiffuse;
out vec2 vTCNormal;
#ifdef HAS_MAP_EMISSIVE
out vec2 vTCEmissive;
#endif
out vec2 vTCAO;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBitangent;
#ifdef WITH_REFLECTION
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

void main( void ){
	// this is the same for all points
	vNormal = particleNormal;
	vTangent = particleTangent;
	vBitangent = particleBitangent;
	
	vColor = vParticle1[ 0 ];
	vEmissivity = vParticle0[ 0 ].y;
	
	gl_PrimitiveID = gl_PrimitiveIDIn;
	gl_Layer = 0;
	
	// generate billboard
	mat2 rotmat = mat2( cos( vParticle0[ 0 ].z ), -sin( vParticle0[ 0 ].z ), sin( vParticle0[ 0 ].z ), cos( vParticle0[ 0 ].z ) );
	vec4 position = gl_in[ 0 ].gl_Position; // z and w stays the same for all vertices
	vec2 size = vec2( vParticle0[ 0 ].x );
	
	position.xy += rotmat * ( bc1 * size );
	gl_Position = pMatrixProj * position;
	vTCDiffuse = tc1;
	vTCNormal = tc1;
#ifdef HAS_MAP_EMISSIVE
	vTCEmissive = tc1;
#endif
	vTCAO = tc1;
#ifdef WITH_REFLECTION
	vReflectDir = position;
#endif
	EmitVertex();
	
	position.xy = gl_in[ 0 ].gl_Position.xy + rotmat * ( bc2 * size );
	gl_Position = pMatrixProj * position;
	vTCDiffuse = tc2;
	vTCNormal = tc2;
#ifdef HAS_MAP_EMISSIVE
	vTCEmissive = tc2;
#endif
	vTCAO = tc2;
#ifdef WITH_REFLECTION
	vReflectDir = position;
#endif
	EmitVertex();
	
	position.xy = gl_in[ 0 ].gl_Position.xy + rotmat * ( bc3 * size );
	gl_Position = pMatrixProj * position;
	vTCDiffuse = tc3;
	vTCNormal = tc3;
#ifdef HAS_MAP_EMISSIVE
	vTCEmissive = tc3;
#endif
	vTCAO = tc3;
#ifdef WITH_REFLECTION
	vReflectDir = position;
#endif
	EmitVertex();
	
	position.xy = gl_in[ 0 ].gl_Position.xy + rotmat * ( bc4 * size );
	gl_Position = pMatrixProj * position;
	vTCDiffuse = tc4;
	vTCNormal = tc4;
#ifdef HAS_MAP_EMISSIVE
	vTCEmissive = tc4;
#endif
	vTCAO = tc4;
#ifdef WITH_REFLECTION
	vReflectDir = position;
#endif
	EmitVertex();
	
	EndPrimitive();
}
