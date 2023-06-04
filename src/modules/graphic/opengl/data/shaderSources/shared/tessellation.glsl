// contains tessellation routines used for tessellation evaluation shaders

// tessellate using linear interpolation
void tessLinearTri( out vec4 result, in vec4 p1, in vec4 p2, in vec4 p3 ){
	result = p1 * vec4( gl_TessCoord.x )
		+ p2 * vec4( gl_TessCoord.y )
		+ p3 * vec4( gl_TessCoord.z );
}

void tessLinearTri( out vec3 result, in vec3 p1, in vec3 p2, in vec3 p3 ){
	result = p1 * vec3( gl_TessCoord.x )
		+ p2 * vec3( gl_TessCoord.y )
		+ p3 * vec3( gl_TessCoord.z );
}

void tessLinearTri( out vec2 result, in vec2 p1, in vec2 p2, in vec2 p3 ){
	result = p1 * vec2( gl_TessCoord.x )
		+ p2 * vec2( gl_TessCoord.y )
		+ p3 * vec2( gl_TessCoord.z );
}

void tessLinearTri( out float result, in float p1, in float p2, in float p3 ){
	result = p1 * gl_TessCoord.x
		+ p2 * gl_TessCoord.y
		+ p3 * gl_TessCoord.z;
}

#define TESS_VAR_LINEAR_TRI(o,i) tessLinearTri( o, i[0], i[1], i[2] )



void tessLinearQuad( out vec4 result, in vec4 p1, in vec4 p2, in vec4 p3, in vec4 p4 ){
	result = mix(
		mix( p2, p1, gl_TessCoord.x ),
		mix( p3, p4, gl_TessCoord.x ),
			gl_TessCoord.y );
}

void tessLinearQuad( out vec3 result, in vec3 p1, in vec3 p2, in vec3 p3, in vec3 p4 ){
	result = mix(
		mix( p2, p1, gl_TessCoord.x ),
		mix( p3, p4, gl_TessCoord.x ),
			gl_TessCoord.y );
}

void tessLinearQuad( out vec2 result, in vec2 p1, in vec2 p2, in vec2 p3, in vec2 p4 ){
	result = mix(
		mix( p2, p1, gl_TessCoord.x ),
		mix( p3, p4, gl_TessCoord.x ),
			gl_TessCoord.y );
}

void tessLinearQuad( out float result, in float p1, in float p2, in float p3, in float p4 ){
	result = mix(
		mix( p2, p1, gl_TessCoord.x ),
		mix( p3, p4, gl_TessCoord.x ),
			gl_TessCoord.y );
}

#define TESS_VAR_LINEAR_QUAD(o,i) tessLinearTri( o, i[0], i[1], i[2], i[3] )
