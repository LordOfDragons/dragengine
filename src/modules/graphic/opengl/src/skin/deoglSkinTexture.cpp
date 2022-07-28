/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRSkin.h"
#include "deoglSkinCalculatedProperty.h"
#include "deoglSkinTexture.h"
#include "deoglSkinPropertyMap.h"
#include "deoglSkinRenderable.h"
#include "shader/deoglSkinShaderManager.h"
#include "state/deoglSkinState.h"
#include "state/deoglSkinStateRenderable.h"
#include "visitor/deoglVSDetermineChannelFormat.h"
#include "../deoglCaches.h"
#include "../deGraphicOpenGl.h"
#include "../capabilities/deoglCapsTextureFormat.h"
#include "../configuration/deoglConfiguration.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../rendering/deoglRenderReflection.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTChoices.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../shaders/paramblock/deoglSPBlockSSBO.h"
#include "../shaders/paramblock/deoglShaderParameterBlock.h"
#include "../shaders/paramblock/shared/deoglSharedSPBList.h"
#include "../shaders/paramblock/shared/deoglSharedSPBElement.h"
#include "../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"
#include "../texture/arraytexture/deoglArrayTexture.h"
#include "../texture/compression/deoglTextureCompression.h"
#include "../texture/cubemap/deoglCubeMap.h"
#include "../texture/deoglImage.h"
#include "../texture/deoglRImage.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/pixelbuffer/deoglPixelBufferMipMap.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinPropertyColor.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyValue.h>
#include <dragengine/resources/skin/property/deSkinPropertyVideo.h>
#include <dragengine/resources/skin/property/deSkinPropertyMapped.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitorIdentify.h>



// Definitions
////////////////

#define ENABLE_CACHE_LOGGING		false

#define CACHE_FILE_VERSION			1

enum eShaderConfigurationTypes{
	esctGeometry,
	esctDepth,
	esctCounter,
	esctShadow,
	esctEnvMap,
	esctOutlineGeometry,
	esctOutlineDepth,
	esctOutlineCounter
};

