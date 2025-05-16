uniform HIGHP mat4 pColorTransform;
uniform HIGHP vec4 pColorTransform2;

uniform mediump sampler2D texTexture;

in mediump vec2 vTexCoord;

layout(location=0) out lowp vec4 outColor;

void main( void ){
	outColor = pColorTransform * textureLod( texTexture, vTexCoord, 0.0 ) + pColorTransform2;
}
