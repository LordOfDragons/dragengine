#include "v130/shared/defren/skin/macros_geometry.glsl"

// Uniform Parameters
///////////////////////

#include "v130/shared/ubo_defines.glsl"
#include "v130/shared/defren/ubo_render_parameters.glsl"
#include "v130/shared/defren/skin/ubo_texture_parameters.glsl"
#include "v130/shared/defren/skin/ubo_instance_parameters.glsl"
#include "v130/shared/defren/skin/ubo_dynamic_parameters.glsl"



// Samplers
/////////////

#ifdef WITH_VARIATIONS
	// functions are defined right before main due to Shared-SPB support
	#define SAMPLER_2D sampler2DArray
	#define TEXTURE(s,tc) texture(s, tcTexVar(tc, textureSize(s, 0).z))
#else
	#define SAMPLER_2D sampler2D
	#define TEXTURE(s,tc) texture(s, tc)
#endif

#ifdef TEXTURE_COLOR
	uniform lowp SAMPLER_2D texColor;
#endif
#ifdef TEXTURE_TRANSPARENCY
	uniform lowp SAMPLER_2D texTransparency;
#endif
#ifdef TEXTURE_COLOR_TINT_MASK
	uniform lowp SAMPLER_2D texColorTintMask;
#endif
#ifdef TEXTURE_SOLIDITY
	uniform lowp SAMPLER_2D texSolidity;
#endif
#ifdef TEXTURE_NORMAL
	uniform lowp SAMPLER_2D texNormal;
#endif
#ifdef TEXTURE_HEIGHT
	uniform lowp SAMPLER_2D texHeight;
#endif
#ifdef TEXTURE_REFLECTIVITY
	uniform lowp SAMPLER_2D texReflectivity;
#endif
#ifdef TEXTURE_ROUGHNESS
	uniform lowp SAMPLER_2D texRoughness;
#endif
#ifdef TEXTURE_EMISSIVITY
	uniform mediump SAMPLER_2D texEmissivity;
#endif
#ifdef TEXTURE_RENDERCOLOR
	uniform mediump sampler2DArray texRenderColor;
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
	uniform lowp SAMPLER_2D texRefractionDistort;
#endif
#ifdef TEXTURE_AO
	uniform lowp SAMPLER_2D texAO;
#endif
#ifdef TEXTURE_ENVMAP
	#ifdef TEXTURE_ENVMAP_EQUI
		uniform mediump sampler2D texEnvMap;
		#ifdef TEXTURE_ENVMAP_FADE
			uniform mediump sampler2D texEnvMapFade;
		#endif
	#else
		uniform mediump samplerCube texEnvMap;
		#ifdef TEXTURE_ENVMAP_FADE
			uniform mediump samplerCube texEnvMapFade;
		#endif
	#endif
#endif
#ifdef TEXTURE_ENVROOM
	uniform lowp samplerCube texEnvRoom;
#endif
#ifdef TEXTURE_ENVROOM_MASK
	uniform lowp SAMPLER_2D texEnvRoomMask;
#endif
#ifdef TEXTURE_ENVROOM_EMISSIVITY
	uniform mediump samplerCube texEnvRoomEmissivity;
#endif
#ifdef TEXTURE_ABSORPTION
	uniform lowp SAMPLER_2D texAbsorption;
#endif
#ifdef TEXTURE_RIM_EMISSIVITY
	uniform mediump SAMPLER_2D texRimEmissivity;
#endif
#ifdef TEXTURE_NONPBR_ALBEDO
	uniform lowp SAMPLER_2D texNonPbrAlbedo;
#endif
#ifdef TEXTURE_NONPBR_METALNESS
	uniform lowp SAMPLER_2D texNonPbrMetalness;
#endif
#ifdef DEPTH_TEST
	uniform HIGHP sampler2DArray texDepthTest;
#endif



// Inputs
///////////

in vec2 vTCColor;
#ifdef TEXTURE_COLOR_TINT_MASK
in vec2 vTCColorTintMask;
#endif
#ifdef TEXTURE_NORMAL
in vec2 vTCNormal;
#endif
#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
in vec2 vTCReflectivity;
#endif
#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
in vec2 vTCEmissivity;
#endif
#ifdef TEXTURE_REFRACTION_DISTORT
in vec2 vTCRefractionDistort;
#endif
#ifdef TEXTURE_AO
in vec2 vTCAO;
#endif

