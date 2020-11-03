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

#include "deoglLightShader.h"
#include "deoglLightShaderManager.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../extensions/deoglExtensions.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../renderthread/deoglRTShader.h"
#include "../../shaders/deoglShaderCompiled.h"
#include "../../shaders/deoglShaderLanguage.h"
#include "../../shaders/deoglShaderManager.h"
#include "../../shaders/deoglShaderProgram.h"
#include "../../shaders/deoglShaderSources.h"
#include "../../shaders/paramblock/deoglSPBlockUBO.h"
#include "../../shaders/paramblock/deoglSPBParameter.h"

#include <dragengine/common/exceptions.h>



// Definitions
////////////////

static const char *vTextureTargetNames[ deoglLightShader::ETT_COUNT ] = {
	"texDepth", // ettDepth
	"texDiffuse", // ettDiffuse
	"texNormal", // ettNormal
	"texReflectivity", // ettReflectivity
	"texRoughness", // ettRoughness
	"texAOSolidity", // ettAOSolidity
	"texSubSurface", // ettSubSurface
	"texColor", // ettColor
	"texColorCubemap", // ettColorCubemap
	"texColorEquirect", // ettColorEquirect
	"texSamples", // ettSamples
	"texNoise", // ettNoise
	"texShadow1SolidDepth", // ettShadow1SolidDepth
	"texShadow1TransparentDepth", // ettShadow1TransparentDepth
	"texShadow1TransparentColor", // ettShadow1TransparentColor
	"texShadow2SolidDepth", // ettShadow2SolidDepth
	"texShadow2TransparentDepth", // ettShadow2TransparentDepth
	"texShadow2TransparentColor", // ettShadow2TransparentColor
	"texShadowAmbient", // ettShadowAmbient
	"texLightDepth1", // ettLightDepth1
	"texLightDepth2" // ettLightDepth2
};

static const char *vInstanceUniformTargetNames[ deoglLightShader::EIUT_COUNT ] = {
	"pMatrixMVP", // eiutMatrixMVP
	"pMatrixMV", // eiutMatrixMV
	
	"pSamplesParams", // eiutSamplesParams
	"pBurstFactor", // eiutBurstFactor
	
	"pLightPosition", // eiutLightPosition
	"pLightView", // eiutLightView
	
	"pShadowMatrix", // eiutShadowMatrix
	"pShadowMatrix2", // eiutShadowMatrix2
	"pShadowMatrix3", // eiutShadowMatrix3
	"pShadowMatrix4", // eiutShadowMatrix4
	"pLayerBorder", // eiutLayerBorder
	
	"pLightImageOmniMatrix", // eiutLightImageOmniMatrix
	
	"pShadow1Solid", // elutShadow1Solid
	"pShadow1Transparent", // elutShadow1Transparent
	"pShadow2Solid", // eiutShadow2Solid
	"pShadow2Transparent", // eiutShadow2Transparent
	
	"pShadowDepthTransform", // eiutShadowDepthTransform
	"pShadowDepthTransform2" // eiutShadowDepthTransform2
};

static const char *vLightUniformTargetNames[ deoglLightShader::ELUT_COUNT ] = {
	"pLightColor", // elutLightColor
	"pLightRange", // elutLightRange
	"pLightColorAmbient", // elutLightColorAmbient
	"pLightAmbientRatio", // elutLightAmbientRatio
	"pLightAttenuationCoefficient", // elutLightAttenuationCoefficient
	"pLightDampingCoefficient", // elutLightDampingCoefficient
	"pLightDampingThreshold", // elutLightDampingThreshold
	"pLightImageGamma", // elutLightImageGamma
	
	"pLightSpotFactor", // elutLightSpotFactor
	"pLightSpotBase", // elutLightSpotBase
	"pLightSpotExponent" // elutLightSpotExponent
};

struct sSPBParameterDefinition{
	deoglSPBParameter::eValueTypes dataType;
	int componentCount;
	int vectorCount;
};

