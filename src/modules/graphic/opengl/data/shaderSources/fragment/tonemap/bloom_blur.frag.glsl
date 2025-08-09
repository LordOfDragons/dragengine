#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(3) uniform vec4 pOffsets1; // offset1.s, offset1.t, offset2.s, offset2.t
UNIFORM_BIND(4) uniform vec4 pOffsets2; // offset3.s, offset3.t, offset4.s, offset4.t
UNIFORM_BIND(5) uniform vec4 pOffsets3; // offset5.s, offset5.t, offset6.s, offset6.t
UNIFORM_BIND(6) uniform vec4 pOffsets4; // offset7.s, offset7.t, offset8.s, offset8.t
UNIFORM_BIND(7) uniform vec4 pOffsets5; // offset9.s, offset9.t, offset10.s, offset10.t
UNIFORM_BIND(8) uniform vec4 pWeights1; // weightCenter, weight1, weight2, weight3
UNIFORM_BIND(9) uniform vec4 pWeights2; // weight4, weight5, n/a, n/a
UNIFORM_BIND(10) uniform vec2 pClamp; // clamp.s, clamp.t
UNIFORM_BIND(11) uniform float pLevel;
UNIFORM_BIND(12) uniform float pScaleColor;

layout(binding=0) uniform mediump sampler2DArray texColor;

#include "shared/interface/2d/fragment.glsl"

layout(location=0) out vec3 outColor;

void main( void ){
	outColor = textureLod( texColor, vec3( vTexCoord, vLayer ), pLevel ).rgb * vec3( pWeights1.x );
	
	vec4 tc = min( vTexCoord.stst + pOffsets1, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), pLevel ).rgb * vec3( pWeights1.y );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), pLevel ).rgb * vec3( pWeights1.y );
	
	tc = min( vTexCoord.stst + pOffsets2, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), pLevel ).rgb * vec3( pWeights1.z );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), pLevel ).rgb * vec3( pWeights1.z );
	
	tc = min( vTexCoord.stst + pOffsets3, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), pLevel ).rgb * vec3( pWeights1.w );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), pLevel ).rgb * vec3( pWeights1.w );
	
	tc = min( vTexCoord.stst + pOffsets4, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), pLevel ).rgb * vec3( pWeights2.x );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), pLevel ).rgb * vec3( pWeights2.x );
	
	tc = min( vTexCoord.stst + pOffsets5, pClamp.stst );
	outColor += textureLod( texColor, vec3( tc.xy, vLayer ), pLevel ).rgb * vec3( pWeights2.y );
	outColor += textureLod( texColor, vec3( tc.zw, vLayer ), pLevel ).rgb * vec3( pWeights2.y );
	
	outColor *= vec3( pScaleColor );
}
