// request high precision if the graphic card supports this
#ifdef HIGH_PRECISION
precision highp float;
precision highp int;
#endif

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