static const sSPBParameterDefinition vInstanceSPBParamDefs[ deoglLightShader::EIUT_COUNT ] = {
	{ deoglSPBParameter::evtFloat, 4, 4 }, // pMatrixMVP ( mat4 )
	{ deoglSPBParameter::evtFloat, 4, 3 }, // pMatrixMV ( mat4x3 )
	
	{ deoglSPBParameter::evtFloat, 4, 1 }, // pSamplesParams ( vec4 )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pBurstFactor ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pLightPosition ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pLightView ( vec3 )
	
	{ deoglSPBParameter::evtFloat, 4, 4 }, // pShadowMatrix ( mat4 )
	{ deoglSPBParameter::evtFloat, 4, 4 }, // pShadowMatrix2 ( mat4 )
	{ deoglSPBParameter::evtFloat, 4, 4 }, // pShadowMatrix3 ( mat4 )
	{ deoglSPBParameter::evtFloat, 4, 4 }, // pShadowMatrix4 ( mat4 )
	{ deoglSPBParameter::evtFloat, 4, 1 }, // pLayerBorder ( vec4 )
	
	{ deoglSPBParameter::evtFloat, 4, 3 }, // pLightImageOmniMatrix ( mat4x3 )
	
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pShadow1Solid ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pShadow1Transparent ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pShadow2Solid ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pShadow2Transparent ( vec3 )
	
	{ deoglSPBParameter::evtFloat, 2, 1 }, // pShadowDepthTransform ( vec2 )
	{ deoglSPBParameter::evtFloat, 2, 1 } // pShadowDepthTransform2 ( vec2 )
};

static const sSPBParameterDefinition vLightSPBParamDefs[ deoglLightShader::ELUT_COUNT ] = {
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pLightColor ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightRange ( float )
	{ deoglSPBParameter::evtFloat, 3, 1 }, // pLightColorAmbient ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightAmbientRatio ( float )
	
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightAttenuationCoefficient ( float )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightDampingCoefficient ( float )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightDampingThreshold ( float )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightImageGamma ( float )
	
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightSpotFactor ( float )
	{ deoglSPBParameter::evtFloat, 1, 1 }, // pLightSpotBase ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1 } // pLightSpotExponent ( float )
};



// Class deoglLightShader
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightShader::deoglLightShader( deoglRenderThread &renderThread, const deoglLightShaderConfig &config ) :
pRenderThread( renderThread )
{
	int i;
	
	pConfig = config;
	
	for( i=0; i<ETT_COUNT; i++ ){
		pTextureTargets[ i ] = -1;
	}
	pUsedTextureTargetCount = 0;
	
	for( i=0; i<EIUT_COUNT; i++ ){
		pInstanceUniformTargets[ i ] = -1;
	}
	pUsedInstanceUniformTargetCount = 0;
	for( i=0; i<ELUT_COUNT; i++ ){
		pLightUniformTargets[ i ] = -1;
	}
	pUsedLightUniformTargetCount = 0;
	
	pSources = NULL;
	pShader = NULL;
}

deoglLightShader::~deoglLightShader(){
	if( pShader ){
		delete pShader;
	}
	if( pSources ){
		delete pSources;
	}
}



// Management
///////////////

int deoglLightShader::GetTextureTarget( eTextureTargets target ) const{
	return pTextureTargets[ target ];
}

void deoglLightShader::SetTextureTarget( eTextureTargets target, int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	pTextureTargets[ target ] = index;
}

