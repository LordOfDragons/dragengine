#include "shared/preamble.glsl"

precision HIGHP float;
precision HIGHP int;

UNIFORM_BIND(0) uniform ivec4 pParameters; // to.mapsPerRow, to.rowsPerImage, from.mapsPerRow, from.rowsPerImage

layout(location=0) in vec2 inPosition;

#include "shared/interface/2d/vertex.glsl"

void main( void ){
	vertexShaderDefaultOutputs();
	
	// calculate target map to write to and source map to sample from.
	// both the target and source calculations are done at the same time.
	// the basic calculation is like this:
	// 
	// vec2 scale = vec2( 1 ) / vec2( mapsPerRow, rowsPerImage );
	// vec2 offsetScale = vec2( 2 ) / vec2( mapsPerRow, rowsPerImage );
	// vec2 offsetBase = offsetScale * vec2( 0.5 ) - vec2( 1 );
	// ivec2 matMapCoord = ivec2( gl_InstanceID % mapsPerRow, gl_InstanceID / mapsPerRow );
	// vec2 offset = offsetScale * vec2( matMapCoord ) + offsetBase;
	
	vec4 scale = vec4( 1 ) / vec4( pParameters );
	vec4 offsetScale = vec4( 2 ) / vec4( pParameters );
	vec4 offsetBase = offsetScale * vec4( 0.5 ) - vec4( 1 );
	
	ivec4 matMapCoord = ivec4( ivec2( gl_InstanceID ) % pParameters.xz, ivec2( gl_InstanceID ) / pParameters.xz );
	
	vec4 offset = offsetScale * vec4( matMapCoord.xzyw ) + offsetBase;
	
	vec4 result = inPosition.xyxy * scale + offset;
	
	gl_Position = vec4( result.xy, 0, 1 );
	
	vTexCoord = result.zw * vec2( 0.5 ) + vec2( 0.5 );
}