//#define SSC( enumentry ) deoglSkinShaderConfig::enumentry
#define SCIE(type, clipPlane, billboard, stereo, shaderMode, geometryMode, depthMode, particleMode, depthTestMode) \
	{ esct##type, clipPlane, billboard, stereo, \
		deoglSkinShaderConfig::esm##shaderMode, \
		deoglSkinShaderConfig::egm##geometryMode, \
		deoglSkinShaderConfig::edm##depthMode, \
		deoglSkinShaderConfig::epm##particleMode, \
		deoglSkinShaderConfig::edtm##depthTestMode }

struct sShaderConfigInfo{
	eShaderConfigurationTypes type;
	bool clipPlane;
	bool billboard;
	bool stereo;
	deoglSkinShaderConfig::eShaderModes shaderMode;
	deoglSkinShaderConfig::eGeometryModes geometryMode;
	deoglSkinShaderConfig::eDepthModes depthMode;
	deoglSkinShaderConfig::eParticleModes particleMode;
	deoglSkinShaderConfig::eDepthTestModes depthTestMode;
};

static sShaderConfigInfo vShaderConfigInfo[ deoglSkinTexture::ShaderTypeCount ] = {
	// Component
	//   Depth Node edmDepth
	//     depth test mode is used only for !pSolid. For pSolid edtmNone is used
	
	// estComponentGeometry
	SCIE(Geometry, false, false, false, Geometry, Component, Projection, Particle, None),
	// estComponentDepth
	SCIE(Depth, false, false, false, Depth, Component, Projection, Particle, Larger),
	// estComponentDepthClipPlane
	SCIE(Depth, true, false, false, Depth, Component, Projection, Particle, Larger),
	// estComponentDepthReversed
	SCIE(Depth, false, false, false, Depth, Component, Projection, Particle, Smaller),
	// estComponentDepthClipPlaneReversed
	SCIE(Depth, true, false, false, Depth, Component, Projection, Particle, Smaller),
	// estComponentCounter
	SCIE(Counter, false, false, false, Depth, Component, Projection, Particle, None),
	// estComponentCounterClipPlane
	SCIE(Counter, true, false, false, Depth, Component, Projection, Particle, None),
	// estComponentShadowProjection
	SCIE(Shadow, false, false, false, Depth, Component, Projection, Particle, None),
	// estComponentShadowOrthogonal
	SCIE(Shadow, false, false, false, Depth, Component, Orthogonal, Particle, None),
	// estComponentShadowOrthogonalCascaded
	SCIE(Shadow, false, false, false, Depth, Component, Orthogonal, Particle, None),
	// estComponentShadowDistance
	SCIE(Shadow, false, false, false, Depth, Component, Distance, Particle, None),
	// estComponentShadowDistanceCube
	SCIE(Shadow, false, false, false, Depth, Component, Distance, Particle, None),
	// estComponentEnvMap
	SCIE(EnvMap, false, false, false, EnvMap, Component, Projection, Particle, None),
	// estComponentLuminance
	SCIE(Geometry, false, false, false, Geometry, Component, Projection, Particle, None),
	// estComponentGIMaterial
	SCIE(Geometry, false, false, false, Geometry, Component, Projection, Particle, None),
	
	
	// Billboard
	// estBillboardGeometry
	SCIE(Geometry, false, true, false, Geometry, Billboard, Projection, Particle, None),
	// estBillboardDepth
	SCIE(Depth, false, true, false, Depth, Billboard, Projection, Particle, Larger),
	// estBillboardDepthClipPlane
	SCIE(Depth, true, true, false, Depth, Billboard, Projection, Particle, Larger),
	// estBillboardDepthReversed
	SCIE(Depth, false, true, false, Depth, Billboard, Projection, Particle, Smaller),
	// estBillboardDepthClipPlaneReversed
	SCIE(Depth, true, true, false, Depth, Billboard, Projection, Particle, Smaller),
	// estBillboardCounter
	SCIE(Counter, false, true, false, Depth, Billboard, Projection, Particle, None),
	// estBillboardCounterClipPlane
	SCIE(Counter, true, true, false, Depth, Billboard, Projection, Particle, None),
	// estBillboardEnvMap
	SCIE(EnvMap, false, true, false, EnvMap, Billboard, Projection, Particle, None),
	
	
	// Decal
	
	// estDecalGeometry
	SCIE(Geometry, false, false, false, Geometry, Decal, Projection, Particle, None),
	// estDecalEnvMap
	SCIE(EnvMap, false, false, false, EnvMap, Decal, Projection, Particle, None),
	
	
	
	// Prop Field
	//   Depth Node edmDepth
	//     depth test mode is used only for !pSolid. For pSolid edtmNone is used
	
	// estPropFieldGeometry
	SCIE(Geometry, false, false, false, Geometry, PropField, Projection, Particle, None),
	// estPropFieldImposterGeometry
	SCIE(Geometry, false, true, false, Geometry, PropField, Projection, Particle, None),
	// estPropFieldDepth
	SCIE(Depth, false, false, false, Depth, PropField, Projection, Particle, Larger),
	// estPropFieldImposterDepth
	SCIE(Depth, false, true, false, Depth, PropField, Projection, Particle, Larger),
	// estPropFieldDepthClipPlane
	SCIE(Depth, true, false, false, Depth, PropField, Projection, Particle, Larger),
	// estPropFieldImposterDepthClipPlane
	SCIE(Depth, true, true, false, Depth, PropField, Projection, Particle, Larger),
	// estPropFieldDepthReversed
	SCIE(Depth, false, false, false, Depth, PropField, Projection, Particle, Smaller),
	// estPropFieldImposterDepthReversed
	SCIE(Depth, false, true, false, Depth, PropField, Projection, Particle, Smaller),
	// estPropFieldDepthClipPlaneReversed
	SCIE(Depth, true, false, false, Depth, PropField, Projection, Particle, Smaller),
	// estPropFieldImposterDepthClipPlaneReversed
	SCIE(Depth, true, true, false, Depth, PropField, Projection, Particle, Smaller),
	// estPropFieldCounter
	SCIE(Counter, false, false, false, Depth, PropField, Projection, Particle, None),
	// estPropFieldCounterClipPlane
	SCIE(Counter, true, false, false, Depth, PropField, Projection, Particle, None),
	// estPropFieldShadowProjection
	SCIE(Shadow, false, false, false, Depth, PropField, Projection, Particle, None),
	// estPropFieldShadowOrthogonal
	SCIE(Shadow, false, false, false, Depth, PropField, Orthogonal, Particle, None),
	// estPropFieldShadowDistance
	SCIE(Shadow, false, false, false, Depth, PropField, Distance, Particle, None),
	// estPropFieldEnvMap
	SCIE(EnvMap, false, false, false, EnvMap, PropField, Projection, Particle, None),
	
	
	// Height Terrain
	//   Depth Node edmDepth
	//     depth test mode is used only for !pSolid. For pSolid edtmNone is used
	
	// estHeightMapGeometry
	SCIE(Geometry, false, false, false, Geometry, HeightMap, Projection, Particle, None),
	// estHeightMapDepth
	SCIE(Depth, false, false, false, Depth, HeightMap, Projection, Particle, Larger),
	// estHeightMapDepthClipPlane
	SCIE(Depth, true, false, false, Depth, HeightMap, Projection, Particle, Larger),
	// estHeightMapDepthReversed
	SCIE(Depth, false, false, false, Depth, HeightMap, Projection, Particle, Smaller),
	// estHeightMapDepthClipPlaneReversed
	SCIE(Depth, true, false, false, Depth, HeightMap, Projection, Particle, Smaller),
	// estHeightMapCounter
	SCIE(Counter, false, false, false, Depth, HeightMap, Projection, Particle, None),
	// estHeightMapCounterClipPlane
	SCIE(Counter, true, false, false, Depth, HeightMap, Projection, Particle, None),
	// estHeightMapShadowProjection
	SCIE(Shadow, false, false, false, Depth, HeightMap, Projection, Particle, None),
	// estHeightMapShadowOrthogonal
	SCIE(Shadow, false, false, false, Depth, HeightMap, Orthogonal, Particle, None),
	// estHeightMapShadowDistance
	SCIE(Shadow, false, false, false, Depth, HeightMap, Distance, Particle, None),
	// estHeightMapEnvMap
	SCIE(EnvMap, false, false, false, EnvMap, HeightMap, Projection, Particle, None),
	// estHeightMapLuminance
	SCIE(Geometry, false, false, false, Geometry, HeightMap, Projection, Particle, None),
	
	
	// Particle Emitter
	
	// estParticleGeometry
	SCIE(Geometry, false, false, false, Geometry, Particle, Projection, Particle, None),
	// estParticleGeometryDepthtest
	SCIE(Geometry, false, false, false, Geometry, Particle, Projection, Particle, Larger),
	// estParticleDepth
	SCIE(Depth, false, false, false, Depth, Particle, Projection, Particle, Larger),
	// estParticleDepthClipPlane
	SCIE(Depth, true, false, false, Depth, Particle, Projection, Particle, Larger),
	// estParticleDepthReversed
	SCIE(Depth, false, false, false, Depth, Particle, Projection, Particle, Smaller),
	// estParticleDepthClipPlaneReversed
	SCIE(Depth, true, false, false, Depth, Particle, Projection, Particle, Smaller),
	// estParticleShadowProjection
	SCIE(Shadow, false, false, false, Depth, Particle, Projection, Particle, None),
	// estParticleShadowOrthogonal
	SCIE(Shadow, false, false, false, Depth, Particle, Orthogonal, Particle, None),
	// estParticleShadowDistance
	SCIE(Shadow, false, false, false, Depth, Particle, Distance, Particle, None),
	// estParticleCounter
	SCIE(Counter, false, false, false, Depth, Particle, Projection, Particle, None),
	// estParticleCounterClipPlane
	SCIE(Counter, true, false, false, Depth, Particle, Projection, Particle, None),
	// estParticleRibbonGeometry
	SCIE(Geometry, false, false, false, Geometry, Particle, Projection, Ribbon, None),
	// estParticleRibbonGeometryDepthtest
	SCIE(Geometry, false, false, false, Geometry, Particle, Projection, Ribbon, Larger),
	// estParticleRibbonDepth
	SCIE(Depth, false, false, false, Depth, Particle, Projection, Ribbon, Larger),
	// estParticleRibbonDepthClipPlane
	SCIE(Depth, true, false, false, Depth, Particle, Projection, Ribbon, Larger),
	// estParticleRibbonDepthReversed
	SCIE(Depth, false, false, false, Depth, Particle, Projection, Ribbon, Smaller),
	// estParticleRibbonDepthClipPlaneReversed
	SCIE(Depth, true, false, false, Depth, Particle, Projection, Ribbon, Smaller),
	// estParticleRibbonCounter
	SCIE(Counter, false, false, false, Depth, Particle, Projection, Ribbon, None),
	// estParticleRibbonCounterClipPlane
	SCIE(Counter, true, false, false, Depth, Particle, Projection, Ribbon, None),
	// estParticleBeamGeometry
	SCIE(Geometry, false, false, false, Geometry, Particle, Projection, Beam, None),
	// estParticleBeamGeometryDepthtest
	SCIE(Geometry, false, false, false, Geometry, Particle, Projection, Beam, Larger),
	// estParticleBeamDepth
	SCIE(Depth, false, false, false, Depth, Particle, Projection, Beam, Larger),
	// estParticleBeamDepthClipPlane
	SCIE(Depth, true, false, false, Depth, Particle, Projection, Beam, Larger),
	// estParticleBeamDepthReversed
	SCIE(Depth, false, false, false, Depth, Particle, Projection, Beam, Smaller),
	// estParticleBeamDepthClipPlaneReversed
	SCIE(Depth, true, false, false, Depth, Particle, Projection, Beam, Smaller),
	// estParticleBeamCounter
	SCIE(Counter, false, false, false, Depth, Particle, Projection, Beam, None),
	// estParticleBeamCounterClipPlane
	SCIE(Counter, true, false, false, Depth, Particle, Projection, Beam, None),
	
	
	
	// Outline
	// estOutlineGeometry
	SCIE(OutlineGeometry, false, false, false, Geometry, Component, Projection, Particle, None),
	// estOutlineDepth
	SCIE(OutlineDepth, false, false, false, Depth, Component, Projection, Particle, Larger),
	// estOutlineDepthClipPlane
	SCIE(OutlineDepth, true, false, false, Depth, Component, Projection, Particle, Larger),
	// estOutlineDepthReversed
	SCIE(OutlineDepth, false, false, false, Depth, Component, Projection, Particle, Smaller),
	// estOutlineDepthClipPlaneReversed
	SCIE(OutlineDepth, true, false, false, Depth, Component, Projection, Particle, Smaller),
	// estOutlineCounter
	SCIE(OutlineCounter, false, false, false, Depth, Component, Projection, Particle, None),
	// estOutlineCounterClipPlane
	SCIE(OutlineCounter, true, false, false, Depth, Component, Projection, Particle, None),
	
	
	
	// stereo
	
	// estStereoComponentGeometry
	SCIE(Geometry, false, false, true, Geometry, Component, Projection, Particle, None),
	// estStereoComponentDepth
	SCIE(Depth, false, false, true, Depth, Component, Projection, Particle, Larger),
	// estStereoComponentDepthClipPlane
	SCIE(Depth, true, false, true, Depth, Component, Projection, Particle, Larger),
	// estStereoComponentDepthReversed
	SCIE(Depth, false, false, true, Depth, Component, Projection, Particle, Smaller),
	// estStereoComponentDepthClipPlaneReversed
	SCIE(Depth, true, false, true, Depth, Component, Projection, Particle, Smaller),
	// estStereoComponentCounter
	SCIE(Counter, false, false, true, Depth, Component, Projection, Particle, None),
	// estStereoComponentCounterClipPlane
	SCIE(Counter, true, false, true, Depth, Component, Projection, Particle, None),
	
	
	
	// Billboard
	// estStereoBillboardGeometry
	SCIE(Geometry, false, true, true, Geometry, Billboard, Projection, Particle, None),
	// estStereoBillboardDepth
	SCIE(Depth, false, true, true, Depth, Billboard, Projection, Particle, Larger),
	// estStereoBillboardDepthClipPlane
	SCIE(Depth, true, true, true, Depth, Billboard, Projection, Particle, Larger),
	// estStereoBillboardDepthReversed
	SCIE(Depth, false, true, true, Depth, Billboard, Projection, Particle, Smaller),
	// estStereoBillboardDepthClipPlaneReversed
	SCIE(Depth, true, true, true, Depth, Billboard, Projection, Particle, Smaller),
	// estStereoBillboardCounter
	SCIE(Counter, false, true, true, Depth, Billboard, Projection, Particle, None),
	// estStereoBillboardCounterClipPlane
	SCIE(Counter, true, true, true, Depth, Billboard, Projection, Particle, None),
	
	
	// Decal
	
	// estStereoDecalGeometry
	SCIE(Geometry, false, false, true, Geometry, Decal, Projection, Particle, None),
	
	
	
	// Prop Field
	//   Depth Node edmDepth
	//     depth testStereo mode is used only for !pSolid. For pSolid edtmNone is used
	
	// estStereoPropFieldGeometry
	SCIE(Geometry, false, false, true, Geometry, PropField, Projection, Particle, None),
	// estStereoPropFieldImposterGeometry
	SCIE(Geometry, false, true, true, Geometry, PropField, Projection, Particle, None),
	// estStereoPropFieldDepth
	SCIE(Depth, false, false, true, Depth, PropField, Projection, Particle, Larger),
	// estStereoPropFieldImposterDepth
	SCIE(Depth, false, true, true, Depth, PropField, Projection, Particle, Larger),
	// estStereoPropFieldDepthClipPlane
	SCIE(Depth, true, false, true, Depth, PropField, Projection, Particle, Larger),
	// estStereoPropFieldImposterDepthClipPlane
	SCIE(Depth, true, true, true, Depth, PropField, Projection, Particle, Larger),
	// estStereoPropFieldDepthReversed
	SCIE(Depth, false, false, true, Depth, PropField, Projection, Particle, Smaller),
	// estStereoPropFieldImposterDepthReversed
	SCIE(Depth, false, true, true, Depth, PropField, Projection, Particle, Smaller),
	// estStereoPropFieldDepthClipPlaneReversed
	SCIE(Depth, true, false, true, Depth, PropField, Projection, Particle, Smaller),
	// estStereoPropFieldImposterDepthClipPlaneReversed
	SCIE(Depth, true, true, true, Depth, PropField, Projection, Particle, Smaller),
	// estStereoPropFieldCounter
	SCIE(Counter, false, false, true, Depth, PropField, Projection, Particle, None),
	// estStereoPropFieldCounterClipPlane
	SCIE(Counter, true, false, true, Depth, PropField, Projection, Particle, None),
	
	
	// Height Terrain
	//   Depth Node edmDepth
	//     depth testStereo mode is used only for !pSolid. For pSolid edtmNone is used
	
	// estStereoHeightMapGeometry
	SCIE(Geometry, false, false, true, Geometry, HeightMap, Projection, Particle, None),
	// estStereoHeightMapDepth
	SCIE(Depth, false, false, true, Depth, HeightMap, Projection, Particle, Larger),
	// estStereoHeightMapDepthClipPlane
	SCIE(Depth, true, false, true, Depth, HeightMap, Projection, Particle, Larger),
	// estStereoHeightMapDepthReversed
	SCIE(Depth, false, false, true, Depth, HeightMap, Projection, Particle, Smaller),
	// estStereoHeightMapDepthClipPlaneReversed
	SCIE(Depth, true, false, true, Depth, HeightMap, Projection, Particle, Smaller),
	// estStereoHeightMapCounter
	SCIE(Counter, false, false, true, Depth, HeightMap, Projection, Particle, None),
	// estStereoHeightMapCounterClipPlane
	SCIE(Counter, true, false, true, Depth, HeightMap, Projection, Particle, None),
	
	
	// Particle Emitter
	
	// estStereoParticleGeometry
	SCIE(Geometry, false, false, true, Geometry, Particle, Projection, Particle, None),
	// estStereoParticleGeometryDepthtestStereo
	SCIE(Geometry, false, false, true, Geometry, Particle, Projection, Particle, Larger),
	// estStereoParticleDepth
	SCIE(Depth, false, false, true, Depth, Particle, Projection, Particle, Larger),
	// estStereoParticleDepthClipPlane
	SCIE(Depth, true, false, true, Depth, Particle, Projection, Particle, Larger),
	// estStereoParticleDepthReversed
	SCIE(Depth, false, false, true, Depth, Particle, Projection, Particle, Smaller),
	// estStereoParticleDepthClipPlaneReversed
	SCIE(Depth, true, false, true, Depth, Particle, Projection, Particle, Smaller),
	// estStereoParticleCounter
	SCIE(Counter, false, false, true, Depth, Particle, Projection, Particle, None),
	// estStereoParticleCounterClipPlane
	SCIE(Counter, true, false, true, Depth, Particle, Projection, Particle, None),
	// estStereoParticleRibbonGeometry
	SCIE(Geometry, false, false, true, Geometry, Particle, Projection, Ribbon, None),
	// estStereoParticleRibbonGeometryDepthtestStereo
	SCIE(Geometry, false, false, true, Geometry, Particle, Projection, Ribbon, Larger),
	// estStereoParticleRibbonDepth
	SCIE(Depth, false, false, true, Depth, Particle, Projection, Ribbon, Larger),
	// estStereoParticleRibbonDepthClipPlane
	SCIE(Depth, true, false, true, Depth, Particle, Projection, Ribbon, Larger),
	// estStereoParticleRibbonDepthReversed
	SCIE(Depth, false, false, true, Depth, Particle, Projection, Ribbon, Smaller),
	// estStereoParticleRibbonDepthClipPlaneReversed
	SCIE(Depth, true, false, true, Depth, Particle, Projection, Ribbon, Smaller),
	// estStereoParticleRibbonCounter
	SCIE(Counter, false, false, true, Depth, Particle, Projection, Ribbon, None),
	// estStereoParticleRibbonCounterClipPlane
	SCIE(Counter, true, false, true, Depth, Particle, Projection, Ribbon, None),
	// estStereoParticleBeamGeometry
	SCIE(Geometry, false, false, true, Geometry, Particle, Projection, Beam, None),
	// estStereoParticleBeamGeometryDepthtestStereo
	SCIE(Geometry, false, false, true, Geometry, Particle, Projection, Beam, Larger),
	// estStereoParticleBeamDepth
	SCIE(Depth, false, false, true, Depth, Particle, Projection, Beam, Larger),
	// estStereoParticleBeamDepthClipPlane
	SCIE(Depth, true, false, true, Depth, Particle, Projection, Beam, Larger),
	// estStereoParticleBeamDepthReversed
	SCIE(Depth, false, false, true, Depth, Particle, Projection, Beam, Smaller),
	// estStereoParticleBeamDepthClipPlaneReversed
	SCIE(Depth, true, false, true, Depth, Particle, Projection, Beam, Smaller),
	// estStereoParticleBeamCounter
	SCIE(Counter, false, false, true, Depth, Particle, Projection, Beam, None),
	// estStereoParticleBeamCounterClipPlane
	SCIE(Counter, true, false, true, Depth, Particle, Projection, Beam, None),
	
	
	
	// Outline
	// estStereoOutlineGeometry
	SCIE(OutlineGeometry, false, false, true, Geometry, Component, Projection, Particle, None),
	// estStereoOutlineDepth
	SCIE(OutlineDepth, false, false, true, Depth, Component, Projection, Particle, Larger),
	// estStereoOutlineDepthClipPlane
	SCIE(OutlineDepth, true, false, true, Depth, Component, Projection, Particle, Larger),
	// estStereoOutlineDepthReversed
	SCIE(OutlineDepth, false, false, true, Depth, Component, Projection, Particle, Smaller),
	// estStereoOutlineDepthClipPlaneReversed
	SCIE(OutlineDepth, true, false, true, Depth, Component, Projection, Particle, Smaller),
	// estStereoOutlineCounter
	SCIE(OutlineCounter, false, false, true, Depth, Component, Projection, Particle, None),
	// estStereoOutlineCounterClipPlane
	SCIE(OutlineCounter, true, false, true, Depth, Component, Projection, Particle, None)
};



// #ifdef OS_ANDROID
// #undef ENABLE_CACHE_LOGGING
// #define ENABLE_CACHE_LOGGING true
// #endif



// Class deoglSkinTexture
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinTexture::deoglSkinTexture( deoglRenderThread &renderThread, deoglRSkin &skin, const deSkinTexture &texture ) :
pRenderThread( renderThread ),
pName( texture.GetName() ),
pSharedSPBElement( NULL )
{
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	
	int i;
	
	pHasSolidity = false;
	pHasZeroSolidity = false;
	pHasTransparency = false;
	pHasEmissivity = false;
	
	pSolid = true;
	pHasHoles = false;
	pQuickTransp = false;
	pDynamicChannels = false;
	pCalculatedProperties = false;
	pRenderableChannels = false;
	pRenderableMaterialProperties = false;
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		pChannels[ i ] = NULL;
	}
	
	pAbsorption.Set( 0.0f, 0.0f, 0.0f );
	pAbsorptionHalfIntensityDistance = 1.25f;
	pAbsorptionRange = 0.0f;
	pAmbientOcclusion = 1.0f;
	pAmbientOcclusionSolidityMultiplier = 1.0f;
	pColor.Set( 0.0f, 0.0f, 0.0f );
	pColorGamma = 2.2f;
	pColorTint.Set( 1.0f, 1.0f, 1.0f );
	pColorTintMask = 1.0f;
	pColorSolidityMultiplier = 1.0f;
	pColorOmnidirCube.Set( 0.0f, 0.0f, 0.0f );
	pColorOmnidirEquirect.Set( 0.0f, 0.0f, 0.0f );
	pEmissivityTint.Set( 1.0f, 1.0f, 1.0f );
	pEmissivityIntensity = 0.0f;
	pEmissivity.SetZero();
	pEmissivityCameraAdapted = false;
	pEnvironmentRoomEmissivityTint.Set( 1.0f, 1.0f, 1.0f );
	pEnvironmentRoomEmissivityIntensity = 0.0f;
	pEnvironmentRoomSize.Set( 1.0f, 1.0f );
	pEnvironmentRoomOffset.Set( 0.0f, 0.0f, 0.0f );
	pHeightScale = 0.1f;
	pHeightOffset = 0.0f;
	pNormal.Set( 0.5f, 0.5f, 1.0f );
	pNormalStrength = 1.0f;
	pNormalSolidityMultiplier = 1.0f;
	pReflectivityMultiplier = 1.0f;
	pReflectivity.Set( 0.0f, 0.0f, 0.0f );
	pReflectivitySolidityMultiplier = 1.0f;
	pRefractDistortStrength = 0.0f;
	pRoughness = 1.0f;
	pRoughnessGamma = 1.0f;
	pRoughnessRemapLower = 0.0f;
	pRoughnessRemapUpper = 1.0f;
	pRoughnessSolidityMultiplier = 1.0f;
	pThickness = 0.0f;
	pTransparency = 1.0f;
	pTransparencyMultiplier = 1.0f;
	pSolidity = 1.0f;
	pSolidityMultiplier = 1.0f;
	pSolidityMasked = false;
	pSolidityFilterPriority = 0.5f;
	
	pReflected = true;
	pShadeless = false;
	pShadowNone = false;
	pShadowImportance = 10;
	pHintNoCompression = false;
	pHintLightBlocker = false;
	pTexCoordClamp = false;
	pTexCoordOffset.Set( 0.0f, 0.0f );
	pTexCoordScale.Set( 1.0f, 1.0f );
	pTexCoordRotate = 0.0f;
	pRenderTaskFilters = 0;
	
	pVariationU = false;
	pVariationV = false;
	
	pParticleSheetCount = 1;
	
	pOutlineColor.Set( 0.0f, 0.0f, 0.0f );
	pOutlineColorTint.Set( 1.0f, 1.0f, 1.0f );
	pOutlineThickness = 0.0f;
	pOutlineThicknessScreen = false;
	pOutlineSolidity = 1.0f;
	pOutlineEmissivity.Set( 0.0f, 0.0f, 0.0f );
	pOutlineEmissivityTint.Set( 1.0f, 1.0f, 1.0f );
	pOutlineEmissivityIntensity = 0.0f;
	pHasOutline = false;
	pIsOutlineSolid = true;
	pIsOutlineEmissive = false;
	
	pRimEmissivity.Set( 0.0f, 0.0f, 0.0f );
	pRimEmissivityTint.Set( 1.0f, 1.0f, 1.0f );
	pRimEmissivityIntensity = 0.0f;
	pRimAngle = 0.0f;
	pRimExponent = 1.0f;
	
	pNonPbrAlbedo.Set( 0.0f, 0.0f, 0.0f );
	pNonPbrMetalness = 0.0f;
	
	pMirror = false;
	pRendered = false;
	pReflects = false;
	
	try{
		pPrepareChannels( skin, texture );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	// store texture features required to do educated choices later on
	if( pHasTransparency ){
		pSolid = false;
		pHasHoles = false;
		
	}else if( pHasSolidity ){
		if( pHasZeroSolidity ){
			// special case. the entire material is invisible. in general this would mean
			// the material has not to be rendered at all but if emissivity is present
			// a faster rendering can be done for this material only adding light to the
			// scene. this is a special case not yet looked at so consider it masked solid
			pSolid = false;
			pHasHoles = false;
			
		}else if( pSolidityMasked ){
			if( pHasEmissivity ){
				// if we have emissivity we have a problem. non-solid fragmenents will be
				// discarded. if these have now emissivity larger than 0 the emissivity
				// would not be rendered. we need to make this texture non-solid
				pSolid = false;
				pHasHoles = false;
				
			}else{
				pSolid = true;
				pHasHoles = true;
			}
			
		}else{
			pSolid = false;
			pHasHoles = false;
		}
	}
	
	pUpdateRenderTaskFilters();
}

deoglSkinTexture::~deoglSkinTexture(){
	pCleanUp();
}



// Management
///////////////

void deoglSkinTexture::BuildChannels( deoglRSkin &skin, const deSkinTexture &texture ){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	#ifdef DO_PERFORMANCE_TIMING
	decTimer timer;
	#endif
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	int i;
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( ! pChannels[ i ] ){
			continue;
		}
		if( pChannels[ i ]->GetIsCached() ){
			continue; // loaded from cache
		}
		
		if( pChannels[ i ]->GetImage() && pChannels[ i ]->GetImage()->GetSkinUse() ){
			// already build by somebody else.
			// 
			// building channels runs potentially parallel to the render thread running delayed
			// operations to init skins. during this time skin-use parameter can be changed to true.
			// it is thus possible skin-use has been false while the skin-texture has been
			// constructed but not while BuildChannels() is run. this results in the pixel buffer
			// to be present but not initialized. then pWriteCached() would write an
			// non-initialized pixel buffer which breaks future loading. to be sure nothing goes
			// wrong the cache-id has to be cleared and the pixel buffer dropped. then
			// pWriteCached() will not pick it up and delayed operations will not try to init the
			// skin using non-initialized data
			pChannels[ i ]->SetCacheID( "" );
			pChannels[ i ]->SetCanBeCached( false );
			pChannels[ i ]->SetPixelBufferMipMap( NULL );
			continue;
		}
		
		if( enableCacheLogging ){
			pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Not cached, building"
				" (skin='%s' texture='%s' channel='%s' id='%s')",
				skin.GetFilename().GetString(), pName.GetString(),
				deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
				pChannels[ i ]->GetCacheID().GetString() );
		}
		
		pChannels[ i ]->BuildChannel( texture );
		#ifdef DO_PERFORMANCE_TIMING
		pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s) Channel(%s): Build %dms",
			skin.GetFilename().GetString(), pName.GetString(),
			deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
			( int )( timer.GetElapsedTime() * 1e3f ) );
		#endif
	}
	
	pCreateMipMaps();
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s): MipMaps %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		( int )( timer.GetElapsedTime() * 1e3f ) );
	#endif
	
	pCompressTextures( skin, texture );
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s): Compress %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		( int )( timer.GetElapsedTime() * 1e3f ) );
	#endif
	
	// write channels to cache suitable for caching
	pWriteCached( skin );
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s): SaveCache %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		( int )( timer.GetElapsedTime() * 1e3f ) );
	#endif
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( pChannels[ i ] ){
			pChannels[ i ]->ClearCacheData();
		}
	}
}

