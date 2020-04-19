precision lowp float;
precision lowp int;

uniform vec4 pGamma; // red, green, blue
uniform vec4 pBrightness; // red, green, blue
uniform vec4 pContrast; // red, green, blue

uniform lowp sampler2D texColor;

in vec2 vTexCoord;

out vec4 outColor;

void main( void ){
	// gamma correction only
	//outColor = pow( texture( texColor, vTexCoord ), pGamma );
	
	// gamma, contrast and brightness correction
	outColor = pow( texture( texColor, vTexCoord ), pGamma ) * pContrast + pBrightness;
}