in vec3 vNormal;
#ifdef WITH_TANGENT
	in vec3 vTangent;
#endif
#ifdef WITH_BITANGENT
	in vec3 vBitangent;
#endif
#ifdef WITH_REFLECT_DIR
	in vec3 vReflectDir;
#endif
#ifdef PARTICLE
	in vec4 vParticleColor; // from curve property
	#ifdef TEXTURE_EMISSIVITY
		in float vParticleEmissivity; // from curve property
	#endif
#endif
#ifdef HEIGHT_MAP
	in float vHTMask;
#endif
#ifdef FADEOUT_RANGE
	in float vFadeZ;
#endif

#ifdef SHARED_SPB
	flat in int vSPBIndex;
	#define spbIndex vSPBIndex
	#include "v130/shared/defren/skin/shared_spb_redirect.glsl"
#endif

#if defined GS_RENDER_STEREO || defined VS_RENDER_STEREO
	in flat int vLayer;
#else
	const int vLayer = 0;
#endif

#include "v130/shared/defren/skin/shared_spb_texture_redirect.glsl"



// Outputs
////////////

#ifdef OUTPUT_MATERIAL_PROPERTIES
	#ifdef OUTPUT_LIMITBUFFERS
		out vec4 outDiffuse; // diffuse.r, diffuse.g, diffuse.b, transparency
		out vec4 outNormal; // normal.x, normal.y, normal.z, blend
		out vec4 outReflectivity; // reflectivity.r, reflectivity.g, reflectivity.b, blend
	#else
		out vec4 outDiffuse; // diffuse.r, diffuse.g, diffuse.b, transparency
		out vec4 outNormal; // normal.x, normal.y, normal.z, blend
		out vec4 outReflectivity; // reflectivity.r, reflectivity.g, reflectivity.b, blend
		out vec4 outRoughness; // roughness, n/a, n/a, blend
		out vec4 outAOSolidity; // ao, ssao, solidity, blend
		out vec4 outSubSurface; // subsurface.rgb, blend
	#endif
#endif
#ifdef LUMINANCE_ONLY
	out vec3 outLuminance; // luminance, n/a, n/a
	out vec3 outNormal; // normal.x, normal.y, normal.z
#else
	out vec4 outColor; // color.r, color.g, color.b, n/a
#endif
#ifdef NODE_FRAGMENT_OUTPUTS
NODE_FRAGMENT_OUTPUTS
#endif



// Includes requiring inputs to be defined
////////////////////////////////////////////

#ifndef HAS_TESSELLATION_SHADER
	#include "v130/shared/defren/skin/relief_mapping.glsl"
#endif

#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
	#include "v130/shared/defren/skin/environment_room.glsl"
#endif

#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
	#include "v130/shared/defren/skin/nonpbr_metalness.glsl"
#endif



// Constants
//////////////

#ifdef DECODE_IN_DEPTH
	const vec3 unpackDepth = vec3( 1.0, 1.0 / 256.0, 1.0 / 65536.0 );
#endif

#ifdef TEXTURE_ENVMAP_EQUI
	const vec4 cemefac = vec4( 0.5, 1.0, -0.1591549, -0.3183099 ); // 0.5, 1.0, -1/2pi, -1/pi
#endif

const vec4 colorTransparent = vec4( 0.0, 0.0, 0.0, 1.0 );

#ifdef LUMINANCE_ONLY
	const vec3 lumiFactors = vec3( 0.2125, 0.7154, 0.0721 );
	//const vec3 lumiFactors = vec3( 0.3086, 0.6094, 0.0820 ); // nVidia
#endif

#include "v130/shared/normal.glsl"


