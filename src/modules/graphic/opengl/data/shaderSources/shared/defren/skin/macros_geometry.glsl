/*
	<!-- shared/defren/skin/macros_geometry.glsl -->
	<define>GEOMETRY_MODE</define>
	<define>DEPTH_TEST</define>
	<define>OUTPUT_MODE</define>
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

const bool SolidityMultiplier = GeometryMode == GeometryModeDecal;
const bool WithSolidity = TextureSolidity || WithOutline || SkinClipPlane;
const bool WithEmissivity = TextureEmissivity || TextureRimEmissivity || WithOutline;

const bool WithReflectDir = WithReflection || TextureEnvMap || TextureRimEmissivity
	|| TextureEnvRoom || TextureEnvRoomEmissivity;

const bool RequireNormal = TextureHeight || TextureNormal || WithReflectDir;

const bool RequireTextureColor = OutputMode == OutputModeColor || TextureSolidity
	|| TextureHeight || TextureEmissivity || TextureRimEmissivity || RequireNormal;
