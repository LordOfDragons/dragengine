/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINTEXTUREPIPELINES_H_
#define _DEOGLSKINTEXTUREPIPELINES_H_

#include "deoglSkinTexturePipeline.h"
#include "../channel/deoglSkinChannel.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../utils/deoglDebugNamesEnumSet.h"

#include <dragengine/deObject.h>

class deoglShaderLoadingTimeout;
class deoglSkinTexture;
class deoglSkinShaderConfig;


/**
 * Geometry specific skin texture pipelines.
 */
class deoglSkinTexturePipelines : public deObject{
public:
	typedef deTObjectReference<deoglSkinTexturePipelines> Ref;
	
	/** Type. */
	enum eTypes{
		etGeometry,
		etGeometryDepthTest,
		etDepth,
		etDepthClipPlane,
		etDepthReversed,
		etDepthClipPlaneReversed,
		etCounter,
		etCounterClipPlane,
		etMask,
		etShadowProjection,
		etShadowProjectionCube,
		etShadowOrthogonal,
		etShadowOrthogonalCascaded,
		etShadowDistance,
		etShadowDistanceCube,
		etEnvMap,
		etLuminance,
		etGIMaterial
	};
	
	const static int TypeCount = etGIMaterial + 1;
	static const deoglDebugNamesEnum DebugNamesTypes;
	
	/** Modifier. */
	enum eModifiers{
		emStereo = 0x1,
		emFlipCullFace = 0x2,
		emDoubleSided = 0x4
	};
	
	const static int ModifiersPerType = emDoubleSided << 1;
	static const deoglDebugNamesEnumSet DebugNamesModifiers;
	
	
	
protected:
	const deoglSkinTexture &pTexture;
	
	deoglSkinTexturePipeline::Ref pPipelines[ TypeCount ][ ModifiersPerType ];
	bool pPrepared;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline. */
	deoglSkinTexturePipelines( const deoglSkinTexture &texture );
	
protected:
	/** Clean up skin texture pipeline. */
	virtual ~deoglSkinTexturePipelines();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Texture. */
	inline const deoglSkinTexture &GetTexture() const{ return pTexture; }
	
	/** Pipeline with type and modifiers or nullptr. */
	const deoglSkinTexturePipeline *GetWith( eTypes type, int modifiers = 0 ) const;
	
	/** Pipeline with type and modifiers or throws exception. */
	const deoglSkinTexturePipeline &GetWithRef( eTypes type, int modifiers = 0 ) const;
	
	/** Prepare pipelines. */
	void Prepare( deoglShaderLoadingTimeout &timeout );
	
	/** Debug name. */
	virtual const char *GetDebugName() const = 0;
	/*@}*/
	
	
	
protected:
	struct sChannelInfo{
		const deoglSkinChannel *channel;
		bool hasTextures;
	};
	
	typedef sChannelInfo ChannelInfo[ deoglSkinChannel::CHANNEL_COUNT ];
	virtual void pPreparePipelines( const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout ) = 0;
	
	virtual void pPrepareGeometry( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareGeometryDepthTest( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareAllDepth( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareDepth( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareDepthClipPlane( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareDepthReversed( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareDepthClipPlaneReversed( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareAllCounter( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareCounter( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareCounterClipPlane( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareMask( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareAllShadow( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareShadowProjection( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareShadowProjectionCube( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareShadowOrthogonal( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareShadowOrthogonalCascaded( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareShadowDistance( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareShadowDistanceCube( deoglPipelineConfiguration &basePipelineConfig,
		deoglSkinShaderConfig &baseShaderConfig, const ChannelInfo &cinfo,
		deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareEnvMap( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareLuminance( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pPrepareGIMaterial( deoglSkinShaderConfig &baseShaderConfig,
		const ChannelInfo &cinfo, deoglShaderLoadingTimeout &timeout );
	
	virtual void pInitChannelInfo( ChannelInfo &cinfo );
	
	virtual void pPipelineConfigGeometry( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigGeometryDepthTest( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigDepth( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigDepthReversed( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigCounter( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigMask( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigShadowProjection( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigShadowOrthogonal( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigShadowDistance( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigGIMaterial( deoglPipelineConfiguration &config );
	
	virtual void pPipelineConfigSetShadowOffsetProjection( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigSetShadowOffsetOrthogonal( deoglPipelineConfiguration &config );
	virtual void pPipelineConfigSetShadowOffsetDistance( deoglPipelineConfiguration &config );
	
	virtual void pSetBase( deoglSkinShaderConfig &config );
	
	virtual void pSetGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetGeometryDepthTest( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetMask( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetShadowProjection( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetShadowProjectionCube( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetShadowOrthogonal( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetShadowOrthogonalCascaded( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetShadowDistance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetShadowDistanceCube( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetEnvMap( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetLuminance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetGIMaterial( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	
	virtual void pSetMaskedSolidity( deoglSkinShaderConfig &config );
	virtual void pSetSkinClipping( deoglSkinShaderConfig &config );
	
	virtual void pSetTypeGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTypeDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTypeCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTypeMask( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTypeShadow( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	
	virtual void pSetTexturesGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTexturesGeometryLuminance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetTexturesGeometryGI( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	
	virtual void pSetDynamicGeometry( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicsGeometryLuminance( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicDepth( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicCounter( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	virtual void pSetDynamicMask( deoglSkinShaderConfig &config, const ChannelInfo &cinfo );
	
	virtual void pCreatePipelines( const deoglPipelineConfiguration &basePipelineConfig,
		const deoglSkinShaderConfig &baseShaderConfig, eTypes type, int modifierMask,
		deoglShaderLoadingTimeout &timeout );
};

#endif