void deoglSkinTexture::FinalizeAsyncResLoading(){
	int i;
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( pChannels[ i ] ){
			pChannels[ i ]->FinalizeAsyncResLoading();
		}
	}
}



void deoglSkinTexture::SetHasSolidity( bool hasSolidity ){
	pHasSolidity = hasSolidity;
}

void deoglSkinTexture::SetHasZeroSolidity( bool hasZeroSolidity ){
	pHasZeroSolidity = hasZeroSolidity;
}

void deoglSkinTexture::SetHasTransparency( bool hasTransparency ){
	pHasTransparency = hasTransparency;
}

void deoglSkinTexture::SetHasEmissivity( bool hasEmissivity ){
	pHasEmissivity = hasEmissivity;
}



void deoglSkinTexture::SetSolidityMasked( bool solidityMasked ){
	pSolidityMasked = solidityMasked;
}

void deoglSkinTexture::SetSolidityFilterPriority( float solidityFilterPriority ){
	pSolidityFilterPriority = decMath::clamp( solidityFilterPriority, 0.0f, 1.0f );
}

void deoglSkinTexture::SetSolid( bool solid ){
	pSolid = solid;
}

void deoglSkinTexture::SetHasHoles( bool hasHoles ){
	pHasHoles = hasHoles;
}

void deoglSkinTexture::SetQuickTransparency( bool quickTransp ){
	pQuickTransp = quickTransp;
}

void deoglSkinTexture::SetDynamicChannels( bool dynamicChannels ){
	pDynamicChannels = dynamicChannels;
}

void deoglSkinTexture::SetCalculatedProperties( bool calculatedProperties ){
	pCalculatedProperties = calculatedProperties;
}

void deoglSkinTexture::SetRenderableChannels( bool renderableChannels ){
	pRenderableChannels = renderableChannels;
}

void deoglSkinTexture::SetRenderableMaterialProperties( bool renderableMaterialProperties ){
	pRenderableMaterialProperties = renderableMaterialProperties;
}



deoglSkinChannel *deoglSkinTexture::GetChannelAt( deoglSkinChannel::eChannelTypes type ) const{
	return pChannels[ type ];
}

bool deoglSkinTexture::IsChannelEnabled( deoglSkinChannel::eChannelTypes type ) const{
	return pChannels[ type ] != NULL;
}

deoglSkinShader *deoglSkinTexture::GetShaderFor( eShaderTypes shaderType ) const{
	return pShaders[ shaderType ];
}

void deoglSkinTexture::PrepareShaders(){
	int i;
	for( i=0; i<ShaderTypeCount; i++ ){
		if( pShaders[ i ] ){
			continue;
		}
		
		deoglSkinShaderConfig config;
		if( ! GetShaderConfigFor( ( eShaderTypes )i, config ) ){
			continue;
		}
		
		pShaders[ i ] = pRenderThread.GetShader().GetSkinShaderManager().GetShaderWith( config );
		pShaders[ i ]->PrepareShader();
	}
}

