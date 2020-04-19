precision mediump float;
precision mediump int;

uniform vec4 pDistortTransform;

uniform mediump sampler2D texColor;
uniform lowp sampler2D texDistort;

in vec2 vTexCoord;

out mediump vec4 outColor;

void main( void ){
	outColor = texture( texColor, vTexCoord + ( texture( texDistort, vTexCoord ).rg * pDistortTransform.xy + pDistortTransform.zw ) );
}
