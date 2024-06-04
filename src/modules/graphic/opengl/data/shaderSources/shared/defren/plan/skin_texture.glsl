
/*
for skin-texture there are 11 pipelines lists each with 18 pipeline types each
with 8 pipeline modifiers. this is a total of 11*18*8 = 1584 pipelines. this can
be stored as 396 uvec4 with a total of 6336 bytes per entry. With for example
500 skin textures this results in over 3.2MB for these tables.

pipelines between pipeline lists, pipeline types and pipeline modifiers are in
general not compatible. the large tables in GPU can thus be avoided.

sort order:
- pipeline lists
- pipeline type
- pipeline modifiers

this sorting results now in groups containing potentially shared pipelines.
pipelies in skin textures have though a certain relationship. in particular
if we sort by the most granular pipeline type (typically geometry) we get a
sorting which is also usable for coarse grained pipeline types. this works
if during rendering the active pipeline is tracked and switched only if an
upcoming render step uses a different one. this should still result in an
efficient rendering without redundant pipeline switches. the sort order value
below is just the pipeline index of the most fine grained pipeline but could
be any number which has the same properties.

the sorting thus becomes this:
- skin texture sort order
- element texture sort order
- element instance index

with element texture sort order we have the same problem as with skin texture
pipelines. each pipeline type has potentially a different tuc. thus 18 tucs
are possible for elements but in general less than these are used. here too
one tuc is the finest grained one (typically geometry). here too we can use
the tuc index of the finest grained one as sort order. this way another look
up table can be avoided by tracking during rendering the last activated tuc.

to improve the sorting and reducing memory consumption the pipeline list,
pipeline type and pipeline modifiers are combined into one value like this:
- sortPipeline = (pipelineList << 11) | (pipelineType << 5) | pipelineModifiers

in glsl this can be handled using bitfieldInsert and bitfieldExtract. the
offsets are due to the bit requirements:
- pipeline lists: 11 values => 4 bits
- pipeline type: 18 values => 5 bits
- pipeline modifiers: 8 values => 3 bits

this yields a total of 12 bits. we want to keep this under 16 bits so we
can potentially use 16-bit integer. to avoid future changes the bits are
extended a bit like this:
- pipeline lists: 5 bits
- pipeline type: 6 bits
- pipeline modifiers: 5 bits
*/


// define this somewhere. variable name pSkinTexturePipelines has to stay intact
/*
UBOLAYOUT_BIND(2) readonly buffer SkinTexturePipelines {
	uvec4 pSkinTexturePipelines[];
};
*/


// constants
const uint vSkinTextureListCount = uint( 11 );
const uint vSkinTextureTypeCount = uint( 18 );
const uint vSkinTextureModifierCount = uint( 8 );

const uint vSkinTexturePipelinesPerType = vSkinTextureModifierCount;
const uint vSkinTexturePipelinesPerList = vSkinTexturePipelinesPerType * vSkinTextureTypeCount;
const uint vSkinTexturePipelinesPerTexture = vSkinTexturePipelinesPerList * vSkinTextureListCount;