bool deoglSkinTexture::GetShaderConfigFor( eShaderTypes shaderType, deoglSkinShaderConfig &config ) const{
	const deoglDeferredRendering &defren = pRenderThread.GetDeferredRendering();
	const sShaderConfigInfo &shaderConfigInfo = vShaderConfigInfo[ shaderType ];
	const bool useEquiEnvMap = pRenderThread.GetRenderers().GetReflection().GetUseEquiEnvMap();
	bool hasChanTex[ deoglSkinChannel::CHANNEL_COUNT ];
	const bool isDecal = ( shaderConfigInfo.geometryMode == deoglSkinShaderConfig::egmDecal );
	const bool isParticle = ( shaderConfigInfo.geometryMode == deoglSkinShaderConfig::egmParticle );
	//const bool isPropField = ( shaderConfigInfo.geometryMode == deoglSkinShaderConfig::egmPropField );
	const bool realTranspParticle = GetRenderThread().GetChoices().GetRealTransparentParticles();
	const bool luminanceOnly = shaderType == estComponentLuminance || shaderType == estHeightMapLuminance;
	const bool giMaterial = shaderType == estComponentGIMaterial;
	
	int i;
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		hasChanTex[ i ] = ( pChannels[ i ] &&
			( pChannels[ i ]->GetImage()
			|| pChannels[ i ]->GetTexture()
			|| pChannels[ i ]->GetCubeMap()
			|| pChannels[ i ]->GetArrayTexture()
			|| pChannels[ i ]->GetRenderable() != -1 ) );
	}
	
	config.Reset();
	
	switch( shaderConfigInfo.geometryMode ){
	case deoglSkinShaderConfig::egmComponent:
	case deoglSkinShaderConfig::egmBillboard:
	case deoglSkinShaderConfig::egmDecal:
		if( ! config.GetGIMaterial() ){
			config.SetSharedSPB( true );
		}
		break;
		
	case deoglSkinShaderConfig::egmHeightMap:
	case deoglSkinShaderConfig::egmParticle:
	case deoglSkinShaderConfig::egmPropField:
	default:
		break;
	}
	
	config.SetShaderMode( shaderConfigInfo.shaderMode );
	config.SetGeometryMode( shaderConfigInfo.geometryMode );
	config.SetDepthMode( shaderConfigInfo.depthMode );
	config.SetParticleMode( shaderConfigInfo.particleMode );
	config.SetLuminanceOnly( luminanceOnly );
	config.SetGIMaterial( giMaterial );
	config.SetBillboard( shaderConfigInfo.billboard );
	config.SetGSRenderStereo( shaderConfigInfo.stereo );
	
	config.SetMaterialNormalModeEnc( deoglSkinShaderConfig::emnmFloat );
	config.SetInverseDepth( defren.GetUseInverseDepth() );
	if( ! luminanceOnly && ! giMaterial ){
		config.SetMaskedSolidity( pSolidityMasked || pHasZeroSolidity );
	}
	config.SetVariations( pVariationU || pVariationV );
	
	config.SetUseNormalRoughnessCorrection( pMaterialProperties[ empNormal ].GetRenderable() == -1 );
		// if normal has renderable we can no more control the alpha-channel of the normal which
		// is used for roughness correction. by disabling it things are not optimal but especially
		// not wrong. could be made better by tracking at run-time if the renderable is actually
		// used or not
	
	switch( shaderConfigInfo.type ){
	case esctGeometry:
		config.SetFadeOutRange( defren.GetUseFadeOutRange() );
		config.SetMaterialNormalModeDec( deoglSkinShaderConfig::emnmIntBasic );
		config.SetDepthTestMode( shaderConfigInfo.depthTestMode );
		config.SetAmbientLightProbe( isParticle && ! realTranspParticle );
		//config.SetSkinReflections( hasChanTex[ deoglSkinChannel::ectEnvironmentMap ] || ! pSolid || isParticle );
		
		if( giMaterial ){
			config.SetTextureColor( hasChanTex[ deoglSkinChannel::ectColor ] );
			config.SetTextureColorTintMask( hasChanTex[ deoglSkinChannel::ectColorTintMask ] );
			config.SetTextureReflectivity( hasChanTex[ deoglSkinChannel::ectReflectivity ] );
			config.SetTextureRoughness( hasChanTex[ deoglSkinChannel::ectRoughness ] );
			config.SetTextureSolidity( hasChanTex[ deoglSkinChannel::ectSolidity ] );
			config.SetTextureEmissivity( hasChanTex[ deoglSkinChannel::ectEmissivity ] );
			config.SetTextureEnvRoom( hasChanTex[ deoglSkinChannel::ectEnvironmentRoom ] );
			config.SetTextureEnvRoomMask( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomMask ] );
			config.SetTextureEnvRoomEmissivity( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomEmissivity ] );
			config.SetTextureNonPbrAlbedo( hasChanTex[ deoglSkinChannel::ectNonPbrAlbedo ] );
			config.SetTextureNonPbrMetalness( hasChanTex[ deoglSkinChannel::ectNonPbrMetalness ] );
			/*
			config.SetDynamicColorTint( pMaterialProperties[ empColorTint ].IsDynamic() );
			config.SetDynamicColorGamma( pMaterialProperties[ empColorGamma ].IsDynamic() );
			config.SetDynamicHeightRemap(
				pMaterialProperties[ empHeightScale ].IsDynamic()
				|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
			config.SetDynamicRoughnessRemap(
				pMaterialProperties[ empRoughnessRemapLower ].IsDynamic()
				|| pMaterialProperties[ empRoughnessRemapUpper ].IsDynamic() );
			config.SetDynamicRoughnessGamma( pMaterialProperties[ empRoughnessGamma ].IsDynamic() );
			config.SetDynamicReflectivityMultiplier( pMaterialProperties[ empReflectivityMultiplier ].IsDynamic() );
			config.SetDynamicEmissivityTint( pMaterialProperties[ empEmissivityTint ].IsDynamic() );
			config.SetDynamicEmissivityIntensity( pMaterialProperties[ empEmissivityIntensity ].IsDynamic() );
			config.SetDynamicEnvRoomSize( pMaterialProperties[ empEnvironmentRoomSize ].IsDynamic() );
			config.SetDynamicEnvRoomOffset( pMaterialProperties[ empEnvironmentRoomOffset ].IsDynamic() );
			config.SetDynamicEnvRoomEmissivityTint( pMaterialProperties[ empEnvironmentRoomEmissivityTint ].IsDynamic() );
			config.SetDynamicEnvRoomEmissivityIntensity( pMaterialProperties[ empEnvironmentRoomEmissivityIntensity ].IsDynamic() );
			config.SetDynamicVariation(
				pMaterialProperties[ empVariationU ].IsDynamic()
				|| pMaterialProperties[ empVariationV ].IsDynamic() );
			*/
			
		}else if( luminanceOnly ){
			config.SetTextureHeight( hasChanTex[ deoglSkinChannel::ectHeight ] );
			config.SetTextureEmissivity( hasChanTex[ deoglSkinChannel::ectEmissivity ] );
			config.SetTextureEnvRoomEmissivity( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomEmissivity ] );
			
			if( ! isParticle ){
				config.SetDynamicHeightRemap(
					pMaterialProperties[ empHeightScale ].IsDynamic()
					|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
				config.SetDynamicEmissivityTint(
					pMaterialProperties[ empEmissivityTint ].IsDynamic() );
				config.SetDynamicEmissivityIntensity(
					pMaterialProperties[ empEmissivityIntensity ].IsDynamic() );
				config.SetDynamicEnvRoomEmissivityTint(
					pMaterialProperties[ empEnvironmentRoomEmissivityTint ].IsDynamic() );
				config.SetDynamicEnvRoomEmissivityIntensity(
					pMaterialProperties[ empEnvironmentRoomEmissivityIntensity ].IsDynamic() );
			}
			
		}else{
			config.SetSkinReflections( hasChanTex[ deoglSkinChannel::ectEnvironmentMap ]
				|| ( isParticle && ! realTranspParticle ) );
			
			config.SetTextureColor( hasChanTex[ deoglSkinChannel::ectColor ] );
			config.SetTextureColorTintMask( hasChanTex[ deoglSkinChannel::ectColorTintMask ] );
			config.SetTextureTransparency( hasChanTex[ deoglSkinChannel::ectTransparency ] );
			config.SetTextureSolidity( hasChanTex[ deoglSkinChannel::ectSolidity ] );
			config.SetTextureNormal( hasChanTex[ deoglSkinChannel::ectNormal ] );
			config.SetTextureHeight( hasChanTex[ deoglSkinChannel::ectHeight ] );
			config.SetTextureReflectivity( hasChanTex[ deoglSkinChannel::ectReflectivity ] );
			config.SetTextureRoughness( hasChanTex[ deoglSkinChannel::ectRoughness ] );
			config.SetTextureEmissivity( hasChanTex[ deoglSkinChannel::ectEmissivity ] );
			config.SetTextureAO( hasChanTex[ deoglSkinChannel::ectAO ] );
			config.SetTextureRimEmissivity( hasChanTex[ deoglSkinChannel::ectRimEmissivity ] );
			
			if( deoglSkinShader::REFLECTION_TEST_MODE == 0 ){
				config.SetTextureEnvMap( pReflects || ( isParticle && ! realTranspParticle ) );
				
			}else if( deoglSkinShader::REFLECTION_TEST_MODE == 1 ){
				// !pSolid only until transparency works properly with the separate environment map pass
				config.SetTextureEnvMap( hasChanTex[ deoglSkinChannel::ectEnvironmentMap ]
					|| ! pSolid || ( isParticle && ! realTranspParticle ) );
				
			}else{
				//config.SetTextureEnvMap( isParticle && ! realTranspParticle );
				config.SetTextureEnvMap( true );
			}
			
			config.SetTextureEnvMapEqui( ! hasChanTex[ deoglSkinChannel::ectEnvironmentMap ] && useEquiEnvMap );
			config.SetTextureRenderColor( ! pSolid && ! isDecal && ! ( isParticle && ! realTranspParticle ) );
					//&& ! pHasHoles  // problems with transparent
			//config.SetTextureRenderColor( ! pSolid && ! isDecal ); // problems with emssivity-only
			config.SetTextureRefractionDistort( config.GetTextureRenderColor()
				&& hasChanTex[ deoglSkinChannel::ectRefractDistort ] );
			config.SetTextureEnvRoom( hasChanTex[ deoglSkinChannel::ectEnvironmentRoom ] );
			config.SetTextureEnvRoomMask( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomMask ] );
			config.SetTextureEnvRoomEmissivity( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomEmissivity ] );
			config.SetTextureAbsorption( hasChanTex[ deoglSkinChannel::ectAbsorption ] );
			
			config.SetTextureNonPbrAlbedo( hasChanTex[ deoglSkinChannel::ectNonPbrAlbedo ] );
			config.SetTextureNonPbrMetalness( hasChanTex[ deoglSkinChannel::ectNonPbrMetalness ] );
			
			if( ! isParticle ){
				config.SetDynamicColorTint(
					pMaterialProperties[ empColorTint ].IsDynamic() );
				config.SetDynamicColorGamma(
					pMaterialProperties[ empColorGamma ].IsDynamic() );
				config.SetDynamicColorSolidityMultiplier(
					pMaterialProperties[ empColorSolidityMultiplier ].IsDynamic() );
				config.SetDynamicAmbientOcclusionSolidityMultiplier(
					pMaterialProperties[ empAmbientOcclusionSolidityMultiplier ].IsDynamic() );
				config.SetDynamicTransparencyMultiplier(
					pMaterialProperties[ empTransparencyMultiplier ].IsDynamic() );
				config.SetDynamicSolidityMultiplier(
					pMaterialProperties[ empSolidityMultiplier ].IsDynamic() );
				config.SetDynamicHeightRemap(
					pMaterialProperties[ empHeightScale ].IsDynamic()
					|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
				config.SetDynamicNormalStrength(
					pMaterialProperties[ empNormalStrength ].IsDynamic() );
				config.SetDynamicNormalSolidityMultiplier(
					pMaterialProperties[ empNormalSolidityMultiplier ].IsDynamic() );
				config.SetDynamicRoughnessRemap(
					pMaterialProperties[ empRoughnessRemapLower ].IsDynamic()
					|| pMaterialProperties[ empRoughnessRemapUpper ].IsDynamic() );
				config.SetDynamicRoughnessGamma(
					pMaterialProperties[ empRoughnessGamma ].IsDynamic() );
				config.SetDynamicRoughnessSolidityMultiplier(
					pMaterialProperties[ empRoughnessSolidityMultiplier ].IsDynamic() );
				config.SetDynamicRefractionDistortStrength(
					pMaterialProperties[ empRefractDistortStrength ].IsDynamic() );
				config.SetDynamicReflectivitySolidityMultiplier(
					pMaterialProperties[ empReflectivitySolidityMultiplier ].IsDynamic() );
				config.SetDynamicReflectivityMultiplier(
					pMaterialProperties[ empReflectivityMultiplier ].IsDynamic() );
				config.SetDynamicEmissivityTint(
					pMaterialProperties[ empEmissivityTint ].IsDynamic() );
				config.SetDynamicEmissivityIntensity(
					pMaterialProperties[ empEmissivityIntensity ].IsDynamic() );
				config.SetDynamicEnvRoomSize(
					pMaterialProperties[ empEnvironmentRoomSize ].IsDynamic() );
				config.SetDynamicEnvRoomOffset(
					pMaterialProperties[ empEnvironmentRoomOffset ].IsDynamic() );
				config.SetDynamicEnvRoomEmissivityTint(
					pMaterialProperties[ empEnvironmentRoomEmissivityTint ].IsDynamic() );
				config.SetDynamicEnvRoomEmissivityIntensity(
					pMaterialProperties[ empEnvironmentRoomEmissivityIntensity ].IsDynamic() );
				config.SetDynamicThickness(
					pMaterialProperties[ empThickness ].IsDynamic() );
				config.SetDynamicAbsorption(
					pMaterialProperties[ empAbsorptionRange ].IsDynamic()
					|| pMaterialProperties[ empAbsorptionHalfIntensityDistance ].IsDynamic() );
				config.SetDynamicVariation(
					pMaterialProperties[ empVariationU ].IsDynamic()
					|| pMaterialProperties[ empVariationV ].IsDynamic() );
				config.SetDynamicRimEmissivityTint(
					pMaterialProperties[ empRimEmissivityTint ].IsDynamic() );
				config.SetDynamicRimEmissivityIntensity(
					pMaterialProperties[ empRimEmissivityIntensity ].IsDynamic() );
				config.SetDynamicRimAngle(
					pMaterialProperties[ empRimAngle ].IsDynamic() );
				config.SetDynamicRimExponent(
					pMaterialProperties[ empRimExponent ].IsDynamic() );
				
				// required to be compatible with outline shaders if used to build parameter block
				config.SetDynamicOutlineColor(
					pMaterialProperties[ empOutlineColor ].IsDynamic() );
				config.SetDynamicOutlineColorTint(
					pMaterialProperties[ empOutlineColorTint ].IsDynamic() );
				config.SetDynamicOutlineThickness(
					pMaterialProperties[ empOutlineThickness ].IsDynamic() );
				config.SetDynamicOutlineSolidity(
					pMaterialProperties[ empOutlineSolidity ].IsDynamic() );
				config.SetDynamicOutlineEmissivity(
					pMaterialProperties[ empOutlineEmissivity ].IsDynamic()
					|| pMaterialProperties[ empOutlineEmissivityIntensity ].IsDynamic() );
				config.SetDynamicOutlineEmissivityTint(
					pMaterialProperties[ empOutlineEmissivityTint ].IsDynamic() );
			}
		}
		
		if( config.GetTextureHeight() ){ // temporary
			//config.SetTessellationMode( deoglSkinShaderConfig::etmLinear );
		}
		break;
		
	case esctDepth:
		config.SetFadeOutRange( defren.GetUseFadeOutRange() );
		config.SetClipPlane( shaderConfigInfo.clipPlane );
		
		if( pSolid ){
			config.SetDepthTestMode( deoglSkinShaderConfig::edtmNone );
			
		}else{
			config.SetDepthTestMode( shaderConfigInfo.depthTestMode );
		}
		
		//config.SetTextureColorTransparency( pSolid && pHasHoles && hasChanTex[ deoglSkinChannel::ectColor ] );
		config.SetTextureSolidity( pSolid && pHasHoles && hasChanTex[ deoglSkinChannel::ectSolidity ] );
		
		config.SetTextureHeight( hasChanTex[ deoglSkinChannel::ectHeight ] );
		config.SetDynamicHeightRemap(
			pMaterialProperties[ empHeightScale ].IsDynamic()
			|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
		
		if( config.GetTextureHeight() ){ // temporary
			//config.SetTessellationMode( deoglSkinShaderConfig::etmLinear );
		}
		
		if( pHasEmissivity ){
			// emissivity textures are required to avoid discarding non-solid fragments
			config.SetTextureSolidity( hasChanTex[ deoglSkinChannel::ectSolidity ] );
			
			config.SetTextureEmissivity( hasChanTex[ deoglSkinChannel::ectEmissivity ] );
			config.SetTextureEnvRoomEmissivity( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomEmissivity ] );
			config.SetTextureRimEmissivity( hasChanTex[ deoglSkinChannel::ectRimEmissivity ] );
			if( ! isParticle ){
				config.SetDynamicEmissivityIntensity(
					pMaterialProperties[ empEmissivityIntensity ].IsDynamic() );
				config.SetDynamicEnvRoomEmissivityIntensity(
					pMaterialProperties[ empEnvironmentRoomEmissivityIntensity ].IsDynamic() );
				config.SetDynamicRimEmissivityIntensity(
					pMaterialProperties[ empRimEmissivityIntensity ].IsDynamic() );
			}
		}
		
		// required to be compatible with outline shaders if used to build parameter block
		config.SetDynamicOutlineThickness(
			pMaterialProperties[ empOutlineThickness ].IsDynamic() );
		config.SetDynamicOutlineSolidity(
			pMaterialProperties[ empOutlineSolidity ].IsDynamic() );
		config.SetDynamicOutlineEmissivity(
			pMaterialProperties[ empOutlineEmissivity ].IsDynamic()
			|| pMaterialProperties[ empOutlineEmissivityIntensity ].IsDynamic() );
		break;
		
	case esctCounter:
		config.SetFadeOutRange( defren.GetUseFadeOutRange() );
		config.SetClipPlane( shaderConfigInfo.clipPlane );
		config.SetOutputConstant( true );
		
		//config.SetTextureColorTransparency( pHasHoles && hasChanTex[ deoglSkinChannel::ectColor ] );
		
		if( pHasEmissivity ){
			// emissivity textures are required to avoid discarding non-solid fragments
			config.SetTextureSolidity( hasChanTex[ deoglSkinChannel::ectSolidity ] );
			
			config.SetTextureEmissivity( hasChanTex[ deoglSkinChannel::ectEmissivity ] );
			config.SetTextureEnvRoomEmissivity( hasChanTex[ deoglSkinChannel::ectEnvironmentRoomEmissivity ] );
			config.SetTextureRimEmissivity( hasChanTex[ deoglSkinChannel::ectRimEmissivity ] );
			if( ! isParticle ){
				config.SetDynamicEmissivityIntensity(
					pMaterialProperties[ empEmissivityIntensity ].IsDynamic() );
				config.SetDynamicEnvRoomEmissivityIntensity(
					pMaterialProperties[ empEnvironmentRoomEmissivityIntensity ].IsDynamic() );
				config.SetDynamicRimEmissivityIntensity(
					pMaterialProperties[ empRimEmissivityIntensity ].IsDynamic() );
			}
		}
		
		break;
		
	case esctShadow:
		config.SetEncodeOutDepth( false );
		config.SetNoZClip( shaderConfigInfo.depthMode == deoglSkinShaderConfig::edmOrthogonal );
		config.SetOutputColor( ! pSolid );
		
		if( shaderConfigInfo.depthMode != deoglSkinShaderConfig::edmProjection ){
			config.SetInverseDepth( false );
		}
		
		config.SetTextureColor( ( ( pSolid && pHasHoles ) || ! pSolid )
			&& hasChanTex[ deoglSkinChannel::ectColor ] );
		
		config.SetTextureSolidity( pSolid && pHasHoles && hasChanTex[ deoglSkinChannel::ectSolidity ] );
		
		config.SetTextureHeight( hasChanTex[ deoglSkinChannel::ectHeight ] );
		config.SetDynamicHeightRemap(
			pMaterialProperties[ empHeightScale ].IsDynamic()
			|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
		
		switch( shaderType ){
		case estComponentShadowOrthogonalCascaded:
			config.SetGSRenderCascaded( true );
			break;
			
		case estComponentShadowDistanceCube:
			config.SetGSRenderCube( true );
			break;
			
		default:
			break;
		}
		
		if( config.GetTextureHeight() ){ // temporary
			//config.SetTessellationMode( deoglSkinShaderConfig::etmLinear );
		}
		break;
		
	case esctEnvMap:
		return false;
		
	case esctOutlineGeometry:
		config.SetOutline( true );
		config.SetOutlineThicknessScreen( pOutlineThicknessScreen );
		config.SetFadeOutRange( defren.GetUseFadeOutRange() );
		config.SetMaterialNormalModeDec( deoglSkinShaderConfig::emnmIntBasic );
		config.SetDepthTestMode( shaderConfigInfo.depthTestMode );
		
		config.SetTextureNormal( hasChanTex[ deoglSkinChannel::ectNormal ] );
		config.SetTextureHeight( hasChanTex[ deoglSkinChannel::ectHeight ] );
		config.SetTextureRenderColor( ! pIsOutlineSolid );
		
		config.SetDynamicHeightRemap(
			pMaterialProperties[ empHeightScale ].IsDynamic()
			|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
		config.SetDynamicNormalStrength(
			pMaterialProperties[ empNormalStrength ].IsDynamic() );
		config.SetDynamicVariation(
			pMaterialProperties[ empVariationU ].IsDynamic()
			|| pMaterialProperties[ empVariationV ].IsDynamic() );
		
		config.SetDynamicOutlineColor(
			pMaterialProperties[ empOutlineColor ].IsDynamic() );
		config.SetDynamicOutlineColorTint(
			pMaterialProperties[ empOutlineColorTint ].IsDynamic() );
		config.SetDynamicOutlineThickness(
			pMaterialProperties[ empOutlineThickness ].IsDynamic() );
		config.SetDynamicOutlineSolidity(
			pMaterialProperties[ empOutlineSolidity ].IsDynamic() );
		config.SetDynamicOutlineEmissivity(
			pMaterialProperties[ empOutlineEmissivity ].IsDynamic()
			|| pMaterialProperties[ empOutlineEmissivityIntensity ].IsDynamic() );
		config.SetDynamicOutlineEmissivityTint(
			pMaterialProperties[ empOutlineEmissivityTint ].IsDynamic() );
		break;
		
	case esctOutlineDepth:
		config.SetOutline( true );
		config.SetOutlineThicknessScreen( pOutlineThicknessScreen );
		config.SetFadeOutRange( defren.GetUseFadeOutRange() );
		config.SetClipPlane( shaderConfigInfo.clipPlane );
		
		if( pIsOutlineSolid ){
			config.SetDepthTestMode( deoglSkinShaderConfig::edtmNone );
			
		}else{
			config.SetDepthTestMode( shaderConfigInfo.depthTestMode );
		}
		
		config.SetTextureHeight( hasChanTex[ deoglSkinChannel::ectHeight ] );
		config.SetDynamicHeightRemap(
			pMaterialProperties[ empHeightScale ].IsDynamic()
			|| pMaterialProperties[ empHeightOffset ].IsDynamic() );
		
		config.SetDynamicOutlineThickness(
			pMaterialProperties[ empOutlineThickness ].IsDynamic() );
		
		// emissivity is required to avoid discarding non-solid fragments
		config.SetDynamicOutlineEmissivity(
			pMaterialProperties[ empOutlineEmissivity ].IsDynamic()
			|| pMaterialProperties[ empOutlineEmissivityIntensity ].IsDynamic() );
		break;
		
	case esctOutlineCounter:
		config.SetOutline( true );
		config.SetOutlineThicknessScreen( pOutlineThicknessScreen );
		config.SetFadeOutRange( defren.GetUseFadeOutRange() );
		config.SetClipPlane( shaderConfigInfo.clipPlane );
		config.SetOutputConstant( true );
		
		config.SetDynamicOutlineThickness(
			pMaterialProperties[ empOutlineThickness ].IsDynamic() );
		
		// emissivity is required to avoid discarding non-solid fragments
		config.SetDynamicOutlineEmissivity(
			pMaterialProperties[ empOutlineEmissivity ].IsDynamic()
			|| pMaterialProperties[ empOutlineEmissivityIntensity ].IsDynamic() );
		break;
		
	default:
		return false;
	}
	
// 	decString debugString;
// 	config.DebugGetConfigString( debugString );
// 	GetRenderThread().GetLogger().LogInfoFormat( "GetShaderConfigFor(%d): %s", shaderType, debugString.GetString() );
	return true;
}

