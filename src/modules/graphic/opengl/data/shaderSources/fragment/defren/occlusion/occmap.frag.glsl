#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

#include "shared/ubo_defines.glsl"
#include "shared/defren/occmap.glsl"

VARYING_BIND(0) in float vDepth;
VARYING_BIND(1) in vec3 vPosition;
VARYING_BIND(2) in vec3 vClipCoord;
VARYING_BIND(3) flat in int vLayer;
VARYING_BIND(4) flat in int vSPBIndex;

void main(void){
	if(UseClipPlane){
		if(dot(vClipCoord, getClipPlane(vLayer).xyz) <= getClipPlane(vLayer).w){
			discard;
		}
	}
	if(PerspectiveToLinear){
		gl_FragDepth = vDepth;
	}
	if(DepthDistance){
		gl_FragDepth = length(vPosition) * getZToDepth().x + getZToDepth().y;
	}
}