// functions required to be define last because they are based on stuff defined above
#ifdef WITH_VARIATIONS
	// NOTE: according to opengl website the layer is clamped like this:
	//         layer = clamp( floor( texCoord.p​ + 0.5 ), 0, d​ - 1 ).
	//       thus the texCoord.p has to be reduced by 0.5 to map the entire range, example:
	//         layerCount = 3
	//         texCoord.p = 0.0 .. 3.0
	//         texCoord.p - 0.5 = -0.5 .. 2.5
	//         clamped => -0.5 .. 0.5 => 0; 0.5 .. 1.5 => 1; 1.5 .. 2.5 => 2
	//       this way all value have the same dominance
	
	vec3 tcTexVar( in vec2 tc, in int layerCount ){
		vec3 p = fract( floor( tc.sts * pVariationEnableScale.xyx + pVariationSeed.xyx ) * vec3( 0.1031 ) );
		p += dot( p, p.yzx + vec3( 19.19 ) );
		return vec3( tc, fract( ( p.x + p.y ) * p.z ) * float( layerCount ) - 0.5 );
	}
	
	#if 0
	vec3 tcTexVar( in vec2 tc, in int layerCount ){
		return vec3( tc, fract( sin( dot( floor( tc ) / vec2( 1024 ) * pVariationEnableScale + pVariationSeed, vec2( 12.9898, 78.233 ) ) )
			* 43758.5453123 ) * float( layerCount ) - 0.5 );
	}
	#endif
#endif



float finalEmissivityIntensity( in float intensity ){
	return mix( intensity, intensity * pCameraAdaptedIntensity, pEmissivityCameraAdapted );
}

vec3 finalEmissivityIntensity( in vec3 intensity ){
	return mix( intensity, intensity * vec3( pCameraAdaptedIntensity ), bvec3( pEmissivityCameraAdapted ) );
}

// Main Function
//////////////////

