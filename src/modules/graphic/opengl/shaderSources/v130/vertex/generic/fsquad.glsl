precision highp float;
precision highp int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

in vec2 inPosition;

void main( void ){
	gl_Position = vec4( vec3( inPosition, 0.0 ), 1.0 );
}
