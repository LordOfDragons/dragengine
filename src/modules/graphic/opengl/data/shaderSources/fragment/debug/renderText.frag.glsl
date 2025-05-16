precision HIGHP float;
precision HIGHP int;

uniform mediump sampler2D texTexture;

flat in vec4 vColor;
in vec2 vTexCoord;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = textureLod( texTexture, vTexCoord, 0.0 ) * vec4( vColor );
}