void deoglLightShader::SetUsedTextureTargetCount( int usedTextureTargetCount ){
	if( usedTextureTargetCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	pUsedTextureTargetCount = usedTextureTargetCount;
}



int deoglLightShader::GetInstanceUniformTarget( eInstanceUniformTargets target ) const{
	return pInstanceUniformTargets[ target ];
}

void deoglLightShader::SetInstanceUniformTarget( eInstanceUniformTargets target, int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	pInstanceUniformTargets[ target ] = index;
}

int deoglLightShader::GetLightUniformTarget( eLightUniformTargets target ) const{
	return pLightUniformTargets[ target ];
}

void deoglLightShader::SetLightUniformTarget( eLightUniformTargets target, int index ){
	if( index < -1 ){
		DETHROW( deeInvalidParam );
	}
	pLightUniformTargets[ target ] = index;
}



void deoglLightShader::EnsureShaderExists(){
	if( ! pShader ){
		GenerateShader();
	}
}

deoglShaderProgram *deoglLightShader::GetShader(){
	EnsureShaderExists();
	return pShader;
}



deoglSPBlockUBO *deoglLightShader::CreateSPBRender( deoglRenderThread &renderThread ){
	// this shader parameter block will not be optimzed. the layout is always the same
	// no matter what configuration is used for skins. this is also why this method is
	// a static method not an regular method
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *spb = NULL;
	
	try{
		spb = new deoglSPBlockUBO( renderThread );
		spb->SetRowMajor( ! renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		spb->SetParameterCount( ERUT_COUNT );
		
		spb->GetParameterAt( erutPosTransform ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 ); // vec4
		spb->GetParameterAt( erutAOSelfShadow ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		spb->GetParameterAt( erutLumFragCoordScale ).SetAll( deoglSPBParameter::evtFloat, 2, 1, 1 ); // vec2
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglLightShader::eubRenderParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}

deoglSPBlockUBO *deoglLightShader::CreateSPBInstParam() const{
	// this shader parameter block will be optimized. the layout is adapted to
	// the configuration used for this light shader
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *spb = NULL;
	int i, target;
	
	try{
		spb = new deoglSPBlockUBO( pRenderThread );
		spb->SetRowMajor( ! pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		spb->SetParameterCount( pUsedInstanceUniformTargetCount );
		
		for( i=0; i<EIUT_COUNT; i++ ){
			target = pInstanceUniformTargets[ i ];
			
			if( target != -1 ){
				spb->GetParameterAt( target ).SetAll( vInstanceSPBParamDefs[ i ].dataType,
					vInstanceSPBParamDefs[ i ].componentCount,
					vInstanceSPBParamDefs[ i ].vectorCount, 1 );
			}
		}
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglLightShader::eubInstanceParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}

deoglSPBlockUBO *deoglLightShader::CreateSPBLightParam() const{
	// this shader parameter block will be optimized. the layout is adapted to
	// the configuration used for this light shader
	if( ! pglUniformBlockBinding ){
		DETHROW( deeInvalidParam );
	}
	
	deoglSPBlockUBO *spb = NULL;
	int i, target;
	
	try{
		spb = new deoglSPBlockUBO( pRenderThread );
		spb->SetRowMajor( ! pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() );
		spb->SetParameterCount( pUsedLightUniformTargetCount );
		
		for( i=0; i<ELUT_COUNT; i++ ){
			target = pLightUniformTargets[ i ];
			
			if( target != -1 ){
				spb->GetParameterAt( target ).SetAll( vLightSPBParamDefs[ i ].dataType,
					vLightSPBParamDefs[ i ].componentCount,
					vLightSPBParamDefs[ i ].vectorCount, 1 );
			}
		}
		
		spb->MapToStd140();
		spb->SetBindingPoint( deoglLightShader::eubLightParameters );
		
	}catch( const deException & ){
		if( spb ){
			spb->FreeReference();
		}
		throw;
	}
	
	return spb;
}



// Shader Generation
//////////////////////

void deoglLightShader::GenerateShader(){
	deoglShaderManager &smgr = pRenderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines;
	
	if( pShader ){
		delete pShader;
		pShader = NULL;
	}
	if( pSources ){
		delete pSources;
		pSources = NULL;
	}
	
	try{
		pSources = new deoglShaderSources;
		
		pSources->SetVersion( "150" );
		
		GenerateDefines( defines );
		GenerateVertexSC();
		GenerateGeometrySC();
		GenerateFragmentSC();
		
		UpdateTextureTargets();
		UpdateUniformTargets();
		
		InitShaderParameters();
		
		// create shader
		pShader = new deoglShaderProgram( pSources, defines );
		
		// add unit source codes from source files
		if( ! pSources->GetPathVertexSourceCode().IsEmpty() ){
			pShader->SetVertexSourceCode( smgr.GetUnitSourceCodeWithPath( pSources->GetPathVertexSourceCode() ) );
			
			if( ! pShader->GetVertexSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Light Shader: Vertex source '%s' not found",
					pSources->GetPathVertexSourceCode().GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		if( ! pSources->GetPathGeometrySourceCode().IsEmpty() ){
			pShader->SetGeometrySourceCode( smgr.GetUnitSourceCodeWithPath( pSources->GetPathGeometrySourceCode() ) );
			
			if( ! pShader->GetGeometrySourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Light Shader: Geometry source '%s' not found",
					pSources->GetPathGeometrySourceCode().GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		if( ! pSources->GetPathFragmentSourceCode().IsEmpty() ){
			pShader->SetFragmentSourceCode( smgr.GetUnitSourceCodeWithPath( pSources->GetPathFragmentSourceCode() ) );
			
			if( ! pShader->GetFragmentSourceCode() ){
				pRenderThread.GetLogger().LogErrorFormat( "Light Shader: Fragment source '%s' not found",
					pSources->GetPathFragmentSourceCode().GetString() );
				DETHROW( deeInvalidParam );
			}
		}
		
		// compile shader
		pShader->SetCompiled( pRenderThread.GetShader().GetShaderManager().GetLanguage()->CompileShader( *pShader ) );
		/*
		if( pConfig.GetShaderMode() == deoglLightShaderConfig::esmGeometry ){
			const int count = pSources->GetParameterCount();
			int i;
			
			pRenderThread.GetLogger().LogInfo( "Skin Shader: Debug Parameters" );
			for( i=0; i<count; i++ ){
				pRenderThread.GetLogger().LogInfoFormat( "- parameter %i (%s) = %i", i, pSources->GetParameterAt( i ), pShader->GetCompiled()->GetParameterAt( i ) );
			}
		}
		*/
		
	}catch( const deException &e ){
		pRenderThread.GetLogger().LogException( e );
		throw;
	}
}

void deoglLightShader::GenerateDefines( deoglShaderDefines &defines ){
	defines.AddDefine( "HIGH_PRECISION", "1" );
	defines.AddDefine( "HIGHP", "highp" ); // if not supported by GPU medp
	
	if( pglUniformBlockBinding ){
		defines.AddDefine( "UBO", "1" );
		
		if( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Broken() ){
			defines.AddDefine( "UBO_IDMATACCBUG", "1" );
		}
		if( pRenderThread.GetCapabilities().GetUBODirectLinkDeadloop().Broken() ){
			defines.AddDefine( "BUG_UBO_DIRECT_LINK_DEAD_LOOP", "1" );
		}
	}
	
	switch( pConfig.GetLightMode() ){
	case deoglLightShaderConfig::elmPoint:
		defines.AddDefine( "POINT_LIGHT", "1" );
		break;
		
	case deoglLightShaderConfig::elmSpot:
		defines.AddDefine( "SPOT_LIGHT", "1" );
		break;
		
	case deoglLightShaderConfig::elmProjector:
		defines.AddDefine( "PROJECTOR_LIGHT", "1" );
		break;
		
	case deoglLightShaderConfig::elmSky:
		defines.AddDefine( "SKY_LIGHT", "1" );
		break;
		
	case deoglLightShaderConfig::elmParticle:
		defines.AddDefine( "PARTICLE_LIGHT", "1" );
		
		switch( pConfig.GetParticleMode() ){
		case deoglLightShaderConfig::epmRibbon:
			defines.AddDefine( "PARTICLE_RIBBON", "1" );
			break;
			
		case deoglLightShaderConfig::epmBeam:
			defines.AddDefine( "PARTICLE_BEAM", "1" );
			break;
			
		case deoglLightShaderConfig::epmParticle:
			break;
		}
		break;
	}
	
	switch( pConfig.GetMaterialNormalMode() ){
	case deoglLightShaderConfig::emnmIntBasic:
		defines.AddDefine( "MATERIAL_NORMAL_INTBASIC", "1" );
		break;
		
	case deoglLightShaderConfig::emnmSpheremap:
		defines.AddDefine( "MATERIAL_NORMAL_SPHEREMAP", "1" );
		break;
		
	case deoglLightShaderConfig::emnmFloat:
		break;
	}
	
	if( pConfig.GetHWDepthCompare() ){
		defines.AddDefine( "HW_DEPTH_COMPARE", "1" );
	}
	
	if( pConfig.GetTextureNoise() ){
		defines.AddDefine( "TEXTURE_NOISE", "1" );
	}
	if( pConfig.GetTextureColor() ){
		defines.AddDefine( "TEXTURE_COLOR", "1" );
	}
	if( pConfig.GetTextureColorOmnidirCube() ){
		defines.AddDefine( "TEXTURE_COLOR_CUBEMAP", "1" );
	}
	if( pConfig.GetTextureColorOmnidirEquirect() ){
		defines.AddDefine( "TEXTURE_COLOR_EQUIRECT", "1" );
	}
	
	if( pConfig.GetTextureShadow1Solid() ){
		defines.AddDefine( "TEXTURE_SHADOW1_SOLID", "1" );
	}
	if( pConfig.GetTextureShadow1Transparent() ){
		defines.AddDefine( "TEXTURE_SHADOW1_TRANSPARENT", "1" );
	}
	if( pConfig.GetTextureShadow2Solid() ){
		defines.AddDefine( "TEXTURE_SHADOW2_SOLID", "1" );
	}
	if( pConfig.GetTextureShadow2Transparent() ){
		defines.AddDefine( "TEXTURE_SHADOW2_TRANSPARENT", "1" );
	}
	if( pConfig.GetTextureShadowAmbient() ){
		defines.AddDefine( "TEXTURE_SHADOW_AMBIENT", "1" );
	}
	
	if( pConfig.GetDecodeInDepth() ){
		defines.AddDefine( "DECODE_IN_DEPTH", "1" );
	}
	if( pConfig.GetDecodeInShadow() ){
		defines.AddDefine( "DECODE_IN_SHADOW", "1" );
	}
	if( pConfig.GetEncodeOutDepth() ){
		defines.AddDefine( "ENCODE_OUT_DEPTH", "1" );
	}
	if( pConfig.GetShadowMatrix2EqualsMatrix1() ){
		defines.AddDefine( "SHAMAT2_EQUALS_SHAMAT1", "1" );
	}
	if( pConfig.GetShadowInverseDepth() ){
		defines.AddDefine( "SHADOW_INVERSE_DEPTH", "1" );
	}
	if( pConfig.GetAmbientLighting() ){
		defines.AddDefine( "AMBIENT_LIGHTING", "1" );
	}
	if( pConfig.GetFullScreenQuad() ){
		defines.AddDefine( "FULLSCREENQUAD", "1" );
	}
	if( pConfig.GetSubSurface() ){
		defines.AddDefine( "WITH_SUBSURFACE", "1" );
	}
	if( pConfig.GetLuminanceOnly() ){
		defines.AddDefine( "LUMINANCE_ONLY", "1" );
	}
	
	switch( pConfig.GetShadowTapMode() ){
	case deoglLightShaderConfig::estmPcf4:
		defines.AddDefine( "PCF_4TAP", "1" );
		break;
		
	case deoglLightShaderConfig::estmPcf9:
		defines.AddDefine( "PCF_9TAP", "1" );
		break;
		
	case deoglLightShaderConfig::estmPcfVariableTap:
		defines.AddDefine( "PCF_VARTAP", "1" );
		break;
		
	case deoglLightShaderConfig::estmSingle:
		break;
	}
	
	if( pConfig.GetTextureNoise() ){
		defines.AddDefine( "NOISE_TAP", "1" );
	}
	
	// this symbol activates an optimization for shadow casting lights. if set fragments facing
	// away from the light source are not send through shadow taping and are simply considered
	// fully in shadow. if no ambient lighting is used this ends up in a quick discard statement
	//defines.AddDefine( "OPTIMIZE_SHADOW_BACKFACE", "1" );
	
	// Definition of shadow mapping algorithm to use (* = 1 or 2):
	// SMA*_2D: Use 2D texture mapping with sampler2D{Shadow}
	// SMA*_CUBE: Use Cube texture mapping with samplerCube
	// SMA*_DUALPARA: Use dual paraboloid shadow mapping using sampler2D{Shadow}
	// SMA*_PYRAMID: Use pyramid shadow mapping using sampler2D{Shadow}
	switch( pConfig.GetShadowMappingAlgorithm1() ){
	case deoglLightShaderConfig::esma2D:
		defines.AddDefine( "SMA1_2D", "1" );
		break;
		
	case deoglLightShaderConfig::esmaCube:
		defines.AddDefine( "SMA1_CUBE", "1" );
		break;
		
	case deoglLightShaderConfig::esmaDualParaboloid:
		defines.AddDefine( "SMA1_DUALPARA", "1" );
		break;
		
	case deoglLightShaderConfig::esmaPyramid:
		defines.AddDefine( "SMA1_PYRAMID", "1" );
		break;
	}
	
	switch( pConfig.GetShadowMappingAlgorithm2() ){
	case deoglLightShaderConfig::esma2D:
		defines.AddDefine( "SMA2_2D", "1" );
		break;
		
	case deoglLightShaderConfig::esmaCube:
		defines.AddDefine( "SMA2_CUBE", "1" );
		break;
		
	case deoglLightShaderConfig::esmaDualParaboloid:
		defines.AddDefine( "SMA2_DUALPARA", "1" );
		break;
		
	case deoglLightShaderConfig::esmaPyramid:
		defines.AddDefine( "SMA2_PYRAMID", "1" );
		break;
	}
}

void deoglLightShader::GenerateVertexSC(){
	int unitSourceCodePath;
	
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle ){
		unitSourceCodePath = deoglLightShaderManager::euscpVertexParticle;
		
	}else{
		unitSourceCodePath = deoglLightShaderManager::euscpVertexLight;
	}
	
	pSources->SetPathVertexSourceCode( pRenderThread.GetShader().GetLightShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglLightShader::GenerateGeometrySC(){
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle ){
		int unitSourceCodePath;
		
		unitSourceCodePath = deoglLightShaderManager::euscpGeometryParticle;
		
		pSources->SetPathGeometrySourceCode( pRenderThread.GetShader().GetLightShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
	}
}

void deoglLightShader::GenerateFragmentSC(){
	int unitSourceCodePath;
	
	unitSourceCodePath = deoglLightShaderManager::euscpFragmentLight;
	
	pSources->SetPathFragmentSourceCode( pRenderThread.GetShader().GetLightShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglLightShader::UpdateTextureTargets(){
	int textureUnitNumber = 0, i;
	
	for( i=0; i<ETT_COUNT; i++ ){
		pTextureTargets[ i ] = -1;
	}
	pUsedTextureTargetCount = 0;
	
	pTextureTargets[ ettDepth ] = textureUnitNumber++;
	pTextureTargets[ ettDiffuse ] = textureUnitNumber++;
	pTextureTargets[ ettNormal ] = textureUnitNumber++;
	pTextureTargets[ ettReflectivity ] = textureUnitNumber++;
	pTextureTargets[ ettRoughness ] = textureUnitNumber++;
	pTextureTargets[ ettAOSolidity ] = textureUnitNumber++;
	if( pConfig.GetSubSurface() ){
		pTextureTargets[ ettSubSurface ] = textureUnitNumber++;
	}
	
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle ){
		pTextureTargets[ ettSamples ] = textureUnitNumber++;
	}
	
	if( pConfig.GetTextureNoise() ){
		pTextureTargets[ ettNoise ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureColor() ){
		pTextureTargets[ ettColor ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureColorOmnidirCube() ){
		pTextureTargets[ ettColorCubemap ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureColorOmnidirEquirect() ){
		pTextureTargets[ ettColorEquirect ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureShadow1Solid() ){
		pTextureTargets[ ettShadow1SolidDepth ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureShadow1Transparent() ){
		pTextureTargets[ ettShadow1TransparentDepth ] = textureUnitNumber++;
		pTextureTargets[ ettShadow1TransparentColor ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureShadow2Solid() ){
		pTextureTargets[ ettShadow2SolidDepth ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureShadow2Transparent() ){
		pTextureTargets[ ettShadow2TransparentDepth ] = textureUnitNumber++;
		pTextureTargets[ ettShadow2TransparentColor ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureShadowAmbient() ){
		pTextureTargets[ ettShadowAmbient ] = textureUnitNumber++;
	}
	if( pConfig.GetSubSurface() ){
		if( pConfig.GetTextureShadow1Solid() ){
			pTextureTargets[ ettLightDepth1 ] = textureUnitNumber++;
		}
		if( pConfig.GetTextureShadow2Solid() ){
			pTextureTargets[ ettLightDepth2 ] = textureUnitNumber++;
		}
	}
	
	pUsedTextureTargetCount = textureUnitNumber;
}

void deoglLightShader::UpdateUniformTargets(){
	const bool modeParticle = pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle;
	const bool modeProjector = pConfig.GetLightMode() == deoglLightShaderConfig::elmProjector;
	const bool modeSpot = pConfig.GetLightMode() == deoglLightShaderConfig::elmSpot;
	const bool modeSky = pConfig.GetLightMode() == deoglLightShaderConfig::elmSky;
	int i;
	
	for( i=0; i<EIUT_COUNT; i++ ){
		pInstanceUniformTargets[ i ] = -1;
	}
	pUsedInstanceUniformTargetCount = 0;
	for( i=0; i<ELUT_COUNT; i++ ){
		pLightUniformTargets[ i ] = -1;
	}
	pUsedLightUniformTargetCount = 0;
	
	// instance parameters. this block is guaranteed to work across all possible shader
	// configurations for a specific light. thus only parameters which depend on the
	// light configuration are used
	pInstanceUniformTargets[ eiutMatrixMVP ] = pUsedInstanceUniformTargetCount++;
	pInstanceUniformTargets[ eiutMatrixMV ] = pUsedInstanceUniformTargetCount++;
	
	if( modeParticle ){
		pInstanceUniformTargets[ eiutSamplesParams ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutBurstFactor ] = pUsedInstanceUniformTargetCount++;
		
	}else{
		pInstanceUniformTargets[ eiutLightPosition ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutLightView ] = pUsedInstanceUniformTargetCount++;
		
		pInstanceUniformTargets[ eiutShadowMatrix1 ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutShadowMatrix2 ] = pUsedInstanceUniformTargetCount++;
		
		if( modeSky ){
			pInstanceUniformTargets[ eiutShadowMatrix3 ] = pUsedInstanceUniformTargetCount++;
			pInstanceUniformTargets[ eiutShadowMatrix4 ] = pUsedInstanceUniformTargetCount++;
			pInstanceUniformTargets[ eiutLayerBorder ] = pUsedInstanceUniformTargetCount++;
		}
		
		if( pConfig.GetTextureColorOmnidirCube() || pConfig.GetTextureColorOmnidirEquirect() ){
			pInstanceUniformTargets[ eiutLightImageOmniMatrix ] = pUsedInstanceUniformTargetCount++;
		}
		
		pInstanceUniformTargets[ eiutShadow1Solid ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutShadow1Transparent ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutShadow2Solid ] = pUsedInstanceUniformTargetCount++;
		pInstanceUniformTargets[ eiutShadow2Transparent ] = pUsedInstanceUniformTargetCount++;
		
		pInstanceUniformTargets[ eiutShadowDepthTransform ] = pUsedInstanceUniformTargetCount++;
		
		if( modeSky ){
			pInstanceUniformTargets[ eiutShadowDepthTransform2 ] = pUsedInstanceUniformTargetCount++;
		}
	}
	
	// light parameters. this block is guaranteed to work across all possible shader
	// configurations for a specific light. thus only parameters which depend on the
	// light configuration are used
	
	if( ! modeParticle ){
		pLightUniformTargets[ elutLightColor ] = pUsedLightUniformTargetCount++;
		pLightUniformTargets[ elutLightRange ] = pUsedLightUniformTargetCount++;
		pLightUniformTargets[ elutLightColorAmbient ] = pUsedLightUniformTargetCount++;
		pLightUniformTargets[ elutLightAmbientRatio ] = pUsedLightUniformTargetCount++;
	}
	
	pLightUniformTargets[ elutLightAttenuationCoefficient ] = pUsedLightUniformTargetCount++;
	pLightUniformTargets[ elutLightDampingCoefficient ] = pUsedLightUniformTargetCount++;
	pLightUniformTargets[ elutLightDampingThreshold ] = pUsedLightUniformTargetCount++;
	
	pLightUniformTargets[ elutLightImageGamma ] = pUsedLightUniformTargetCount++;
	
	if( modeSpot || modeProjector ){
		pLightUniformTargets[ elutLightSpotFactor ] = pUsedLightUniformTargetCount++;
		pLightUniformTargets[ elutLightSpotBase ] = pUsedLightUniformTargetCount++;
		pLightUniformTargets[ elutLightSpotExponent ] = pUsedLightUniformTargetCount++;
	}
}

void deoglLightShader::InitShaderParameters(){
	deoglShaderBindingList &uniformBlockList = pSources->GetUniformBlockList();
	deoglShaderBindingList &textureList = pSources->GetTextureList();
	deoglShaderBindingList &inputList = pSources->GetAttributeList();
	deoglShaderBindingList &outputList = pSources->GetOutputList();
	decStringList &parameterList = pSources->GetParameterList();
	int i;
	
	// texture targets
	for( i=0; i<ETT_COUNT; i++ ){
		if( pTextureTargets[ i ] != -1 ){
			textureList.Add( vTextureTargetNames[ i ], pTextureTargets[ i ] );
		}
	}
	
	// uniforms
	parameterList.Add( "pPosTransform" ); // erutPosTransform
	
	for( i=0; i<EIUT_COUNT; i++ ){
		if( pInstanceUniformTargets[ i ] != -1 ){
			parameterList.Add( vInstanceUniformTargetNames[ i ] );
		}
	}
	
	for( i=0; i<ELUT_COUNT; i++ ){
		if( pLightUniformTargets[ i ] != -1 ){
			parameterList.Add( vLightUniformTargetNames[ i ] );
		}
	}
	
	// inputs
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle ){
		inputList.Add( "inParticle0", 0 );
		inputList.Add( "inParticle1", 1 );
		inputList.Add( "inParticle2", 2 ); // unused but does not hurt to define it
		inputList.Add( "inParticle3", 3 );
		/*
		if( pConfig.GetParticleMode() == deoglLightShaderConfig::epmBeam ){
			inputList.Add( "inParticle4", 4 );
		}
		*/
		
	}else{
		inputList.Add( "inPosition", 0 );
	}
	
	// outputs
	outputList.Add( "outColor", 0 );
	if( pConfig.GetSubSurface() ){
		outputList.Add( "outSubSurface", 1 );
	}
	
	// uniform blocks
	uniformBlockList.Add( "RenderParameters", eubRenderParameters );
	uniformBlockList.Add( "InstanceParameters", eubInstanceParameters );
	uniformBlockList.Add( "LightParameters", eubLightParameters );
}