void deoglSkinTexture::PrepareParamBlock(){
	if( ! pSharedSPBElement ){
		if( pRenderThread.GetChoices().GetSharedSPBUseSSBO() ){
			pSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinTextureSSBO ).AddElement();
			
		}else{
			pSharedSPBElement = pRenderThread.GetBufferObject()
				.GetSharedSPBList( deoglRTBufferObject::esspblSkinTextureUBO ).AddElement();
		}
		
		deoglShaderParameterBlock &paramBlock = pSharedSPBElement->MapBuffer();
		try{
			pUpdateParamBlock( paramBlock, pSharedSPBElement->GetIndex() );
			
		}catch( const deException & ){
			paramBlock.UnmapBuffer();
			throw;
		}
		paramBlock.UnmapBuffer();
	}
}



// Material Properties
////////////////////////

void deoglSkinTexture::SetColor( const decColor &color ){
	pColor = color;
}

void deoglSkinTexture::SetColorGamma( float gamma ){
	pColorGamma = gamma;
}

void deoglSkinTexture::SetColorTint( const decColor &tint ){
	pColorTint = tint;
}

void deoglSkinTexture::SetColorTintMask( float mask ){
	pColorTintMask = decMath::clamp( mask, 0.0f, 1.0f );
}

void deoglSkinTexture::SetColorSolidityMultiplier( float multiplier ){
	pColorSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetColorOmnidirCube( const decColor &color ){
	pColorOmnidirCube = color;
}

void deoglSkinTexture::SetColorOmnidirEquirect( const decColor &color ){
	pColorOmnidirEquirect = color;
}

void deoglSkinTexture::SetAmbientOcclusion( float ao ){
	pAmbientOcclusion = ao;
}

void deoglSkinTexture::SetAmbientOcclusionSolidityMultiplier( float multiplier ){
	pAmbientOcclusionSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetTransparency( float transparency ){
	pTransparency = transparency;
}

void deoglSkinTexture::SetTransparencyMultiplier( float multiplier ){
	pTransparencyMultiplier = multiplier;
}

void deoglSkinTexture::SetSolidity( float solidity ){
	pSolidity = solidity;
}

void deoglSkinTexture::SetSolidityMultiplier( float multiplier ){
	pSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetHeightScale( float scale ){
	pHeightScale = scale;
}

void deoglSkinTexture::SetHeightOffset( float offset ){
	pHeightOffset = offset;
}

void deoglSkinTexture::SetNormal( const decColor &normal ){
	pNormal = normal;
}

void deoglSkinTexture::SetNormalStrength( float strength ){
	pNormalStrength = strength;
}

void deoglSkinTexture::SetNormalSolidityMultiplier( float multiplier ){
	pNormalSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetRoughness( float roughness ){
	pRoughness = roughness;
}

void deoglSkinTexture::SetRoughnessRemapLower( float value ){
	pRoughnessRemapLower = value;
}

void deoglSkinTexture::SetRoughnessRemapUpper( float value ){
	pRoughnessRemapUpper = value;
}

void deoglSkinTexture::SetRoughnessGamma( float gamma ){
	pRoughnessGamma = gamma;
}

void deoglSkinTexture::SetRoughnessSolidityMultiplier( float multiplier ){
	pRoughnessSolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetRefractDistortStrength( float strength ){
	pRefractDistortStrength = strength;
}

void deoglSkinTexture::SetReflectivity( const decColor &reflectivity ){
	pReflectivity = reflectivity;
}

void deoglSkinTexture::SetReflectivityMultiplier( float multiplier ){
	pReflectivityMultiplier = multiplier;
}

void deoglSkinTexture::SetReflectivitySolidityMultiplier( float multiplier ){
	pReflectivitySolidityMultiplier = multiplier;
}

void deoglSkinTexture::SetReflected( bool reflected ){
	pReflected = reflected;
}

void deoglSkinTexture::SetEnvironmentRoomSize( const decVector2 &size ){
	pEnvironmentRoomSize = size;
}

void deoglSkinTexture::SetEnvironmentRoomOffset( const decVector &offset ){
	pEnvironmentRoomOffset = offset;
}

void deoglSkinTexture::SetEnvironmentRoomEmissivityTint( const decColor &tint ){
	pEnvironmentRoomEmissivityTint = tint;
}

void deoglSkinTexture::SetEnvironmentRoomEmissivityIntensity( float intensity ){
	pEnvironmentRoomEmissivityIntensity = intensity;
}

void deoglSkinTexture::SetEmissivity( const decColor &emissivity ){
	pEmissivity = emissivity;
}

void deoglSkinTexture::SetEmissivityTint( const decColor &tint ){
	pEmissivityTint = tint;
}

void deoglSkinTexture::SetEmissivityIntensity( float intensity ){
	pEmissivityIntensity = intensity;
}

void deoglSkinTexture::SetEmissivityCameraAdapted( bool cameraAdapted ){
	pEmissivityCameraAdapted = cameraAdapted;
}

void deoglSkinTexture::SetThickness( float thickness ){
	pThickness = thickness;
}

void deoglSkinTexture::SetAbsorption( const decColor &absorption ){
	pAbsorption = absorption;
}

void deoglSkinTexture::SetAbsorptionRange( float range ){
	pAbsorptionRange = range;
}

void deoglSkinTexture::SetAbsorptionHalfIntensityDistance( float distance ){
	pAbsorptionHalfIntensityDistance = distance;
}

void deoglSkinTexture::SetRimEmissivity( const decColor &emissivity ){
	pRimEmissivity = emissivity;
}

void deoglSkinTexture::SetRimEmissivityTint( const decColor &tint ){
	pRimEmissivityTint = tint;
}

void deoglSkinTexture::SetRimEmissivityIntensity( float intensity ){
	pRimEmissivityIntensity = decMath::max( intensity, 0.0f );
}

void deoglSkinTexture::SetRimAngle( float angle ){
	pRimAngle = decMath::clamp( angle, 0.0f, 1.0f );
}

void deoglSkinTexture::SetRimExponent( float exponent ){
	pRimExponent = decMath::max( exponent, 0.0f );
}



void deoglSkinTexture::SetShadeless( bool shadeless ){
	pShadeless = shadeless;
}

void deoglSkinTexture::SetShadowNone( bool shadowNone ){
	pShadowNone = shadowNone;
}

void deoglSkinTexture::SetShadowImportance( int shadowImportance ){
	if( shadowImportance < 0 ){
		pShadowImportance = 0;
		
	}else if( shadowImportance > 10 ){
		pShadowImportance = 10;
		
	}else{
		pShadowImportance = ( char )shadowImportance;
	}
}

void deoglSkinTexture::SetHintNoCompression( bool hintNoCompression ){
	pHintNoCompression = hintNoCompression;
}

void deoglSkinTexture::SetHintLightBlocker( bool hintLightBlocker ){
	pHintLightBlocker = hintLightBlocker;
}



void deoglSkinTexture::SetTexCoordClamp( bool clamp ){
	pTexCoordClamp = clamp;
}

void deoglSkinTexture::SetTexCoordOffset( const decVector2 &offset ){
	pTexCoordOffset = offset;
}

void deoglSkinTexture::SetTexCoordScale( const decVector2 &scale ){
	pTexCoordScale = scale;
}

void deoglSkinTexture::SetTexCoordRotate( float rotate ){
	pTexCoordRotate = rotate;
}

void deoglSkinTexture::SetOmniDirRotate( const decVector &rotate ){
	pOmniDirRotate = rotate;
}

void deoglSkinTexture::SetOmniDirRotateSpot( const decVector &rotate ){
	pOmniDirRotateSpot = rotate;
}



void deoglSkinTexture::SetVariationU( bool enable ){
	pVariationU = enable;
}

void deoglSkinTexture::SetVariationV( bool enable ){
	pVariationV = enable;
}

void deoglSkinTexture::SetParticleSheetCount( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	pParticleSheetCount = count;
}



void deoglSkinTexture::SetOutlineColor( const decColor &color ){
	pOutlineColor = color;
}

void deoglSkinTexture::SetOutlineColorTint( const decColor &color ){
	pOutlineColorTint = color;
}

void deoglSkinTexture::SetOutlineSolidity( float solidity ){
	pOutlineSolidity = decMath::clamp( solidity, 0.0f, 1.0f );
}

void deoglSkinTexture::SetOutlineThickness( float thickness ){
	pOutlineThickness = decMath::max( thickness, 0.0f );
}

void deoglSkinTexture::SetOutlineThicknessScreen( bool enable ){
	pOutlineThicknessScreen = enable;
}

void deoglSkinTexture::SetOutlineEmissivity( const decColor &emissivity ){
	pOutlineEmissivity = emissivity;
}

void deoglSkinTexture::SetOutlineEmissivityTint( const decColor &emissivity ){
	pOutlineEmissivityTint = emissivity;
}

void deoglSkinTexture::SetOutlineEmissivityIntensity( float intensity ){
	pOutlineEmissivityIntensity = decMath::max( intensity, 0.0f );
}



void deoglSkinTexture::SetMirror( bool mirror ){
	pMirror = mirror;
}

void deoglSkinTexture::SetRendered( bool rendered ){
	pRendered = rendered;
}

void deoglSkinTexture::SetReflects( bool reflects ){
	pReflects = reflects;
}



deoglSkinTextureProperty &deoglSkinTexture::GetMaterialPropertyAt( int property ){
	if( property < 0 || property >= EMP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pMaterialProperties[ property ];
}

const deoglSkinTextureProperty &deoglSkinTexture::GetMaterialPropertyAt( int property ) const{
	if( property < 0 || property >= EMP_COUNT ){
		DETHROW( deeInvalidParam );
	}
	
	return pMaterialProperties[ property ];
}



// Private Functions
//////////////////////

void deoglSkinTexture::pCleanUp(){
	if( pSharedSPBElement ){
		pSharedSPBElement->FreeReference();
	}
	
	int i;
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( pChannels[ i ] ){
			delete pChannels[ i ];
			pChannels[ i ] = NULL;
		}
	}
}



#ifdef OS_ANDROID
	#define DO_PERFORMANCE_TIMING
#endif

void deoglSkinTexture::pPrepareChannels( deoglRSkin &skin, const deSkinTexture &texture ){
	// NOTE this is called during asynchronous resource loading. careful accessing other objects
	#ifdef DO_PERFORMANCE_TIMING
	decTimer timer;
	#endif
	
	const int propertyCount = texture.GetPropertyCount();
	int i;
	for( i=0; i<propertyCount; i++ ){
		pProcessProperty( skin, *texture.GetPropertyAt( i ) );
	}
	
	// determine if the skin uses renderables at all
	pRenderableMaterialProperties = false;
	
	for( i=0; i<EMP_COUNT; i++ ){
		if( pMaterialProperties[ i ].GetRenderable() != -1 ){
			pRenderableMaterialProperties = true;
			break;
		}
	}
	
	// determine if the texture is rendered
	pRendered = pMirror;
	
	// prepare channels required for cache loading
	deoglVSDetermineChannelFormat determineFormat( pRenderThread, skin, *this, texture );
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		determineFormat.ProcessChannel( ( deoglSkinChannel::eChannelTypes )i );
		if( ! determineFormat.GetIsDefined() ){
			continue;
		}
		
		/*
		if( determineFormat.GetSharedImage() && determineFormat.GetSharedImage()->GetSkinUse() ){
			pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s) Channel(%s): Using existing image %s",
				skin.GetFilename().GetString(), pName.GetString(),
				deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
				determineFormat.GetSharedImage()->GetFilename().GetString() );
		}
		*/
		
		pChannels[ i ] = new deoglSkinChannel( pRenderThread, ( deoglSkinChannel::eChannelTypes )i );
		pChannels[ i ]->PrepareChannel( skin, *this, texture, determineFormat );
		
		if( i == deoglSkinChannel::ectReflectivity ){
			pReflects = true;
		}
	}
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s): Prepare %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		( int )( timer.GetElapsedTime() * 1e3f ) );
	#endif
	
	// load caches
	pLoadCached( skin );
	#ifdef DO_PERFORMANCE_TIMING
	pRenderThread.GetOgl().LogInfoFormat( "Skin(%s) Texture(%s): LoadCache %dms",
		skin.GetFilename().GetString(), pName.GetString(),
		( int )( timer.GetElapsedTime() * 1e3f ) );
	#endif
	
	// for the rest BuildChannel() has to be called in time
}

void deoglSkinTexture::pLoadCached( deoglRSkin &skin ){
	// try to load caches using the calculated cache ids
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheTextures = caches.GetSkinTextures();
	deoglPixelBufferMipMap *pixelBufferMipMap = NULL;
	decBaseFileReader *reader = NULL;
	char *verifyData = NULL;
	int i;
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	caches.Lock();
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( ! pChannels[ i ] ){
			continue;
		}
		if( pChannels[ i ]->GetCacheID().IsEmpty() ){
			continue;
		}
		
		try{
			reader = cacheTextures.Read( pChannels[ i ]->GetCacheID() );
			if( ! reader ){
				if( enableCacheLogging ){
					pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Cache file absent"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
						pChannels[ i ]->GetCacheID().GetString() );
				}
				continue;
			}
			
			// check cache version in case we upgraded
			if( reader->ReadByte() != CACHE_FILE_VERSION ){
				// cache file outdated
				reader->FreeReference();
				reader = NULL;
				cacheTextures.Delete( pChannels[ i ]->GetCacheID() );
				
				if( enableCacheLogging ){
					pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Old version cache file"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
						pChannels[ i ]->GetCacheID().GetString() );
				}
				
				continue;
			}
			
			// check source verification data to reject the cached file if the source data changed.
			const decMemoryFile * const expectedVerify = pChannels[ i ]->GetCacheVerify();
			const int verifyLen = reader->ReadInt();
			bool verified = false;
			
			if( expectedVerify && verifyLen == expectedVerify->GetLength() ){
				if( verifyLen == 0 ){
					verified = true;
					
				}else{
					verifyData = new char[ verifyLen ];
					reader->Read( verifyData, verifyLen );
					
					verified = memcmp( verifyData, expectedVerify->GetPointer(), verifyLen ) == 0;
					
					delete [] verifyData;
					verifyData = NULL;
				}
			}
			
			if( ! verified ){
				// cache file verify size mismatch or sanity check failed
				reader->FreeReference();
				reader = NULL;
				cacheTextures.Delete( pChannels[ i ]->GetCacheID() );
				
				if( enableCacheLogging ){
					pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Verify data mismatch"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
						pChannels[ i ]->GetCacheID().GetString() );
				}
				
				continue;
			}
			
			// read pixel buffer
			const int pixBufCount = ( int )reader->ReadByte();
			const int width = ( int )reader->ReadShort();
			const int height = ( int )reader->ReadShort();
			const int depth = ( int )reader->ReadShort();
			const deoglPixelBuffer::ePixelFormats pbformat = ( deoglPixelBuffer::ePixelFormats )reader->ReadByte();
			int j;
			
			if( pChannels[ i ]->GetSize() != decPoint3( width, height, depth ) ){
				// cache file image size mismatch. this can happen for example in editors where the variation
				// texture property changed. in this case array textures of depth 1 can be cached due to
				// variation being disabled while the next time variation is enabled and the full depth is
				// used. also if the user changes module parameters it might be possible the textures sizes
				// are reduzed resulting in cache files not be valid anymore.
				reader->FreeReference();
				reader = NULL;
				cacheTextures.Delete( pChannels[ i ]->GetCacheID() );
				
				if( enableCacheLogging ){
					pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Cached image size mismatch."
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
						pChannels[ i ]->GetCacheID().GetString() );
				}
				
				continue;
			}
			
			const int maxMipMapLevel = decMath::max( pixBufCount - 1, 0 );
			
			int expectedMaxMipMaxLevel = maxMipMapLevel;
			if( i == deoglSkinChannel::ectSolidity ){
				expectedMaxMipMaxLevel = decMath::clamp(
					( int )( floorf( log2f( ( height > width ) ? height : width ) ) ) - 3, 0, 100 );
			}
			
			if( maxMipMapLevel < expectedMaxMipMaxLevel ){
				reader->FreeReference();
				reader = NULL;
				cacheTextures.Delete( pChannels[ i ]->GetCacheID() );
				
				if( enableCacheLogging ){
					pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Cached Mip-Map Count lower."
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
						pChannels[ i ]->GetCacheID().GetString() );
				}
				
				continue;
			}
			
			pixelBufferMipMap = new deoglPixelBufferMipMap( pbformat, width, height, depth, maxMipMapLevel );
			
			for( j=0; j<pixBufCount; j++ ){
				deoglPixelBuffer &pixelBuffer = *pixelBufferMipMap->GetPixelBuffer( j );
				reader->Read( pixelBuffer.GetPointer(), pixelBuffer.GetImageSize() );
			}
			
			if( maxMipMapLevel > expectedMaxMipMaxLevel ){
				pixelBufferMipMap->ReducePixelBufferCount( maxMipMapLevel - expectedMaxMipMaxLevel );
			}
			
			pChannels[ i ]->SetPixelBufferMipMap( pixelBufferMipMap );
			pixelBufferMipMap = NULL;
			
			// done
			reader->FreeReference();
			reader = NULL;
			
			if( enableCacheLogging ){
				pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Loaded from cache"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
					pChannels[ i ]->GetCacheID().GetString() );
			}
			
			pChannels[ i ]->SetIsCached( true );
			
		}catch( const deException & ){
			if( verifyData ){
				delete [] verifyData;
				verifyData = NULL;
			}
			if( reader ){
				reader->FreeReference();
				reader = NULL;
			}
			
			cacheTextures.Delete( pChannels[ i ]->GetCacheID() );
			
			if( enableCacheLogging ){
				pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Cache file damaged, forcing rebuild"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
					pChannels[ i ]->GetCacheID().GetString() );
			}
		}
	}
	
	caches.Unlock();
}

