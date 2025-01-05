uniform mat4 pColorTransform;
uniform vec4 pColorTransform2;

uniform mediump sampler2D texTexture;

in vec2 vTexCoord;

out vec4 outColor;


void main( void ){
	outColor = pColorTransform * textureLod( texTexture, vTexCoord, 0.0 ) + pColorTransform2;
}
