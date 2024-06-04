uniform lowp sampler2D texDiffuse;
uniform lowp sampler2D texReflectivity;
uniform mediump sampler2D texEmissivity;

in vec2 vTexCoord;

out vec4 outDiffuse;
out vec4 outReflectivity;
out vec4 outEmissivity;

void main( void ){
	outDiffuse = textureLod( texDiffuse, vTexCoord, 0 );
	outReflectivity = textureLod( texReflectivity, vTexCoord, 0 );
	outEmissivity = textureLod( texEmissivity, vTexCoord, 0 );
}
