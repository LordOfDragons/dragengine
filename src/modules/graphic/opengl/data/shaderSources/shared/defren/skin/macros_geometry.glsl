/*
	<!-- shared/defren/skin/macros_geometry.glsl -->
	<define>DECAL</define>
	<define>DEPTH_TEST</define>
	<define>OUTPUT_COLOR</define>
	<define>SKIN_CLIP_PLANE</define>
	<define>TEXTURE_EMISSIVITY</define>
	<define>TEXTURE_ENVMAP</define>
	<define>TEXTURE_ENVROOM_EMISSIVITY</define>
	<define>TEXTURE_ENVROOM</define>
	<define>TEXTURE_HEIGHT</define>
	<define>TEXTURE_NORMAL</define>
	<define>TEXTURE_REFLECTIVITY</define>
	<define>TEXTURE_RENDERCOLOR</define>
	<define>TEXTURE_RIM_EMISSIVITY</define>
	<define>TEXTURE_ROUGHNESS</define>
	<define>TEXTURE_SOLIDITY</define>
	<define>WITH_OUTLINE</define>
	<define>WITH_REFLECTION</define>
*/

precision HIGHP float;
precision HIGHP int;

// some helper definitions to make the code easier to read
#if defined OUTPUT_COLOR || defined TEXTURE_SOLIDITY || defined TEXTURE_HEIGHT
	#define _REQ_TEX_CLR_1 1
#endif
#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY
	#define _REQ_TEX_CLR_2 1
#endif

#if defined TEXTURE_RENDERCOLOR || defined TEXTURE_SOLIDITY || defined DEPTH_TEST
	#define REQUIRES_TEX_COORD 1
#endif

#ifdef DECAL
	#define SOLIDITY_MULTIPLIER 1
#endif

#if defined TEXTURE_REFLECTIVITY || defined TEXTURE_ROUGHNESS
	#define WITH_REFLECTIVITY 1
#endif

#if defined TEXTURE_SOLIDITY || defined WITH_OUTLINE || defined SKIN_CLIP_PLANE
	#define WITH_SOLIDITY 1
#endif

#if defined TEXTURE_EMISSIVITY || defined TEXTURE_RIM_EMISSIVITY || defined WITH_OUTLINE
	#define WITH_EMISSIVITY 1
#endif

#if defined WITH_REFLECTION || defined TEXTURE_ENVMAP || defined TEXTURE_RIM_EMISSIVITY || defined TEXTURE_ENVROOM || defined TEXTURE_ENVROOM_EMISSIVITY
	#define WITH_REFLECT_DIR 1
#endif

#if defined TEXTURE_NORMAL || defined WITH_REFLECT_DIR
	#define WITH_TANGENT 1
	#define WITH_BITANGENT 1
#endif

#if defined TEXTURE_HEIGHT || defined WITH_TANGENT || defined WITH_BITANGENT
	#define REQUIRES_NORMAL 1
#endif

#if defined _REQ_TEX_CLR_1 || defined _REQ_TEX_CLR_2 || REQUIRES_NORMAL
	#define REQUIRES_TEX_COLOR 1
#endif
