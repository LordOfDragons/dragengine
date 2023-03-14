// element flags
const uint efComponent = uint( 0x1 );
const uint efBillboard = uint( 0x2 );
const uint efParticleEmitter = uint( 0x4 );
const uint efLight = uint( 0x8 );
const uint efPropFieldCluster = uint( 0x10 );
const uint efHeightTerrainSectorCluster = uint( 0x20 );
const uint efDecal = uint( 0x40 );
const uint efStatic = uint( 0x80 );
const uint efDynamic = uint( 0x100 );
const uint efGIStatic = uint( 0x200 );
const uint efGIDynamic = uint( 0x400 );


// element render filters
const uint erfRender = uint( 0x1 );
const uint erfSolid = uint( 0x2 );
const uint erfShadowNone = uint( 0x4 );
const uint erfReflected = uint( 0x8 );
const uint erfRendered = uint( 0x10 );
const uint erfOutline = uint( 0x20 );
const uint erfOutlineSolid = uint( 0x40 );
const uint erfHoles = uint( 0x80 );
const uint erfDecal = uint( 0x100 );
const uint erfDoubleSided = uint( 0x200 );
const uint erfXRay = uint( 0x400 );
const uint erfOcclusion = uint( 0x800 );


// element pipeline lists
const uint eplComponent = uint( 0 );
const uint eplBillboard = uint( 1 );
const uint eplDecal = uint( 2 );
const uint eplPropField = uint( 3 );
const uint eplPropFieldImposter = uint( 4 );
const uint eplHeightMap1 = uint( 5 );
const uint eplHeightMap2 = uint( 6 );
const uint eplParticle = uint( 7 );
const uint eplParticleRibbon = uint( 8 );
const uint eplParticleBeam = uint( 9 );
const uint eplOutline = uint( 10 );


// element pipeline types
const uint eptGeometry = uint( 0 );
const uint eptGeometryDepthTest = uint( 1 );
const uint eptDepth = uint( 2 );
const uint eptDepthClipPlane = uint( 3 );
const uint eptDepthReversed = uint( 4 );
const uint eptDepthClipPlaneReversed = uint( 5 );
const uint eptCounter = uint( 6 );
const uint eptCounterClipPlane = uint( 7 );
const uint eptMask = uint( 8 );
const uint eptShadowProjection = uint( 9 );
const uint eptShadowProjectionCube = uint( 10 );
const uint eptShadowOrthogonal = uint( 11 );
const uint eptShadowOrthogonalCascaded = uint( 12 );
const uint eptShadowDistance = uint( 13 );
const uint eptShadowDistanceCube = uint( 14 );
const uint eptEnvMap = uint( 15 );
const uint eptLuminance = uint( 16 );
const uint eptGIMaterial = uint( 17 );


// element pipeline modifiers
const uint epmStereo = uint( 0x1 );
const uint epmFlipCullFace = uint( 0x2 );
const uint epmDoubleSided = uint( 0x4 );


// element texture tuc
const uint ettGeometry = uint( 0 );
const uint ettDepth = uint( 1 );
const uint ettCounter = uint( 2 );
const uint ettShadow = uint( 3 );
const uint ettShadowCube = uint( 4 );
const uint ettEnvMap = uint( 5 );
const uint ettLuminance = uint( 6 );
const uint ettGIMaterial = uint( 7 );


// cull result
const uint ecrVisible = uint( 0x100 );


// lod method
const uint lmLowest = uint( 0 );
const uint lmHighest = uint( 1 );
const uint lmProjection = uint( 2 );
const uint lmOrthogonal = uint( 3 );
