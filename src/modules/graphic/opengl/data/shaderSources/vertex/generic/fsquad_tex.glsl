precision highp float;
precision highp int;

// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //
//                                                        //
//             use screenspace.glsl instead               //
//                                                        //
// DEPRECATED DEPRECATED DEPRECATED DEPRECATED DEPRECATED //

uniform vec4 pPosToTC; // scaleX, scaleY, offsetX, offsetY

in vec2 inPosition;

out vec2 vTexCoord;

void main( void ){
	gl_Position = vec4( vec3( inPosition, 0.0 ), 1.0 );
	vTexCoord = inPosition.xy * pPosToTC.xy + pPosToTC.zw;
}