void deoglSkinTexture::pCreateMipMaps(){
	int i;
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( ! pChannels[ i ] ){
			continue;
		}
		if( pChannels[ i ]->GetIsCached() ){
			continue;
		}
		
		deoglPixelBufferMipMap * const pbMipMap = pChannels[ i ]->GetPixelBufferMipMap();
		if( ! pbMipMap ){
			continue;
		}
		
		switch( ( deoglSkinChannel::eChannelTypes )i ){
		case deoglSkinChannel::ectColor:
		case deoglSkinChannel::ectTransparency:
		case deoglSkinChannel::ectColorOmnidirCube:
		case deoglSkinChannel::ectColorOmnidirEquirect:
		case deoglSkinChannel::ectColorTintMask:
		case deoglSkinChannel::ectHeight:
		case deoglSkinChannel::ectEmissivity:
		case deoglSkinChannel::ectRefractDistort:
		case deoglSkinChannel::ectReflectivity:
		case deoglSkinChannel::ectRoughness:
		case deoglSkinChannel::ectEnvironmentMap:
		case deoglSkinChannel::ectEnvironmentRoom:
		case deoglSkinChannel::ectEnvironmentRoomMask:
		case deoglSkinChannel::ectEnvironmentRoomEmissivity:
		case deoglSkinChannel::ectAbsorption:
		case deoglSkinChannel::ectRimEmissivity:
		case deoglSkinChannel::ectNonPbrAlbedo:
		case deoglSkinChannel::ectNonPbrMetalness:
			pbMipMap->CreateMipMaps();
			break;
			
		case deoglSkinChannel::ectSolidity:
			// solidity is a tricky problem, especially with masked solidity. shaders use 35%
			// solidity as threshold to discard fragments. by using box filtering 0 values are
			// pulled towards 1 values and quickly end up above the 35% threshold. especially
			// with solidity textures placing holes near borders (for example to create blinds)
			// this causes the holes to vanish and can break shadow casting and GI handling
			// which rely on high LOD levels. there are now three possible solutions:
			// 
			// 1) use maximum filter. this favors solid geometry over holes.
			// 2) use minimum filter. this favors holes over solid geometry.
			// 3) use textureQueryLod in shader to figure out mip-map level used and pull
			//    the threshold from 35% closer to for example 95%.
			// 
			// the third solution has the problem of the function existing only since opengl 4.0
			// and what threshold to pull towards and in what speed.
			// 
			// the other two solutions are more practical and work also for opengl 3.3 GPUs.
			// the main problem here is what to favor. in general filling up holes over distance
			// is more visible since color texture is shown inside holes where it is not supposed
			// to show. this produces visible artifacts and requires carefully made textures
			// to hide these problems. by favoring holes geometry vanishes quicker which is
			// also noticable but does not add artifacts. favoring holes also helps with solidity
			// textures using holes at the border (like blinders). also vanishing solidity
			// geometry quicker improves performance. for vegetation it is a problem since
			// leaves vanish quicker.
			// 
			// one solution could be to limit the maximum mip-map level while creating mip-maps.
			// this avoids the extreme situations and should allow both types of geometry to
			// work better.
			// 
			// in tests both favor holes and favor geometry did not look that good.
			// so right now regular mip-mapping is used without limiting the mip-map level
			#if 0
			pbMipMap->ReducePixelBufferCount( 3 ); // minimum 8x8
			if( pbMipMap->GetPixelBufferCount() == 0 ){
				pChannels[ i ]->SetPixelBufferMipMap( NULL );
				break;
			}
			#endif
			if( pSolidityFilterPriority < 0.35f ){
				pbMipMap->CreateMipMapsMin();
				
			}else if( pSolidityFilterPriority > 0.65f ){
				pbMipMap->CreateMipMapsMax();
				
			}else{
				pbMipMap->CreateMipMaps();
			}
			break;
			
		case deoglSkinChannel::ectNormal:
			pbMipMap->CreateNormalMipMaps();
			break;
			
		case deoglSkinChannel::ectAO:
			pbMipMap->CreateMipMapsMax();
			break;
			
		default:
			break;
		}
	}
}

void deoglSkinTexture::pCompressTextures( deoglRSkin &skin, const deSkinTexture &texture ){
	deoglTextureCompression textureCompression;
	int i;
	
	textureCompression.SetFastCompression( true );
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		if( ! pChannels[ i ] ){
			continue;
		}
		if( pChannels[ i ]->GetIsCached() ){
			continue;
		}
		
		deoglPixelBufferMipMap * const pbMipMapSource = pChannels[ i ]->GetPixelBufferMipMap();
		if( ! pbMipMapSource ){
			continue;
		}
		
		// determine texture format depending on the texture type
		const deoglCapsTextureFormat *texformat;
		
		if( pChannels[ i ]->GetTexture() ){
			texformat = pChannels[ i ]->GetTexture()->GetFormat();
			
		}else if( pChannels[ i ]->GetCubeMap() ){
			texformat = pChannels[ i ]->GetCubeMap()->GetFormat();
			
		}else if( pChannels[ i ]->GetArrayTexture() ){
			texformat = pChannels[ i ]->GetArrayTexture()->GetFormat();
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
		// if the texture format is not compressed continue
		if( ! texformat->GetIsCompressed() ){
			continue;
		}
		
		// determine the compression technique
		const GLint glformat = texformat->GetFormat();
		bool canCompress = false;
		deoglPixelBuffer::ePixelFormats pbformat;
		
		if( glformat == GL_COMPRESSED_RGB_S3TC_DXT1 ){
			pbformat = deoglPixelBuffer::epfDXT1;
			canCompress = true;
			
		}else if( glformat == GL_COMPRESSED_RGBA_S3TC_DXT1 ){
			//pbformat = deoglPixelBuffer::epfDXT1;
			
		}else if( glformat == GL_COMPRESSED_RGBA_S3TC_DXT3 ){
			pbformat = deoglPixelBuffer::epfDXT3;
			canCompress = true;
			
		}else if( glformat == GL_COMPRESSED_RGBA_S3TC_DXT5 ){
			//pbformat = deoglPixelBuffer::epfDXT5;
		}
		
		if( ! canCompress ){
			// we can not compress it. in theory the driver would compress for us. unfortunately
			// various drivers exhibit crashes and even GPU lock-ups due to faulty compression
			// code included. for this reason only formats are allowed which we know how to
			// compress. should we end up here something went wrong.
			pRenderThread.GetLogger().LogErrorFormat( "CompressTextures of not supported format:"
				" skin='%s' texture='%s' channel=%i pbformat=%x", skin.GetFilename().GetString(),
				texture.GetName().GetString(), i, pChannels[ i ]->GetTexture()->GetFormat()->GetFormat() );
			
			DETHROW( deeInvalidParam );
		}
		
		deoglPixelBuffer &basePixelBuffer = *pbMipMapSource->GetPixelBuffer( 0 );
		deoglPixelBufferMipMap *pbMipMapCompressed = NULL;
		
		try{
			pbMipMapCompressed = new deoglPixelBufferMipMap( pbformat,
				basePixelBuffer.GetWidth(), basePixelBuffer.GetHeight(),
				basePixelBuffer.GetDepth(), pbMipMapSource->GetPixelBufferCount() - 1 );
			
			textureCompression.SetDecompressedDataMipMap( pbMipMapSource );
			textureCompression.SetCompressedDataMipMap( pbMipMapCompressed );
			//decTimer timer;
			textureCompression.CompressMipMap();
			/*float elapsed = timer.GetElapsedTime();
			pRenderThread.GetOgl().LogInfoFormat( "CompressTextures: skin='%s' texture='%s' channel=%i"
				" pbformat=%x width=%i height=%i mipmaps=%i elapsed=%iys",
				skin.GetSkin()->GetFilename(), texture.GetName(), i,
				compressedPixelBufferMipMap->GetPixelBuffer( 0 )->GetFormat(),
				basePixelBuffer.GetWidth(), basePixelBuffer.GetHeight(),
				compressedPixelBufferMipMap->GetPixelBufferCount(),
				( int )( elapsed * 1e6f ) );*/
			
			pChannels[ i ]->SetPixelBufferMipMap( pbMipMapCompressed );
			
		}catch( const deException & ){
			if( pbMipMapCompressed ){
				delete pbMipMapCompressed;
			}
			throw;
		}
	}
}

