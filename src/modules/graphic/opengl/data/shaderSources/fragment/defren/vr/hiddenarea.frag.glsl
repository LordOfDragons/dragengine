#include "shared/preamble.glsl"

precision lowp float;
precision lowp int;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = vec4(0, 0, 0, 1);
}
