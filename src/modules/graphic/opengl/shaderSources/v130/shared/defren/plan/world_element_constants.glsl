// element flags
const uint efComponent = uint( 0x1 );
const uint efComponentDynamic = uint( 0x2 );
const uint efBillboard = uint( 0x4 );
const uint efParticleEmitter = uint( 0x8 );
const uint efLight = uint( 0x10 );
const uint efPropFieldCluster = uint( 0x20 );
const uint efHeightTerrainSectorCluster = uint( 0x40 );


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


// element pipeline modifiers
const uint epmDoubleSided = uint( 0x4 ); // skin texture
