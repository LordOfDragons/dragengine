precision HIGHP float;
precision HIGHP int;

layout (binding=0, std140, row_major) uniform TestParameters{
	mat4x3 pMatrix1;
	mat3 pMatrix2;
};

layout(location=0) out vec3 outColor;

void main(){
	mat4x3 indirectAccess = pMatrix1;
	vec3 normal = indirectAccess * vec4( vec3( 0.0, 0.0, 1.0 ), 1.0 );
	
	outColor = normal * vec3( 0.5 ) + vec3( 0.5 );
}
