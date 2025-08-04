#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

in float fbResult;

out float outResult;

void main( void ){
	outResult = fbResult;
}