void main( void ){
	// for limited systems add dummy variables. the compiler is going to remove them. a better
	// solution has to be found for this later.
	#ifdef OUTPUT_LIMITBUFFERS
	vec4 outRoughness;
	vec4 outAOSolidity;
	vec4 outSubSurface;
	#endif
	
	// for height map quickly dicard fragments if the mask is 0
	#ifdef HEIGHT_MAP
		if( vHTMask < 0.001 ){
			discard;
		}
	#endif
	
	// discard fragments beyond render range
	/*
	#ifdef FADEOUT_RANGE
		if( vFadeZ > pFadeRange.y ){
			discard;
		}
	#endif
	*/
	
	// calculate the screen space texture coordinates
	#ifdef REQUIRES_TEX_COORD
		ivec2 tc = ivec2( gl_FragCoord.xy );
	#endif
	
	// test against depth texture
	#ifdef DEPTH_TEST
		#ifdef DECODE_IN_DEPTH
		float depthTestValue = dot( texelFetch( texDepthTest, ivec3( tc, vLayer ), 0 ).rgb, unpackDepth );
		#else
		float depthTestValue = texelFetch( texDepthTest, ivec3( tc, vLayer ), 0 ).r;
		#endif
		
		#ifdef INVERSE_DEPTH
			#ifdef DEPTH_TEST_LARGER
			if( gl_FragCoord.z < depthTestValue ) discard;
			#else
			if( gl_FragCoord.z > depthTestValue ) discard;
			#endif
		#else
			#ifdef DEPTH_TEST_LARGER
			if( gl_FragCoord.z > depthTestValue ) discard;
			#else
			if( gl_FragCoord.z < depthTestValue ) discard;
			#endif
		#endif
	#endif
	
	
	
	// determine the correct normal. for back facing fragments the normal has to be flipped. care
	// has to be taken if the rendering is mirrored. in this case the front facing is exactly the
	// opposite of what we are really looking for. The xor operator does exactly this
	vec3 realNormal = mix( -vNormal, vNormal, vec3( pFlipCulling ^^ gl_FrontFacing ) ); // mix( if-false, if-true, condition )
	/*vec3 realNormal = vNormal;
	
	if( ! gl_FrontFacing ){
		realNormal = -realNormal;
	}*/
	
	// relief mapping. definition of macros has to be delied until here since undefine
	// symbols can lead to tricky situations resulting in compilers failing
	#ifdef HAS_TESSELLATION_SHADER
		#define tcColor vTCColor
		#ifdef TEXTURE_COLOR_TINT_MASK
			#define tcColorTintMask vTCColorTintMask
		#endif
		#ifdef TEXTURE_NORMAL
			#define tcNormal vTCNormal
		#endif
		#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
			#define tcReflectivity vTCReflectivity
		#endif
		#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
			#define tcEmissivity vTCEmissivity
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			#define tcRefractionDistort vTCRefractionDistort
		#endif
		#ifdef TEXTURE_AO
			#define tcAO vTCAO
		#endif
		#ifdef TEXTURE_ABSORPTION
			#define tcAbsorption tcColor
		#endif
	
	#else
		vec2 tcReliefMapped = vTCColor;
		reliefMapping( tcReliefMapped, realNormal );
		
		#define tcColor tcReliefMapped
		#ifdef TEXTURE_COLOR_TINT_MASK
			#define tcColorTintMask tcReliefMapped
		#endif
		#ifdef TEXTURE_NORMAL
			#define tcNormal tcReliefMapped
		#endif
		#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
			#define tcReflectivity tcReliefMapped
		#endif
		#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
			#define tcEmissivity tcReliefMapped
		#endif
		#ifdef TEXTURE_REFRACTION_DISTORT
			#define tcRefractionDistort tcReliefMapped
		#endif
		#ifdef TEXTURE_AO
			#define tcAO tcReliefMapped
		#endif
		#ifdef TEXTURE_ABSORPTION
			#define tcAbsorption tcReliefMapped
		#endif
	#endif
	
	
	
	// get texture properties from textures
	#ifndef LUMINANCE_ONLY
		#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
			vec3 nonpbrAlbedo = vec3( 0 );
			#ifdef TEXTURE_NONPBR_ALBEDO
				nonpbrAlbedo = TEXTURE( texNonPbrAlbedo, tcColor ).rgb;
			#endif
			
			float nonpbrMetalness = 0;
			#ifdef TEXTURE_NONPBR_METALNESS
				nonpbrMetalness = TEXTURE( texNonPbrMetalness, tcReflectivity ).r;
			#endif
		#endif
		
		vec4 color = vec4( 0, 0, 0, 1 );
		#ifdef TEXTURE_TRANSPARENCY
			color.a = TEXTURE( texTransparency, tcColor ).r;
		#endif
		#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
			color.rgb = nonpbrMetalnessToColor( nonpbrAlbedo, nonpbrMetalness );
		#elif defined TEXTURE_COLOR
			#ifdef TEXTURE_TRANSPARENCY
				color.rgb = TEXTURE( texColor, tcColor ).rgb;
			#else
				color = TEXTURE( texColor, tcColor );
			#endif
		#elif defined WITH_OUTLINE
			color.rgb = pOutlineColor * pOutlineColorTint;
		#endif
		color.a *= pTransparencyMultiplier; // add an #ifdef to avoid this calculation?
		
		float solidity;
		#ifdef TEXTURE_SOLIDITY
			solidity = TEXTURE( texSolidity, tcColor ).r * pSolidityMultiplier;
		#elif defined WITH_OUTLINE
			solidity = pOutlineSolidity;
		#else
			solidity = pSolidityMultiplier;
		#endif
		
		#ifdef MASKED_SOLIDITY
		// ensure calculation of depth and geometry matches
		solidity = solidity < 0.35 ? 0.0 : solidity;
		#endif
		
		#ifdef TEXTURE_NORMAL
			vec4 normal = TEXTURE( texNormal, tcNormal );
			normal.xyz = normal.rgb * vec3( 1.9921569 ) + vec3( -0.9921722 );
		#else
			vec4 normal = vec4( realNormal, 0.0 ); // (0,0,1) => realNormal
		#endif
		
		float ao;
		#ifdef TEXTURE_AO
			ao = TEXTURE( texAO, tcAO ).r;
		#else
			ao = 1.0;
		#endif
		
		vec3 reflectivity;
		#if defined TEXTURE_NONPBR_ALBEDO || defined TEXTURE_NONPBR_METALNESS
			reflectivity = nonpbrMetalnessToReflectivity( nonpbrAlbedo, nonpbrMetalness );
		#elif defined TEXTURE_REFLECTIVITY
			reflectivity = TEXTURE( texReflectivity, tcReflectivity ).rgb * vec3( pReflectivityMultiplier );
		#else
			reflectivity = vec3( 0.0 );
		#endif
		
		float roughness;
		#ifdef TEXTURE_ROUGHNESS
			roughness = TEXTURE( texRoughness, tcReflectivity ).r;
		#else
			roughness = 1.0;
		#endif
		
		#ifdef TEXTURE_REFRACTION_DISTORT
			vec2 distort = TEXTURE( texRefractionDistort, tcRefractionDistort ).rg * vec2( 2.0 ) + vec2( -1.0 );
			//vec2 distort = TEXTURE( texRefractionDistort, tcRefractionDistort ).ra * vec2( 2.0 ) + vec2( -1.0 );
			distort *= pScreenSpace.xy * vec2( pRefractionDistortStrength );
		#endif
	#endif
	
	// for height map adjust alpha value
	#ifndef LUMINANCE_ONLY
		#ifdef HEIGHT_MAP
			color.a *= vHTMask;
		#endif
	#endif
	
	
	
	// environment room replaces the diffuse component
	#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
		vec3 envRoomDir = calcEnvRoomDir( tcColor, realNormal );
		
		#ifndef LUMINANCE_ONLY
			#ifdef TEXTURE_ENVROOM
				vec3 envRoomColor = textureLod( texEnvRoom, envRoomDir, 0.0 ).rgb;
			#else
				vec3 envRoomColor = vec3( 0.0 );
			#endif
			
			#ifdef TEXTURE_ENVROOM_MASK
				float envRoomMask = TEXTURE( texEnvRoomMask, tcColor ).r;
				color.rgb = mix( color.rgb, envRoomColor, vec3( envRoomMask ) );
			#else
				color.rgb = envRoomColor;
			#endif
		#endif
	#endif
	/*
	#if defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
		#ifdef OUTPUT_MATERIAL_PROPERTIES
			outDiffuse = vec4( 0.0, 0.0, 0.0, 0.0 );
			outNormal = vec4( normalZeroMaterialEnc, 0.0 );
			outReflectivity = vec4( 0.0, 0.0, 0.0, 0.0 );
			outRoughness = vec4( 0.0, 1.0, 1.0, 0.0 );
		#endif
		return;
	#endif
	*/
	
	
	
	// color, ambient occlusion and masked solidity
	#ifndef LUMINANCE_ONLY
		#ifndef WITH_EMISSIVITY
			#ifdef MASKED_SOLIDITY
				if( solidity < 0.35 ) discard;
				solidity = 1.0;
			#else
				// in the case of direct rendering values of 0 indicate pixels to not render. this is a hack right
				// now since there is no better way yet to handle this situation
				#ifndef OUTPUT_MATERIAL_PROPERTIES
					if( solidity < 0.001 ) discard;
				#endif
			#endif
		#endif
		
		color.rgb = pow( color.rgb, vec3( pColorGamma ) );
		#ifdef PARTICLE
			color *= vParticleColor;
		#else
			#ifdef TEXTURE_COLOR_TINT_MASK
				color.rgb = mix( color.rgb, color.rgb * pColorTint, TEXTURE( texColorTintMask, tcColorTintMask ).r );
			#else
				color.rgb *= pColorTint;
			#endif
		#endif
		
		color.a *= solidity;
		
		#ifdef OUTPUT_MATERIAL_PROPERTIES
			outDiffuse = color;
			#ifdef SOLIDITY_MULTIPLIER
				outDiffuse.a *= pColorSolidityMultiplier;
			#endif
		#endif
		
		#ifdef AMBIENT_LIGHT_PROBE
			//outColor = color * vec4( textureLod( texEnvMap, -vReflectDir, 12.0 ).rgb, 1.0 );
			outColor = color * vec4( pParticleLightHack, 1.0 );
		#else
			outColor = color * pAmbient;
		#endif
		#ifdef TEXTURE_RENDERCOLOR
			outColor.rgb *= vec3( color.a );
		#endif
		
		ao = pow( ao, pColorGamma ); // this is a hack and has to be replaced with a proper ambient.occlusion.gamma texture property
	#endif
	
	
	
	// normal and normal variance
	#ifndef LUMINANCE_ONLY
		#if defined TEXTURE_NORMAL || defined NODE_FRAGMENT_MAIN
			normal.xyz = vTangent * vec3( normal.x ) + vBitangent * vec3( normal.y ) + realNormal * vec3( normal.z );
		#endif
		
		#ifdef TP_NORMAL_STRENGTH
			// mix() is not an option since the texture property can be negative or larger than 1 for special effects
			normal.xyz = ( normal.xyz - realNormal ) * vec3( pNormalStrength ) + realNormal;
			normal.w *= abs( pNormalStrength );
		#endif
		
		// various hacks that should go away later on
		if( dot( normal.xyz, normal.xyz ) < 1e-6 ){
			normal = vec4( 0.0, 0.0, 1.0, 0.0 );
		}
		
		// normalize is required for the later passes to work correctly
		normal.xyz = normalize( normal.xyz );
		#ifdef OUTPUT_MATERIAL_PROPERTIES
			#ifdef WITH_OUTLINE
				outNormal = vec4( normalZeroMaterialEnc, color.a );
			#else
				outNormal = vec4( normalEncodeMaterial( normal.xyz ), color.a );
				#ifdef SOLIDITY_MULTIPLIER
					outNormal.a *= pNormalSolidityMultiplier;
				#endif
			#endif
		#endif
	#endif
	
	
	
	// reflection and refraction
	#if ! defined WITH_OUTLINE && ! defined LUMINANCE_ONLY
		#if defined OUTPUT_MATERIAL_PROPERTIES || defined TEXTURE_ENVMAP
			reflectivity.rgb = pow( reflectivity.rgb, vec3( pColorGamma ) );
			roughness = pow( clamp( roughness, 0.0, 1.0 ), pRoughnessGamma );
			#ifdef TP_ROUGHNESS_REMAP
				roughness = clamp( roughness * pRoughnessRemap.x + pRoughnessRemap.y, 0.0, 1.0 );
			#endif
			#ifdef USE_NORMAL_ROUGHNESS_CORRECTION
				roughness = min( roughness + normal.w * pNorRoughCorrStrength, 1.0 ); // apply normal variance as roughness increase
			#endif
		#endif
		
		#if defined TEXTURE_ENVMAP || defined TEXTURE_RIM_EMISSIVITY
			vec3 fragmentDirection = normalize( vReflectDir );
		#endif
		
		#ifdef TEXTURE_ENVMAP
			float reflectDot = min( abs( dot( -fragmentDirection, normal.xyz ) ), 1.0 );
			vec3 envMapDir = pMatrixEnvMap * vec3( reflect( fragmentDirection, normal.xyz ) );
			
			//float fresnelReduction = smoothstep( 0.5, 1.0, 1.0 - roughness );
			//float fresnelReduction = 1.0 / ( 1.0 + roughness * 3.0 ); // CoD version... no less than 25% reflectivity at fully grazing angle
			//float fresnelReduction = 1.0 - roughness;
			
			// pow(5) fits betters but pow(4) is cheaper:
			//float fresnelReduction = pow( 1.0 - roughness, 5.0 );
		//	vec3 fresnelReduction = vec3( pow( 1.0 - roughness, 5.0 ) * 0.9524 + 0.0476 );
			vec3 fresnelReduction = mix( reflectivity.rgb, vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
		//	vec3 fresnelReduction = mix( mix( vec3( 0.056 ), vec3( 1.0 ), reflectivity.rgb ), vec3( 1.0 ), pow( 1.0 - roughness, 5.0 ) );
			
			// reduce reflectivity depending on AO and roughness angles
			//float aoAngle = 1.0 - acos( 1.0 - ao ) * 0.636620; // * 2/pi
			//fresnelReduction *=  1.0 - smoothstep( aoAngle - roughness, aoAngle + roughness, reflectDot );
			// WARNING! the mix function is very fragile and passing under 0 or over 1 by even just a small
			//          amount results in NaN values and other nasty stuff. clamping costs some cycles
			//          but it prevents the mix function from smuggling NaN values into the image
			fresnelReduction *= vec3( clamp(
				( acos( 1.0 - ao ) + roughness * 1.5707963 - acos( reflectDot ) + 0.01 )
					/ max( roughness * 3.14159265, 0.01 ),
				0.0, 1.0 ) );
			vec3 fresnelFactor = vec3( clamp( pow( 1.0 - reflectDot, 5.0 ), 0.0, 1.0 ) );
			vec3 envMapReflectivity = mix( reflectivity.rgb, vec3( 1.0 ), vec3( fresnelFactor ) )
				* fresnelReduction * vec3( solidity );
// 			fresnelFactor *= fresnelReduction;
// 			
// 			vec4 fresnelFactorWithAlpha = vec4( fresnelFactor, dot( fresnelFactor, vec3( 1.0 / 3.0 ) ) ) * vec4( solidity );
// 			color = mix( color, colorTransparent, fresnelFactorWithAlpha );
// 			#ifdef TEXTURE_RENDERCOLOR
// 				outColor = mix( outColor, colorTransparent, fresnelFactorWithAlpha );
// 			#endif
			
			#ifdef PARTICLE
				envMapReflectivity *= vParticleColor.a;
			#endif
		#endif
	#endif
	
	#ifdef TEXTURE_RENDERCOLOR
		vec4 renderColor;
		#if defined TEXTURE_REFRACTION_DISTORT && ! defined WITH_OUTLINE
			renderColor = textureLod( texRenderColor, vec3( clamp(
				gl_FragCoord.xy * pScreenSpace.zw + distort, pViewport.xy, pViewport.zw ), vLayer ), 0.0 );
		#else
			renderColor = texelFetch( texRenderColor, ivec3( tc, vLayer ), 0 );
		#endif
		
		outColor.rgb = mix( renderColor.rgb, outColor.rgb, color.a );
		outColor.a = min( outColor.a + renderColor.a, 1.0 );
			// vec3(color.a) is a fake solution for true transmission(distanceThroughMaterial).rgb
		//outColor = mix( outColor, vec4( 0.0, 0.0, 0.0, 1.0 ), vec4( fresnelFactor * fresnelReduction ) );
	#endif
	
	#if ! defined OUTPUT_MATERIAL_PROPERTIES && ! defined LUMINANCE_ONLY
		// for direct rendering apply normals as pre-baked lighting
		outColor.rgb *= vec3( 0.25 - clamp( normal.z, -1.0, 0.0 ) * 0.75 );
		
		// for direct rendering the transparency and solidity has to be premultiplied to
		// allow emissivity to properly apply. this is due to use of blend mode (1,1-alpha):
		//   f = d*(1-a) + (s*a + e)*1
		outColor.rgb *= vec3( outColor.a * solidity );
			// this does not work correctly with particles having solid appearance in
			// volumetric-mode
	#endif
	
	#if ! defined WITH_OUTLINE && ! defined LUMINANCE_ONLY && defined TEXTURE_ENVMAP
		#ifndef SKIN_REFLECTIONS
		if( pSkinDoesReflections ){
		#endif
			float envMapLodLevel = log2( 1.0 + pEnvMapLodLevel * roughness );
			
			#ifdef TEXTURE_ENVMAP_EQUI
				envMapDir = normalize( envMapDir );
				vec2 tcEnvMap = cemefac.xy + cemefac.zw * vec2(
					atan( envMapDir.x, envMapDir.z ), acos( envMapDir.y ) );
			#else
				#define tcEnvMap envMapDir
			#endif
			
			vec3 reflectedColor = textureLod( texEnvMap, tcEnvMap, envMapLodLevel ).rgb;
			#if defined TEXTURE_ENVMAP_FADE && ! defined PARTICLE
				reflectedColor = mix( textureLod( texEnvMapFade, tcEnvMap, envMapLodLevel ).rgb,
					reflectedColor, vec3( pEnvMapFade ) );
			#endif
			
			// this should simulate to some degree a prefiltered environment map (cosine filter, phong lobe).
			//reflectedColor *= vec3( ( cos( roughness * 3.1416 ) * 0.5 + 0.5 ) * 0.38 + 0.62 );
			// this version is based on a normalized situation and should be somewhat better
		//	reflectedColor *= vec3( 0.2 + 0.8 / ( 1.0 + 50.0 * roughness * roughness ) );
			
			outColor.rgb += reflectedColor * envMapReflectivity;
		#ifndef SKIN_REFLECTIONS
		}
		#endif
	#endif
	
	#ifdef OUTPUT_MATERIAL_PROPERTIES
		#ifdef WITH_OUTLINE
			outReflectivity = vec4( 0.0, 0.0, 0.0, color.a );
			outRoughness = vec4( 1.0, 1.0, 1.0, color.a );
			outAOSolidity = vec4( 1.0, 1.0, solidity, color.a );
		#else
			outReflectivity = vec4( reflectivity.rgb, color.a );
			#ifdef SOLIDITY_MULTIPLIER
				outReflectivity.a *= pReflectivitySolidityMultiplier;
			#endif
			outRoughness = vec4( roughness, 1.0, 1.0, color.a );
			#ifdef SOLIDITY_MULTIPLIER
				outRoughness.a *= pRoughnessSolidityMultiplier;
			#endif
			
			outAOSolidity = vec4( ao, 1.0, solidity, color.a );
		#endif
	#endif
	
	
	
	// absorption and sub-surf scattering
	#ifdef OUTPUT_MATERIAL_PROPERTIES
		#ifdef WITH_OUTLINE
			outSubSurface = vec4( 0.0, 0.0, 0.0, color.a );
		#else
			outSubSurface = vec4( vec3( pTexAbsorptionRange ), color.a );
			#ifdef TEXTURE_ABSORPTION
				outSubSurface.rgb *= pow( TEXTURE( texAbsorption, tcAbsorption ).rgb, vec3( pColorGamma ) ); // hack, needs pAbsorptionGamma
			#endif
		#endif
	#endif
	
	
	
	// emissivity
	#ifdef LUMINANCE_ONLY
		vec3 outColor = vec3( 0.0 );
	#endif
	
	#ifdef WITH_OUTLINE
		outColor.rgb += pOutlineEmissivity * pOutlineEmissivityTint;
		
	#else
		#ifdef TEXTURE_EMISSIVITY
			#ifdef PARTICLE
				#define SCALE_EMISSIVITY vParticleColor.xyz * finalEmissivityIntensity( vParticleEmissivity )
			#else
				#define SCALE_EMISSIVITY finalEmissivityIntensity( pEmissivityIntensity )
			#endif
			outColor.rgb += pow( TEXTURE( texEmissivity, tcEmissivity ).rgb, vec3( pColorGamma ) ) * SCALE_EMISSIVITY;
		#endif
		
		#ifdef TEXTURE_ENVROOM_EMISSIVITY
			#ifdef TEXTURE_ENVROOM_MASK
				outColor.rgb += textureLod( texEnvRoomEmissivity, envRoomDir, 0.0 ).rgb
					* finalEmissivityIntensity( pEnvRoomEmissivityIntensity ) * vec3( envRoomMask );
			#else
				outColor.rgb += textureLod( texEnvRoomEmissivity, envRoomDir, 0.0 ).rgb
					* finalEmissivityIntensity( pEnvRoomEmissivityIntensity );
			#endif
		#endif
		
		#ifdef TEXTURE_RIM_EMISSIVITY
			if( pRimAngle > 0.5 ){
				// deoglSkinTexture: pRimAngle = angle > 0.001 ? 1 / ( angle * pi/2 ) : 0
				// for "angle = 0.001 .. 1" we have "pRimAngle = 0.637 .. 636.62". hence 0.5 as threshold
				// 
				// using "normal" is not giving the results one expects especially if close up.
				// instead the normal is dotted with the normalized fragment direction.
				outColor.rgb += pow( TEXTURE( texRimEmissivity, tcEmissivity ).rgb, vec3( pColorGamma ) )
					* finalEmissivityIntensity( pRimEmissivityIntensity )
					* vec3( max( 1.0 - pow( asin( abs( dot( fragmentDirection, normal.xyz ) ) )
						* pRimAngle, pRimExponent ), 0.0 ) );
			}
		#endif
	#endif
	
	#ifdef LUMINANCE_ONLY
		outLuminance = vec3( dot( outColor, lumiFactors ) );
		outNormal = realNormal;
	#endif
	
	/* #ifdef HEIGHT_MAP
		outColor.rgb = vec3( vHTMask );
	#endif */
	
	
	
	// fade fragment near render range
	/* produces artifacts with lighting since alpha-blend does not work in this case
	#ifdef FADEOUT_RANGE
		outColor.a *= 1.0 - clamp( vFadeZ, pFadeRange.x, pFadeRange.y ) * pFadeRange.z;
	#endif
	*/
}
