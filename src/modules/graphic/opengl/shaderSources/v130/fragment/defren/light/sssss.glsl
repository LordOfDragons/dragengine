precision highp float;
precision highp int;

uniform vec4 pPosTransform;
uniform vec4 pTCTransform;
uniform vec4 pTCClamp;

uniform float pDropSubSurfaceThreshold;

uniform int pTapCount;
uniform float pAngleConstant;
uniform float pTapRadiusFactor;
uniform float pTapRadiusLimit;
uniform float pTapDropRadiusThreshold;

uniform HIGHP sampler2D texDepth;
uniform lowp sampler2D texDiffuse;
uniform mediump sampler2D texSubSurface;
uniform mediump sampler2D texLight;

in mediump vec2 vTexCoord;

out mediump vec3 outColor;



// Constants
//////////////

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif



// Sub-surface scattering calculation
///////////////////////////////////////

void scatter( in vec2 tc, in vec3 position, in vec3 scatterScale, inout vec3 sumLight, inout vec3 sumWeight ){
	tc = clamp( tc, pTCClamp.xy, pTCClamp.zw );
	
	// calculate distance to the center point
	#ifdef DECODE_IN_DEPTH
		vec3 spos = vec3( dot( textureLod( texDepth, tc, 0.0 ).rgb, unpackDepth ) );
	#else
		vec3 spos = vec3( textureLod( texDepth, tc, 0.0 ).r );
	#endif
	spos.z = pPosTransform.x / ( pPosTransform.y - spos.z );
	spos.xy = tc * pTCTransform.xy + pTCTransform.zw; // convert to -1..1 range
	spos.xy *= pPosTransform.zw * spos.zz;
	
	spos -= position;
	
	vec3 scatDist = vec3( length( spos ) ) * scatterScale;
	
	// calculate the attenuation due to the distance from the center point
	vec3 attenuation = exp( scatDist * vec3( -5.0 ) ); // expFactor = -5.0 on 0..1m
	
	// get the light contribution at the scatter position
	sumLight += textureLod( texLight, tc, 0.0 ).rgb * attenuation;
	sumWeight += attenuation;
}

vec3 subSurfaceScattering( in vec3 position, in float tapRadius, in vec3 scatterScale, in int tapCount ){
	ivec2 tcint = ivec2( gl_FragCoord.xy );
	
	vec3 sumLight = textureLod( texLight, vTexCoord, 0.0 ).rgb;
	vec3 sumWeight = vec3( 1.0 );
	
	vec2 factor1 = vec2( 1.0, 0.5 ) / vec2( tapCount );
	float c1 = 30.0 * float( tcint.x ^ tcint.y ) + 10.0 * float( tcint.x ) * float( tcint.y );
	int i;
	
	for( i=0; i<tapCount; i++ ){
		float v1 = factor1.x * float( i ) + factor1.y;
		float v2 = pAngleConstant * v1 + c1;
		
		vec2 tcoff = vec2( tapRadius * v1 ) * vec2( cos( v2 ), sin( v2 ) );
		
		scatter( vTexCoord + tcoff, position, scatterScale, sumLight, sumWeight );
	}
	
	return sumLight / sumWeight;
}



// Main Function
//////////////////

void main( void ){
	//discard; // for testing translucency in light shader
	ivec2 tc = ivec2( gl_FragCoord.xy );
	
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
		outColor = textureLod( texLight, vTexCoord, 0.0 ).rgb;
		
	}else{
		// determine position of fragment to light
		#ifdef DECODE_IN_DEPTH
			vec3 position = vec3( dot( texelFetch( texDepth, tc, 0 ).rgb, unpackDepth ) );
		#else
			vec3 position = vec3( texelFetch( texDepth, tc, 0 ).r );
		#endif
		position.z = pPosTransform.x / ( pPosTransform.y - position.z );
		position.xy = vTexCoord * pTCTransform.xy + pTCTransform.zw; // convert to -1..1 range
		position.xy *= pPosTransform.zw * position.zz;
		
		// calculate tap radius
		float pTapRadiusLimit = 0.5; // 50% screen size
		float tapRadius = min( pTapRadiusFactor * largestAbsorptionRadius / position.z, pTapRadiusLimit );
		
		// if the tap radius is too small drop the sss calculation altogether to increase performance
		if( tapRadius < pTapDropRadiusThreshold ){
			outColor = textureLod( texLight, vTexCoord, 0.0 ).rgb;
			
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