void deoglSkinTexture::pWriteCached( deoglRSkin &skin ){
	deoglCaches &caches = pRenderThread.GetOgl().GetCaches();
	deCacheHelper &cacheTextures = caches.GetSkinTextures();
	decBaseFileWriter *writer = NULL;
	int i;
	
	const bool enableCacheLogging = ENABLE_CACHE_LOGGING;
	
	caches.Lock();
	
	for( i=0; i<deoglSkinChannel::CHANNEL_COUNT; i++ ){
		try{
			if( ! pChannels[ i ] ){
				continue;
			}
			if( pChannels[ i ]->GetIsCached() ){
				continue; // has been loaded from cache
			}
			if( pChannels[ i ]->GetCacheID().IsEmpty() ){
				continue; // do not cache channel
			}
			
			if( pChannels[ i ]->GetImage() && pChannels[ i ]->GetImage()->GetSkinUse() ){
				// already build by somebody else.
				// 
				// this is an optimization. building channels runs potentially parallel to the
				// render thread running delayed operations to init skins. during this time
				// skin-use parameter can be changed to true. it is possible skin-use has not been
				// set during the first check in BuildChannels(). this forces the channels to be
				// build. if in the mean time the skin-use has been set to true it would be a
				// waste of time to create and upload textures during delayed operation time if
				// we know the textures are going to be dropped right away because somebody else
				// created them already. dropping the cache id and pixel buffer prevents any
				// further processing of this channel
				pChannels[ i ]->SetCacheID( "" );
				pChannels[ i ]->SetCanBeCached( false );
				pChannels[ i ]->SetPixelBufferMipMap( NULL );
				continue;
			}
			
			writer = cacheTextures.Write( pChannels[ i ]->GetCacheID() );
			
			// write cache version
			writer->WriteByte( CACHE_FILE_VERSION );
			
			// write source verification to reject the cached file if the source data changed
			const decMemoryFile * const verifyData = pChannels[ i ]->GetCacheVerify();
			if( ! verifyData ){
				DETHROW( deeInvalidParam );
			}
			
			writer->WriteInt( verifyData->GetLength() );
			writer->Write( verifyData->GetPointer(), verifyData->GetLength() );
			
			// write pixel buffer
			const deoglPixelBufferMipMap * const pixelBufferMipMap = pChannels[ i ]->GetPixelBufferMipMap();
			
			if( pixelBufferMipMap ){
				const int pixBufCount = pixelBufferMipMap->GetPixelBufferCount();
				int j;
				
				const deoglPixelBuffer &pixelBufferBase = *pixelBufferMipMap->GetPixelBuffer( 0 );
				writer->WriteByte( ( unsigned char )pixBufCount );
				writer->WriteShort( ( short )pixelBufferBase.GetWidth() );
				writer->WriteShort( ( short )pixelBufferBase.GetHeight() );
				writer->WriteShort( ( short )pixelBufferBase.GetDepth() );
				writer->WriteByte( ( unsigned char )pixelBufferBase.GetFormat() );
				
				for( j=0; j<pixBufCount; j++ ){
					const deoglPixelBuffer &pixelBuffer = *pixelBufferMipMap->GetPixelBuffer( j );
					writer->Write( pixelBuffer.GetPointer(), pixelBuffer.GetImageSize() );
				}
				
			}else{
				// this should not happen
				writer->WriteByte( 0 );
				
				//if( enableCacheLogging ){
					pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Pixel buffer missing"
						" (skin='%s' texture='%s' channel='%s' id='%s')",
						skin.GetFilename().GetString(), pName.GetString(),
						deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
						pChannels[ i ]->GetCacheID().GetString() );
				//}
			}
			
			// done
			writer->FreeReference();
			writer = NULL;
			
			if( enableCacheLogging ){
				pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Written to cache"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
					pChannels[ i ]->GetCacheID().GetString() );
			}
			
		}catch( const deException & ){
			if( writer ){
				writer->FreeReference();
				writer = NULL;
			}
			cacheTextures.Delete( pChannels[ i ]->GetCacheID() );
			
			if( enableCacheLogging ){
				pRenderThread.GetOgl().LogInfoFormat( "Skin Cache: Written cache file failed"
					" (skin='%s' texture='%s' channel='%s' id='%s')",
					skin.GetFilename().GetString(), pName.GetString(),
					deoglSkinChannel::ChannelNameFor( ( deoglSkinChannel::eChannelTypes )i ),
					pChannels[ i ]->GetCacheID().GetString() );
			}
		}
	}
	
	//cacheTextures.DebugPrint( *pRenderThread.GetOgl().GetGameEngine()->GetLogger(), "OpenGL" );
	caches.Unlock();
}

