precision highp float;
precision highp int;

uniform highp sampler2D texColor;

in vec2 vTexCoord;

out float outLuminance;

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	outLuminance = dot( texture( texColor, vTexCoord ).rgb, lumiFactors );
}
