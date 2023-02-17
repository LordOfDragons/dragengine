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

#ifndef _DEOGLLIGHTSHADER_H_
#define _DEOGLLIGHTSHADER_H_

#include "deoglLightShaderConfig.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"

#include <dragengine/deObject.h>

class deoglRenderThread;
class deoglShaderSources;
class deoglShaderDefines;


/**
 * Skin Shader.
 */
class deoglLightShader : public deObject{
public:
	typedef deTObjectReference<deoglLightShader> Ref;
	
	/** Texture targets. */
	enum eTextureTargets{
		ettDepth,
		ettDiffuse,
		ettNormal,
		ettReflectivity,
		ettRoughness,
		ettAOSolidity,
		ettSubSurface,
		ettColor,
		ettColorCubemap,
		ettColorEquirect,
		ettSamples,
		ettNoise,
		ettShadow1SolidDepth,
		ettShadow1TransparentDepth,
		ettShadow1TransparentColor,
		ettShadow2SolidDepth,
		ettShadow2TransparentDepth,
		ettShadow2TransparentColor,
		ettShadow1Ambient,
		ettShadow2Ambient,
		ettLightDepth1,
		ettLightDepth2,
		ettPosition,
		ettOTOcclusion,
		ettOTDistance,
		ETT_COUNT
	};
	
	/** Instance parameter uniform targets. */
	enum eInstanceUniformTargets{
		eiutMatrixMVP,
		eiutMatrixMV,
		
		eiutSamplesParams,
		eiutBurstFactor,
		
		eiutLightPosition,
		eiutLightView,
		eiutLightParams,
		
		eiutShadowMatrix1,
		eiutShadowMatrix2,
		eiutShadowMatrix3,
		eiutShadowMatrix4,
		eiutLayerBorder,
		
		eiutLightImageOmniMatrix,
		
		eiutShadow1Solid,
		eiutShadow1Transparent,
		eiutShadow2Solid,
		eiutShadow2Transparent,
		
		eiutShadowDepthTransform,
		eiutShadowDepthTransform2,
		
		eiutGIShadowMatrix,
		eiutGIShadowParams,
		
		EIUT_COUNT
	};
	
	/** Light parameter uniform targets. */
	enum eLightUniformTargets{
		elutLightColor,
		elutLightRange,
		elutLightColorAmbient,
		elutLightAmbientRatio,
		elutLightGIAmbientRatio,
		elutLightAttenuationCoefficient,
		elutLightDampingCoefficient,
		elutLightDampingThreshold,
		elutLightImageGamma,
		
		elutLightSpotFactor,
		elutLightSpotBase,
		elutLightSpotExponent,
		
		ELUT_COUNT
	};
	
	/** Uniform blocks. */
	enum eUniformBlocks{
		eubRenderParameters,
		eubInstanceParameters,
		eubLightParameters,
		EUB_COUNT
	};
	
	
	
public:
	deoglRenderThread &pRenderThread;
	
	const deoglLightShaderConfig pConfig;
	
	int pTextureTargets[ ETT_COUNT ];
	int pUsedTextureTargetCount;
	int pInstanceUniformTargets[ EIUT_COUNT ];
	int pUsedInstanceUniformTargetCount;
	int pLightUniformTargets[ ELUT_COUNT ];
	int pUsedLightUniformTargetCount;
	
	deoglShaderSources::Ref pSources;
	deoglShaderProgram::Ref pShader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin shader. */
	deoglLightShader( deoglRenderThread &renderThread, const deoglLightShaderConfig &config );
	/** Cleans up the skin shader. */
	virtual ~deoglLightShader();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Configuration. */
	inline const deoglLightShaderConfig &GetConfig() const{ return pConfig; }
	
	/** Index for texture target or -1 if not used. */
	int GetTextureTarget( eTextureTargets target ) const;
	
	/** Set index for texture target or -1 if not used. */
	void SetTextureTarget( eTextureTargets target, int index );
	
	/** Used texture target count. */
	inline int GetUsedTextureTargetCount() const{ return pUsedTextureTargetCount; }
	
	/** Set used texture target count. */
	void SetUsedTextureTargetCount( int usedTextureTargetCount );
	
	/** Index for instance parameter uniform target or -1 if not used. */
	int GetInstanceUniformTarget( eInstanceUniformTargets target ) const;
	
	/** Set index for instance parameter uniform target or -1 if not used. */
	void SetInstanceUniformTarget( eInstanceUniformTargets target, int index );
	
	/** Index for light parameter uniform target or -1 if not used. */
	int GetLightUniformTarget( eLightUniformTargets target ) const;
	
	/** Ensure shader is created if not existing already. */
	void EnsureShaderExists();
	
	/** Shader generating it if not existing already. */
	deoglShaderProgram *GetShader();
	
	/** Set index for light parameter uniform target or -1 if not used. */
	void SetLightUniformTarget( eLightUniformTargets target, int index );
	
	/** Create instance parameter shader parameter block. */
	deoglSPBlockUBO::Ref CreateSPBInstParam() const;
	
	/** Create light parameter shader parameter block. */
	deoglSPBlockUBO::Ref CreateSPBLightParam() const;
	
	/** Create light occlusion query parameter shader parameter block. */
	static deoglSPBlockUBO::Ref CreateSPBOccQueryParam( deoglRenderThread &renderThread );
	/*@}*/
	
	
	
	/** \name Shader Generation */
	/*@{*/
	/** Generate shader. */
	void GenerateShader();
	
	/** Generate shader defines. */
	void GenerateDefines( deoglShaderDefines &defines );
	
	/** Generate source code for the vertex unit. */
	void GenerateVertexSC();
	
	/** Generate source code for the geometry unit. */
	void GenerateGeometrySC();
	
	/** Generate source code for the fragment unit. */
	void GenerateFragmentSC();
	
	/** Update texture targets. */
	void UpdateTextureTargets();
	
	/** Update static and dynamic uniform targets. */
	void UpdateUniformTargets();
	
	/** Init shader parameters. */
	void InitShaderParameters();
	/*@}*/
};

#endif
