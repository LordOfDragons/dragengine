precision highp float;
precision highp int;

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"

uniform HIGHP sampler2DArray texDepth;
uniform lowp sampler2DArray texDiffuse;
uniform mediump sampler2DArray texSubSurface;
uniform mediump sampler2DArray texLight;

in mediump vec2 vTexCoord;
in mediump vec2 vScreenCoord;

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	flat in int vLayer;
#else
	const int vLayer = 0;
#endif

out mediump vec3 outColor;

#include "v130/shared/defren/depth_to_position.glsl"

#define pDropSubSurfaceThreshold pSSSSSParams1.x
#define pTapRadiusFactor pSSSSSParams1.y
#define pTapRadiusLimit pSSSSSParams1.z
#define pTapDropRadiusThreshold pSSSSSParams1.w

#define pTapCount pSSSSSParams2.x
#define pTurnCount pSSSSSParams2.y


// Sub-surface scattering calculation
///////////////////////////////////////

void scatter( in vec3 tc, in vec3 position, in vec3 scatterScale, inout vec3 sumLight, inout vec3 sumWeight ){
	tc.xy = clamp( tc.xy, pViewportMin, pViewportMax );
	
	vec3 spos = depthToPosition( texDepth, tc, fsquadTexCoordToScreenCoord( tc.xy ), vLayer ) - position;
	
	vec3 scatDist = vec3( length( spos ) ) * scatterScale;
	
	// calculate the attenuation due to the distance from the center point
	vec3 attenuation = exp( scatDist * vec3( -5.0 ) ); // expFactor = -5.0 on 0..1m
	
	// get the light contribution at the scatter position
	sumLight += textureLod( texLight, tc, 0.0 ).rgb * attenuation;
	sumWeight += attenuation;
}

vec3 subSurfaceScattering( in vec3 position, in float tapRadius, in vec3 scatterScale, in int tapCount ){
	const float angleConstant = 6.2831853 * float( pTurnCount ); // pi * 2.0
	ivec2 tcint = ivec2( gl_FragCoord.xy );
	
	vec3 sumLight = textureLod( texLight, vec3( vTexCoord, vLayer ), 0 ).rgb;
	vec3 sumWeight = vec3( 1 );
	
	vec2 factor1 = vec2( 1, 0.5 ) / vec2( tapCount );
	float c1 = float( tcint.x ^ tcint.y ) * 30 + float( tcint.x ) * float( tcint.y ) * 10;
	int i;
	
	for( i=0; i<tapCount; i++ ){
		float v1 = factor1.x * float( i ) + factor1.y;
		float v2 = angleConstant * v1 + c1;
		
		vec2 tcoff = vec2( tapRadius * v1 ) * vec2( cos( v2 ), sin( v2 ) );
		
		scatter( vec3( vTexCoord + tcoff, vLayer ), position, scatterScale, sumLight, sumWeight );
	}
	
	return sumLight / sumWeight;
}



// Main Function
//////////////////

void main( void ){
	//discard; // for testing translucency in light shader
	ivec3 tc = ivec3( gl_FragCoord.xy, vLayer );
	
	// discard not inizalized fragments or fragements that are not supposed to be lit
	vec4 diffuse = texelFetch( texDiffuse, tc, 0 );
	if( diffuse.a == 0.0 ){
		discard;
	}
	
	// get the absorption.radius
	vec3 absorptionRadius = texelFetch( texSubSurface, tc, 0 ).rgb;
	float largestAbsorptionRadius = max( max( absorptionRadius.x, absorptionRadius.y ), absorptionRadius.z );
	
	// if the absorption radius is near zero drop the sss calculation altogether to increase performance
	if( largestAbsorptionRadius < pDropSubSurfaceThreshold ){
		outColor = textureLod( texLight, vec3( vTexCoord, vLayer ), 0 ).rgb;
		
	}else{
		// determine position of fragment to light
		vec3 position = depthToPosition( texDepth, vScreenCoord, vLayer );
		
		// calculate tap radius
		float tapRadius = min( pTapRadiusFactor * largestAbsorptionRadius / position.z, pTapRadiusLimit );
		
		// if the tap radius is too small drop the sss calculation altogether to increase performance
		if( tapRadius < pTapDropRadiusThreshold ){
			outColor = textureLod( texLight, vec3( vTexCoord, vLayer ), 0 ).rgb;
			
		}else{
			// calculate the scatter scaling. this is required to scale the sss calculation differently for each component.
			// it's not a fully correct version but works well enough inside a shader
			vec3 scatterScale = vec3( 1.0 ) / max( absorptionRadius, vec3( 0.0001 ) );
			
			// calculate sss using tap spiral
			int tapCount = pTapCount; // so we can adjust it with smaller radius
			
			outColor = subSurfaceScattering( position, tapRadius, scatterScale, tapCount );
		}
	}
	//outColor = min( outColor, vec3( 0.5 ) ); // HACK
	// apply diffuse color
	outColor *= diffuse.rgb;
}
