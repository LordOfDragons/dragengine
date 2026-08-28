#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform vec3 pGamma;

#include "shared/ubo_defines.glsl"
#include "shared/defren/ubo_render_parameters.glsl"

layout(binding=0) uniform lowp sampler2D texColor;

VARYING_BIND(0) in mediump vec2 vTexCoord;

layout(location=0) out vec4 outColor;

void main( void ){
	outColor = pow(texture(texColor, vTexCoord), vec4(pGamma, 1.0));
}
