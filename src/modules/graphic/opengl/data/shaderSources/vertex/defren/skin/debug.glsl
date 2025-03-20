precision HIGHP float;
precision HIGHP int;

layout (std140, row_major) uniform InstanceParameters{
	mat4 pMatrixMVP;
	mat4x3 pMatrixMV;
	mat3 pMatrixNor;
	mat3x2 pMatrixTexCoord;
	bool pDoubleSided;
	vec3 pColorTint;			// color.tint
	float pNormalStrength;		// normal.strength
	vec3 pEmissivity;			// emissivity
	float pEnvMapFade;
};

in vec3 inPosition;

void main( void ){
	gl_Position = pMatrixMVP * vec4( inPosition, 1.0 );
}