void deoglSkinTexture::pProcessProperty( deoglRSkin &skin, deSkinProperty &property ){
	const deoglSkinPropertyMap::ePropertyTypes propertyType =
		deoglSkinPropertyMap::GetTypeFor( property.GetType().GetString() );
	deSkinPropertyVisitorIdentify identify;
	float value;
	
	property.Visit( identify );
	
	if( identify.IsValue() ){
		value = identify.CastToValue().GetValue();
		
		switch( propertyType ){
		case deoglSkinPropertyMap::eptColor:
			pColor.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptColorGamma:
			pColorGamma = value;
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			pColorTint.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptColorTintMask:
			pColorTintMask = value;
			break;
			
		case deoglSkinPropertyMap::eptColorSolidityMultiplier:
			pColorSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			pColorOmnidirCube.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			pColorOmnidirEquirect.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusion:
			pAmbientOcclusion = value;
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusionSolidityMultiplier:
			pAmbientOcclusionSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptHeightScale:
			pHeightScale = value;
			break;
			
		case deoglSkinPropertyMap::eptHeightOffset:
			pHeightOffset = value;
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			pNormal.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptNormalStrength:
			pNormalStrength = value;
			break;
			
		case deoglSkinPropertyMap::eptNormalSolidityMultiplier:
			pNormalSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughness:
			pRoughness = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapLower:
			pRoughnessRemapLower = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapUpper:
			pRoughnessRemapUpper = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessGamma:
			pRoughnessGamma = value;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessSolidityMultiplier:
			pRoughnessSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptTransparency:
			pTransparency = value;
			break;
			
		case deoglSkinPropertyMap::eptTransparencyMultiplier:
			pTransparencyMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptSolidity:
			pSolidity = value;
			pHasSolidity = true;
			break;
			
		case deoglSkinPropertyMap::eptSolidityMultiplier:
			pSolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptSolidityMasked:
			pSolidityMasked = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptSolidityFilterPriority:
			pSolidityFilterPriority = decMath::clamp( value, 0.0f, 1.0f );
			break;
			
		case deoglSkinPropertyMap::eptRefractionDistortStrength:
			pRefractDistortStrength = value;
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			pReflectivity.Set( value, value, value );
			pReflects = ( value > FLOAT_SAFE_EPSILON );
			break;
			
		case deoglSkinPropertyMap::eptReflectivityMultiplier:
			pReflectivityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptReflectivitySolidityMultiplier:
			pReflectivitySolidityMultiplier = value;
			break;
			
		case deoglSkinPropertyMap::eptReflected:
			pReflected = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptMirror:
			pMirror = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			pEmissivity.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			pEmissivityTint.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptEmissivityIntensity:
			pEmissivityIntensity = value;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityCameraAdapted:
			pEmissivityCameraAdapted = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			pEnvironmentRoomSize.Set( value, value );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			pEnvironmentRoomOffset.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityIntensity:
			pEnvironmentRoomEmissivityIntensity = value;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			pEnvironmentRoomEmissivityTint.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptThickness:
			pThickness = value;
			break;
			
		case deoglSkinPropertyMap::eptAbsorption:
			pAbsorption.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionRange:
			pAbsorptionRange = value;
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionHalfIntensityDistance:
			pAbsorptionHalfIntensityDistance = value;
			break;
			
		case deoglSkinPropertyMap::eptShadeless:
			pShadeless = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptHintNoCompression:
			pHintNoCompression = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptHintLightBlocker:
			pHintLightBlocker = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordClamp:
			pTexCoordClamp = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			pTexCoordOffset.Set( value, value );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			pTexCoordScale.Set( value, value );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordRotate:
			pTexCoordRotate = value;
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			pOmniDirRotate.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			pOmniDirRotateSpot.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptShadowNone:
			pShadowNone = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptShadowImportance:
			pShadowImportance = ( int )( value * 10.0f + 0.01f );
			break;
			
		case deoglSkinPropertyMap::eptVariationU:
			pVariationU = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptVariationV:
			pVariationV = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptParticleSheets:
			pParticleSheetCount = decMath::max( ( int )( value + 0.5f ), 1 );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			pEmissivity.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			pRimEmissivityTint.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityIntensity:
			pRimEmissivityIntensity = value;
			break;
			
		case deoglSkinPropertyMap::eptRimAngle:
			pRimAngle = decMath::clamp( value, 0.0f, 1.0f );
			break;
			
		case deoglSkinPropertyMap::eptRimExponent:
			pRimExponent = decMath::max( value, 0.0f );
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			pOutlineColor.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			pOutlineColorTint.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptOutlineThickness:
			pOutlineThickness = decMath::max( value, 0.0f );
			pHasOutline = pOutlineThickness != 0.0f || ! property.GetRenderable().IsEmpty();
			break;
			
		case deoglSkinPropertyMap::eptOutlineThicknessScreen:
			pOutlineThicknessScreen = value > 0.5f;
			break;
			
		case deoglSkinPropertyMap::eptOutlineSolidity:
			pOutlineSolidity = decMath::clamp( value, 0.0f, 1.0f );
			pIsOutlineSolid = pOutlineSolidity == 1.0f && property.GetRenderable().IsEmpty();
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			pOutlineEmissivity.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			pOutlineEmissivityTint.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityIntensity:
			pOutlineEmissivityIntensity = decMath::max( value, 0.0f );
			pIsOutlineEmissive = pOutlineEmissivityIntensity != 0.0f || ! property.GetRenderable().IsEmpty();
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			pNonPbrAlbedo.Set( value, value, value );
			break;
			
		case deoglSkinPropertyMap::eptNonPbrMetalness:
			pNonPbrMetalness = value;
			break;
			
		default:
			break;
		}
		
	}else if( identify.IsColor() ){
		const decColor &color = identify.CastToColor().GetColor();
		
		switch( propertyType ){
		case deoglSkinPropertyMap::eptAbsorption:
			pAbsorption = color;
			break;
			
		case deoglSkinPropertyMap::eptColor:
			pColor = color;
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			pColorTint = color;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			pColorOmnidirCube = color;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			pColorOmnidirEquirect = color;
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			pNormal = color;
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			pReflectivity = color;
			pReflects = ( color.r > FLOAT_SAFE_EPSILON || color.g > FLOAT_SAFE_EPSILON || color.b > FLOAT_SAFE_EPSILON );
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			pEmissivity = color;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			pEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			pEnvironmentRoomSize.Set( color.r, color.g );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			pEnvironmentRoomOffset.Set( color.r, color.g, color.b );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			pEnvironmentRoomEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			pTexCoordOffset.Set( color.r, color.g );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			pTexCoordScale.Set( color.r, color.g );
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			pOmniDirRotate.Set( color.r, color.g, color.b );
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			pOmniDirRotateSpot.Set( color.r, color.g, color.b );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			pRimEmissivity = color;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			pRimEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			pOutlineColor = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			pOutlineColorTint = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			pOutlineEmissivity = color;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			pOutlineEmissivityTint = color;
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			pNonPbrAlbedo = color;
			break;
			
		default:
			break;
		}
		
	}else if( identify.IsMapped() ){
		deoglSkinTextureProperty *materialProperty = NULL;
		bool requiresTexture = false;
		
		switch( propertyType ){
		case deoglSkinPropertyMap::eptColor:
			materialProperty = pMaterialProperties + empColor;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			materialProperty = pMaterialProperties + empColorOmnidirCube;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			materialProperty = pMaterialProperties + empColorOmnidirEquirect;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			materialProperty = pMaterialProperties + empColorTint;
			break;
			
		case deoglSkinPropertyMap::eptColorTintMask:
			materialProperty = pMaterialProperties + empColorTintMask;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptColorGamma:
			materialProperty = pMaterialProperties + empColorGamma;
			break;
			
		case deoglSkinPropertyMap::eptColorSolidityMultiplier:
			materialProperty = pMaterialProperties + empColorSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusion:
			materialProperty = pMaterialProperties + empAmbientOcclusion;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusionSolidityMultiplier:
			materialProperty = pMaterialProperties + empAmbientOcclusionSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptTransparency:
			materialProperty = pMaterialProperties + empTransparency;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptTransparencyMultiplier:
			materialProperty = pMaterialProperties + empTransparencyMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptSolidity:
			materialProperty = pMaterialProperties + empSolidity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptSolidityMultiplier:
			materialProperty = pMaterialProperties + empSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptHeightScale:
			materialProperty = pMaterialProperties + empHeightScale;
			break;
			
		case deoglSkinPropertyMap::eptHeightOffset:
			materialProperty = pMaterialProperties + empHeightOffset;
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			materialProperty = pMaterialProperties + empNormal;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptNormalStrength:
			materialProperty = pMaterialProperties + empNormalStrength;
			break;
			
		case deoglSkinPropertyMap::eptNormalSolidityMultiplier:
			materialProperty = pMaterialProperties + empNormalSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptRoughness:
			materialProperty = pMaterialProperties + empRoughness;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapLower:
			materialProperty = pMaterialProperties + empRoughnessRemapLower;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapUpper:
			materialProperty = pMaterialProperties + empRoughnessRemapUpper;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessGamma:
			materialProperty = pMaterialProperties + empRoughnessGamma;
			break;
			
		case deoglSkinPropertyMap::eptRoughnessSolidityMultiplier:
			materialProperty = pMaterialProperties + empRoughnessSolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptRefractionDistortStrength:
			materialProperty = pMaterialProperties + empRefractDistortStrength;
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			materialProperty = pMaterialProperties + empReflectivity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptReflectivityMultiplier:
			materialProperty = pMaterialProperties + empReflectivityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptReflectivitySolidityMultiplier:
			materialProperty = pMaterialProperties + empReflectivitySolidityMultiplier;
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			materialProperty = pMaterialProperties + empEmissivity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			materialProperty = pMaterialProperties + empEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptEmissivityIntensity:
			materialProperty = pMaterialProperties + empEmissivityIntensity;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			materialProperty = pMaterialProperties + empEnvironmentRoomSize;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			materialProperty = pMaterialProperties + empEnvironmentRoomOffset;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			materialProperty = pMaterialProperties + empEnvironmentRoomEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityIntensity:
			materialProperty = pMaterialProperties + empEnvironmentRoomEmissivityIntensity;
			break;
			
		case deoglSkinPropertyMap::eptThickness:
			materialProperty = pMaterialProperties + empThickness;
			break;
			
		case deoglSkinPropertyMap::eptAbsorption:
			materialProperty = pMaterialProperties + empAbsorption;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionRange:
			materialProperty = pMaterialProperties + empAbsorptionRange;
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionHalfIntensityDistance:
			materialProperty = pMaterialProperties + empAbsorptionHalfIntensityDistance;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordClamp:
			materialProperty = pMaterialProperties + empTexCoordClamp;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			materialProperty = pMaterialProperties + empTexCoordOffset;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			materialProperty = pMaterialProperties + empTexCoordScale;
			break;
			
		case deoglSkinPropertyMap::eptTexCoordRotate:
			materialProperty = pMaterialProperties + empTexCoordRotate;
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			materialProperty = pMaterialProperties + empOmniDirRotate;
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			materialProperty = pMaterialProperties + empOmniDirRotateSpot;
			break;
			
		case deoglSkinPropertyMap::eptVariationU:
			materialProperty = pMaterialProperties + empVariationU;
			break;
			
		case deoglSkinPropertyMap::eptVariationV:
			materialProperty = pMaterialProperties + empVariationV;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			materialProperty = pMaterialProperties + empRimEmissivity;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			materialProperty = pMaterialProperties + empRimEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityIntensity:
			materialProperty = pMaterialProperties + empRimEmissivityIntensity;
			break;
			
		case deoglSkinPropertyMap::eptRimAngle:
			materialProperty = pMaterialProperties + empRimAngle;
			break;
			
		case deoglSkinPropertyMap::eptRimExponent:
			materialProperty = pMaterialProperties + empRimExponent;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			materialProperty = pMaterialProperties + empOutlineColor;
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			materialProperty = pMaterialProperties + empOutlineColorTint;
			break;
			
		case deoglSkinPropertyMap::eptOutlineThickness:
			materialProperty = pMaterialProperties + empOutlineThickness;
			pHasOutline = true;
			break;
			
		case deoglSkinPropertyMap::eptOutlineSolidity:
			materialProperty = pMaterialProperties + empOutlineSolidity;
			pIsOutlineSolid = false;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			materialProperty = pMaterialProperties + empOutlineEmissivity;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			materialProperty = pMaterialProperties + empOutlineEmissivityTint;
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityIntensity:
			materialProperty = pMaterialProperties + empOutlineEmissivityIntensity;
			pIsOutlineEmissive = true;
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			materialProperty = pMaterialProperties + empNonPbrAlbedo;
			requiresTexture = true;
			break;
			
		case deoglSkinPropertyMap::eptNonPbrMetalness:
			materialProperty = pMaterialProperties + empNonPbrMetalness;
			requiresTexture = true;
			break;
			
		default:
			break;
		}
		
		if( materialProperty ){
			const deSkinPropertyMapped &mapped = identify.CastToMapped();
			
			deObjectReference refCalculated;
			refCalculated.TakeOver( new deoglSkinCalculatedProperty );
			
			deoglSkinCalculatedProperty &calculated = ( deoglSkinCalculatedProperty& )( deObject& )refCalculated;
			calculated.GetMappedComponent( 0 ) = mapped.GetRed();
			calculated.GetMappedComponent( 1 ) = mapped.GetGreen();
			calculated.GetMappedComponent( 2 ) = mapped.GetBlue();
			calculated.GetMappedComponent( 3 ) = mapped.GetAlpha();
			calculated.SetRequiresTexture( requiresTexture );
			calculated.Prepare();
			
			materialProperty->SetCalculatedProperty( skin.AddCalculatedProperty( &calculated ) );
			pCalculatedProperties = true;
		}
	}
	
	// check if the property has a renderable
	if( ! property.GetRenderable().IsEmpty() ){
		const char * const renderable = property.GetRenderable().GetString();
		
		switch( propertyType ){
		case deoglSkinPropertyMap::eptColor:
			pMaterialProperties[ empColor ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empColor ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidir:
			pMaterialProperties[ empColorOmnidirCube ].SetRenderable(
				skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empColorOmnidirCube ].GetRenderable() ).
				SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptColorOmnidirEquirect:
			pMaterialProperties[ empColorOmnidirEquirect ].SetRenderable(
				skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empColorOmnidirEquirect ].GetRenderable() ).
				SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptColorTint:
			pMaterialProperties[ empColorTint ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptColorTintMask:
			pMaterialProperties[ empColorTintMask ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empColorTintMask ].GetRenderable() ).
				SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptColorGamma:
			pMaterialProperties[ empColorGamma ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptColorSolidityMultiplier:
			pMaterialProperties[ empColorSolidityMultiplier ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusion:
			pMaterialProperties[ empAmbientOcclusion ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empAmbientOcclusion ].GetRenderable() )
				.SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptAmbientOcclusionSolidityMultiplier:
			pMaterialProperties[ empAmbientOcclusionSolidityMultiplier ]
				.SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptTransparency:
			pMaterialProperties[ empTransparency ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empTransparency ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptTransparencyMultiplier:
			pMaterialProperties[ empTransparencyMultiplier ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptSolidity:
			pMaterialProperties[ empSolidity ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empSolidity ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptSolidityMultiplier:
			pMaterialProperties[ empSolidityMultiplier ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptHeightScale:
			pMaterialProperties[ empHeightScale ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptHeightOffset:
			pMaterialProperties[ empHeightOffset ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptNormal:
			pMaterialProperties[ empNormal ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empNormal ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptNormalStrength:
			pMaterialProperties[ empNormalStrength ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptNormalSolidityMultiplier:
			pMaterialProperties[ empNormalSolidityMultiplier ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRoughness:
			pMaterialProperties[ empRoughness ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empRoughness ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapLower:
			pMaterialProperties[ empRoughnessRemapLower ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRoughnessRemapUpper:
			pMaterialProperties[ empRoughnessRemapUpper ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRoughnessGamma:
			pMaterialProperties[ empRoughnessGamma ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRoughnessSolidityMultiplier:
			pMaterialProperties[ empRoughnessSolidityMultiplier ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRefractionDistortStrength:
			pMaterialProperties[ empRefractDistortStrength ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptReflectivity:
			pMaterialProperties[ empReflectivity ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empReflectivity ].GetRenderable() )
				.SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptReflectivityMultiplier:
			pMaterialProperties[ empReflectivityMultiplier ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptReflectivitySolidityMultiplier:
			pMaterialProperties[ empReflectivitySolidityMultiplier ]
				.SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptEmissivity:
			pMaterialProperties[ empEmissivity ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empEmissivity ].GetRenderable() )
				.SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptEmissivityTint:
			pMaterialProperties[ empEmissivityTint ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptEmissivityIntensity:
			pMaterialProperties[ empEmissivityIntensity ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomSize:
			pMaterialProperties[ empEnvironmentRoomSize ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomOffset:
			pMaterialProperties[ empEnvironmentRoomOffset ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityTint:
			pMaterialProperties[ empEnvironmentRoomEmissivityTint ]
				.SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptEnvironmentRoomEmissivityIntensity:
			pMaterialProperties[ empEnvironmentRoomEmissivityIntensity ]
				.SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptThickness:
			pMaterialProperties[ empThickness ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptAbsorption:
			pMaterialProperties[ empAbsorption ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empAbsorption ]
				.GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionRange:
			pMaterialProperties[ empAbsorptionRange ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptAbsorptionHalfIntensityDistance:
			pMaterialProperties[ empAbsorptionHalfIntensityDistance ]
				.SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordClamp:
			pMaterialProperties[ empTexCoordClamp ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordOffset:
			pMaterialProperties[ empTexCoordOffset ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordScale:
			pMaterialProperties[ empTexCoordScale ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptTexCoordRotate:
			pMaterialProperties[ empTexCoordRotate ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotate:
			pMaterialProperties[ empOmniDirRotate ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOmniDirRotateSpot:
			pMaterialProperties[ empOmniDirRotateSpot ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptVariationU:
			pMaterialProperties[ empVariationU ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptVariationV:
			pMaterialProperties[ empVariationV ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivity:
			pMaterialProperties[ empRimEmissivity ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empRimEmissivity ].GetRenderable() )
				.SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityTint:
			pMaterialProperties[ empRimEmissivityTint ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRimEmissivityIntensity:
			pMaterialProperties[ empRimEmissivityIntensity ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRimAngle:
			pMaterialProperties[ empRimAngle ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptRimExponent:
			pMaterialProperties[ empRimExponent ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineColor:
			pMaterialProperties[ empOutlineColor ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineColorTint:
			pMaterialProperties[ empOutlineColorTint ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineThickness:
			pMaterialProperties[ empOutlineThickness ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineSolidity:
			pMaterialProperties[ empOutlineSolidity ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivity:
			pMaterialProperties[ empOutlineEmissivity ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityTint:
			pMaterialProperties[ empOutlineEmissivityTint ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptOutlineEmissivityIntensity:
			pMaterialProperties[ empOutlineEmissivityIntensity ].SetRenderable( skin.AddRenderable( renderable ) );
			break;
			
		case deoglSkinPropertyMap::eptNonPbrAlbedo:
			pMaterialProperties[ empNonPbrAlbedo ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empNonPbrAlbedo ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		case deoglSkinPropertyMap::eptNonPbrMetalness:
			pMaterialProperties[ empNonPbrMetalness ].SetRenderable( skin.AddRenderable( renderable ) );
			skin.GetRenderableAt( pMaterialProperties[ empNonPbrMetalness ].GetRenderable() ).SetRequiresTexture( true );
			break;
			
		default:
			break;
		}
	}
}

void deoglSkinTexture::pUpdateParamBlock( deoglShaderParameterBlock &spb, int element ){
	// etutValueColorTransparency,
	// etutValueNormalHeight,
	// etutValueReflectivityRoughness,
	// etutValueRefractionDistort,
	// etutValueAO,
	spb.SetParameterDataVec3( deoglSkinShader::etutTexColorTint, element,
		powf( pColorTint.r, 2.2f ), powf( pColorTint.g, 2.2f ), powf( pColorTint.b, 2.2f ) );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexColorGamma, element, pColorGamma );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexColorSolidMultiplier, element,
		pColorSolidityMultiplier );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexAOSolidityMultiplier, element,
		pAmbientOcclusionSolidityMultiplier );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexTransparencyMultiplier, element,
		pTransparencyMultiplier );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexSolidityMultiplier, element,
		pSolidityMultiplier );
	
	// (texHeight - 0.5 + offset) * scale = texHeight*scale + (offset-0.5)*scale
	spb.SetParameterDataVec2( deoglSkinShader::etutTexHeightRemap, element,
		pHeightScale, ( pHeightOffset - 0.5f ) * pHeightScale );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexNormalStrength, element, pNormalStrength );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexNormalSolidityMultiplier, element,
		pNormalSolidityMultiplier );
	
	spb.SetParameterDataVec2( deoglSkinShader::etutTexRoughnessRemap, element,
		pRoughnessRemapUpper - pRoughnessRemapLower, pRoughnessRemapLower );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexRoughnessGamma, element, pRoughnessGamma );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexRoughnessSolidityMultiplier, element,
		pRoughnessSolidityMultiplier );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexReflectivitySolidityMultiplier, element,
		pReflectivitySolidityMultiplier );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexRefractionDistortStrength, element,
		pRefractDistortStrength );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexEmissivityIntensity, element,
		powf( pEmissivityTint.r, 2.2f ) * pEmissivityIntensity,
		powf( pEmissivityTint.g, 2.2f ) * pEmissivityIntensity,
		powf( pEmissivityTint.b, 2.2f ) * pEmissivityIntensity );
	
	spb.SetParameterDataVec2( deoglSkinShader::etutTexEnvRoomSize, element,
		1.0f / pEnvironmentRoomSize.x, -1.0f / pEnvironmentRoomSize.y );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexEnvRoomOffset, element,
		pEnvironmentRoomOffset );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexEnvRoomEmissivityIntensity, element,
		powf( pEnvironmentRoomEmissivityTint.r, 2.2f ) * pEnvironmentRoomEmissivityIntensity,
		powf( pEnvironmentRoomEmissivityTint.g, 2.2f ) * pEnvironmentRoomEmissivityIntensity,
		powf( pEnvironmentRoomEmissivityTint.b, 2.2f ) * pEnvironmentRoomEmissivityIntensity );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexThickness, element, pThickness );
	spb.SetParameterDataFloat( deoglSkinShader::etutTexAbsorptionRange, element, pAbsorptionRange );
	
	spb.SetParameterDataVec2( deoglSkinShader::etutTexVariationEnableScale, element,
		pVariationU ? 1.0f : 0.0f, pVariationV ? 1.0f : 0.0f );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexReflectivityMultiplier, element,
		pReflectivityMultiplier );
	
	// see shader source geometry/defren/skin/particle_ribbon.glsl for the
	// calculation of the maximum number of supported sheets (8)
	spb.SetParameterDataInt( deoglSkinShader::etutTexParticleSheetCount, element,
		decMath::clamp( pParticleSheetCount, 1, 8 ) );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexRimEmissivityIntensity, element,
		powf( pRimEmissivityTint.r, 2.2f ) * pRimEmissivityIntensity,
		powf( pRimEmissivityTint.g, 2.2f ) * pRimEmissivityIntensity,
		powf( pRimEmissivityTint.b, 2.2f ) * pRimEmissivityIntensity );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexRimAngle, element,
		pRimAngle > 0.001f ? 1.0f / ( pRimAngle * HALF_PI ) : 0.0f );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexRimExponent, element,
		decMath::max( pRimExponent, 0.001f ) );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexOutlineColor, element,
		powf( pOutlineColor.r, 2.2f ), powf( pOutlineColor.g, 2.2f ),
		powf( pOutlineColor.b, 2.2f ) );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexOutlineColorTint, element,
		powf( pOutlineColorTint.r, 2.2f ), powf( pOutlineColorTint.g, 2.2f ),
		powf( pOutlineColorTint.b, 2.2f ) );
	
	spb.SetParameterDataFloat( deoglSkinShader::etutTexOutlineThickness, element, pOutlineThickness );
	spb.SetParameterDataFloat( deoglSkinShader::etutTexOutlineSolidity, element, pOutlineSolidity );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexOutlineEmissivity, element,
		powf( pOutlineEmissivity.r, 2.2f ) * pOutlineEmissivityIntensity,
		powf( pOutlineEmissivity.g, 2.2f ) * pOutlineEmissivityIntensity,
		powf( pOutlineEmissivity.b, 2.2f ) * pOutlineEmissivityIntensity );
	
	spb.SetParameterDataVec3( deoglSkinShader::etutTexOutlineEmissivityTint, element,
		powf( pOutlineEmissivityTint.r, 2.2f ), powf( pOutlineEmissivityTint.g, 2.2f ),
		powf( pOutlineEmissivityTint.b, 2.2f ) );
	
	spb.SetParameterDataBool( deoglSkinShader::etutTexEmissivityCameraAdapted, element,
		pEmissivityCameraAdapted );
}

void deoglSkinTexture::pUpdateRenderTaskFilters(){
	pRenderTaskFilters = ertfRender;
	if( pSolid ){
		pRenderTaskFilters |= ertfSolid;
	}
	if( pHasOutline ){
		pRenderTaskFilters |= ertfOutline;
	}
	if( pIsOutlineSolid ){
		pRenderTaskFilters |= ertfOutlineSolid;
	}
	if( pHasHoles ){
		pRenderTaskFilters |= ertfHoles;
	}
	if( pShadowNone ){
		pRenderTaskFilters |= ertfShadowNone;
	}
	if( pReflected ){
		pRenderTaskFilters |= ertfReflected;
	}
}
