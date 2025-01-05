uniform highp mat4 pColorTransform;
uniform highp vec4 pColorTransform2;

uniform mediump sampler2D texTexture;

in mediump vec2 vTexCoord;

out lowp vec4 outColor;

void main( void ){
	outColor = pColorTransform * textureLod( texTexture, vTexCoord, 0.0 ) + pColorTransform2;
}
