#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d_vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	gl_Position = vec4( vec3( inPosition, 0.0 ), 1.0 );
}
