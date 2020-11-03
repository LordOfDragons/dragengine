precision highp float;
precision highp int;

uniform highp sampler2D texDepth;
uniform highp sampler2D texColor;

in vec2 vTexCoord;

out float outLum;

const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia

void main( void ){
	gl_FragDepth = texture( texDepth, vTexCoord ).r;
	outLum = dot( texture( texColor, vTexCoord ).rgb, lumiFactors );
}
