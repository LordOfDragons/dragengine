/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
#include "../../texture/deoglTextureStageManager.h"

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
	"texShadow1Ambient", // ettShadow1Ambient
	"texShadow2Ambient", // ettShadow2Ambient
	"texLightDepth1", // ettLightDepth1
	"texLightDepth2", // ettLightDepth2
	"texPosition", // ettPosition
	"texOTOcclusion", // ettOTOcclusion
	"texOTDistance" // ettOTDistance
};

static const char *vInstanceUniformTargetNames[ deoglLightShader::EIUT_COUNT ] = {
	"pMatrixMVP", // eiutMatrixMVP
	"pMatrixMV", // eiutMatrixMV
	
	"pSamplesParams", // eiutSamplesParams
	"pBurstFactor", // eiutBurstFactor
	
	"pLightPosition", // eiutLightPosition
	"pLightView", // eiutLightView
	"pLightParams", // eiutLightParams
	
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
	"pShadowDepthTransform2", // eiutShadowDepthTransform2
	
	"pGIShadowMatrix", // eiutGIShadowMatrix
	"pGIShadowParams" // eiutGIShadowParams
};

static const char *vLightUniformTargetNames[ deoglLightShader::ELUT_COUNT ] = {
	"pLightColor", // elutLightColor
	"pLightRange", // elutLightRange
	"pLightColorAmbient", // elutLightColorAmbient
	"pLightAmbientRatio", // elutLightAmbientRatio
	"pLightGIAmbientIntensity", // elutLightGIAmbientIntensity
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
	int arrayCount;
};

static const sSPBParameterDefinition vInstanceSPBParamDefs[ deoglLightShader::EIUT_COUNT ] = {
	{ deoglSPBParameter::evtFloat, 4, 4, 2 }, // pMatrixMVP ( mat4[2] )
	{ deoglSPBParameter::evtFloat, 4, 3, 2 }, // pMatrixMV ( mat4x3[2] )
	
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // pSamplesParams ( vec4 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pBurstFactor ( float )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 2 }, // pLightPosition ( vec3[2] )
	{ deoglSPBParameter::evtFloat, 3, 1, 2 }, // pLightView ( vec3[2] )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // pLightParams ( vec4 )
	
	{ deoglSPBParameter::evtFloat, 4, 4, 2 }, // pShadowMatrix ( mat4[2] )
	{ deoglSPBParameter::evtFloat, 4, 4, 2 }, // pShadowMatrix2 ( mat4[2] )
	{ deoglSPBParameter::evtFloat, 4, 4, 2 }, // pShadowMatrix3 ( mat4[2] )
	{ deoglSPBParameter::evtFloat, 4, 4, 2 }, // pShadowMatrix4 ( mat4[2] )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // pLayerBorder ( vec4 )
	
	{ deoglSPBParameter::evtFloat, 4, 3, 2 }, // pLightImageOmniMatrix ( mat4x3[2] )
	
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pShadow1Solid ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pShadow1Transparent ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pShadow2Solid ( vec3 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pShadow2Transparent ( vec3 )
	
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // pShadowDepthTransform ( vec4 )
	{ deoglSPBParameter::evtFloat, 4, 1, 1 }, // pShadowDepthTransform2 ( vec4 )
	
	{ deoglSPBParameter::evtFloat, 4, 4, 1 }, // pGIShadowMatrix ( mat4 )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 } // pGIShadowParams ( vec3 )
};

static const sSPBParameterDefinition vLightSPBParamDefs[ deoglLightShader::ELUT_COUNT ] = {
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pLightColor ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightRange ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pLightColorAmbient ( vec3 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightAmbientRatio ( float )
	{ deoglSPBParameter::evtFloat, 3, 1, 1 }, // pLightColorAmbientGI ( vec3 )
	
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightAttenuationCoefficient ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightDampingCoefficient ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightDampingThreshold ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightImageGamma ( float )
	
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightSpotFactor ( float )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 }, // pLightSpotBase ( vec2 )
	{ deoglSPBParameter::evtFloat, 1, 1, 1 } // pLightSpotExponent ( float )
};

// cache revision. if skin config or skin unit sources change in any way increment this
// value to make sure existing caches are invalidate
#define SHADER_CACHE_REVISION 1


// Class deoglLightShader::cPrepareShader
///////////////////////////////////////////

deoglLightShader::cPrepareShader::cPrepareShader(deoglLightShader &shader,
	cShaderPreparedListener *listener) :
pShader(shader),
pListener(listener)
{
	DEASSERT_NOTNULL(listener)
}

void deoglLightShader::cPrepareShader::CompileFinished(deoglShaderProgram *program){
	if(program && program->ready){
		pShader.pShader->MoveCompiled(*program);
		pShader.pShader->ready = true;
	}
	
	try{
		pListener->PrepareShaderFinished(pShader);
		
	}catch(const deException &e){
		pShader.GetRenderThread().GetLogger().LogException(e);
	}
	delete pListener;
}


// Class deoglLightShader
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightShader::deoglLightShader( deoglRenderThread &renderThread, const deoglLightShaderConfig &config ) :
pRenderThread( renderThread ),
pConfig( config )
{
	int i;
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
}

deoglLightShader::~deoglLightShader(){
}



// Management
///////////////

int deoglLightShader::GetTextureTarget( eTextureTargets target ) const{
	return pTextureTargets[ target ];
}

void deoglLightShader::SetTextureTarget( eTextureTargets target, int index ){
	DEASSERT_TRUE( index >= -1 )
	pTextureTargets[ target ] = index;
}

void deoglLightShader::SetUsedTextureTargetCount( int usedTextureTargetCount ){
	DEASSERT_TRUE( usedTextureTargetCount >= 0 )
	pUsedTextureTargetCount = usedTextureTargetCount;
}



int deoglLightShader::GetInstanceUniformTarget( eInstanceUniformTargets target ) const{
	return pInstanceUniformTargets[ target ];
}

void deoglLightShader::SetInstanceUniformTarget( eInstanceUniformTargets target, int index ){
	DEASSERT_TRUE( index >= -1 )
	pInstanceUniformTargets[ target ] = index;
}

int deoglLightShader::GetLightUniformTarget( eLightUniformTargets target ) const{
	return pLightUniformTargets[ target ];
}

void deoglLightShader::SetLightUniformTarget( eLightUniformTargets target, int index ){
	DEASSERT_TRUE( index >= -1 )
	pLightUniformTargets[ target ] = index;
}



void deoglLightShader::PrepareShader(cShaderPreparedListener *listener){
	if(listener){
		GenerateShader(listener);
		
	}else{
		if(!pShader){
			GenerateShader(nullptr);
		}
	}
}



deoglSPBlockUBO::Ref deoglLightShader::CreateSPBInstParam() const{
	const deoglSPBlockUBO::Ref spb(deoglSPBlockUBO::Ref::New(new deoglSPBlockUBO(pRenderThread)));
	spb->SetRowMajor(pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
	spb->SetCompact(false);
	spb->SetParameterCount(EIUT_COUNT);
	
	int i;
	for(i=0; i<EIUT_COUNT; i++){
		const sSPBParameterDefinition &pdef = vInstanceSPBParamDefs[i];
		spb->GetParameterAt(i).SetAll(pdef.dataType, pdef.componentCount, pdef.vectorCount, pdef.arrayCount);
	}
	
	spb->MapToStd140();
	spb->SetBindingPoint(deoglLightShader::eubInstanceParameters);
	return spb;
}

deoglSPBlockUBO::Ref deoglLightShader::CreateSPBLightParam() const{
	// this shader parameter block will be optimized. the layout is adapted to
	// the configuration used for this light shader
	const deoglSPBlockUBO::Ref spb( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( pRenderThread ) ) );
	spb->SetRowMajor(pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
	spb->SetParameterCount( pUsedLightUniformTargetCount );
	
	int i;
	for( i=0; i<ELUT_COUNT; i++ ){
		const int target = pLightUniformTargets[ i ];
		if( target != -1 ){
			spb->GetParameterAt( target ).SetAll(
				vLightSPBParamDefs[ i ].dataType, vLightSPBParamDefs[ i ].componentCount,
				vLightSPBParamDefs[ i ].vectorCount, vLightSPBParamDefs[ i ].arrayCount );
		}
	}
	
	spb->MapToStd140();
	spb->SetBindingPoint( deoglLightShader::eubLightParameters );
	return spb;
}

deoglSPBlockUBO::Ref deoglLightShader::CreateSPBOccQueryParam( deoglRenderThread &renderThread ){
	const deoglSPBlockUBO::Ref spb( deoglSPBlockUBO::Ref::New( new deoglSPBlockUBO( renderThread ) ) );
	spb->SetRowMajor(renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working());
	spb->SetParameterCount( 1 );
	spb->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 3, 1 );
	spb->MapToStd140();
	spb->SetBindingPoint( 2 ); // see occmap.shader.xml
	return spb;
}



// Shader Generation
//////////////////////

void deoglLightShader::GenerateShader(cShaderPreparedListener *listener){
	deoglShaderManager &smgr = pRenderThread.GetShader().GetShaderManager();
	deoglShaderDefines defines;
	
	pShader = nullptr;
	pSources = nullptr;
	
	try{
		pSources.TakeOver( new deoglShaderSources );
		
		GenerateDefines( defines );
		GenerateVertexSC();
		GenerateGeometrySC();
		GenerateFragmentSC();
		
		UpdateTextureTargets();
		UpdateUniformTargets();
		
		InitShaderParameters();
		
		// create shader
		pShader.TakeOver(new deoglShaderProgram(pRenderThread, pSources, defines));
		smgr.ResolveProgramUnits(pShader);
		
		// cache id
		decStringList cacheIdParts, cacheIdComponents;
		decString cacheIdFormat;
		
		cacheIdFormat.Format( "light%d", SHADER_CACHE_REVISION );
		cacheIdParts.Add( cacheIdFormat );
		
		cacheIdFormat.Format( "%x,%x,%x", pConfig.GetKey1(), pConfig.GetKey2(), pConfig.GetKey3() );
		cacheIdParts.Add( cacheIdFormat );
		
		cacheIdComponents.Add( pSources->GetPathVertexSourceCode() );
		cacheIdComponents.Add( pSources->GetPathGeometrySourceCode() );
		cacheIdComponents.Add( pSources->GetPathFragmentSourceCode() );
		cacheIdParts.Add( cacheIdComponents.Join( "," ) );
		cacheIdComponents.RemoveAll();
		
		cacheIdParts.Add( defines.CalcCacheId() );
		
		pShader->SetCacheId( cacheIdParts.Join( ";" ) );
		
		// compile shader
		if(listener){
			smgr.GetLanguage()->CompileShaderAsync(pShader, new cPrepareShader(*this, listener));
			
		}else{
			smgr.GetLanguage()->CompileShader(pShader);
		}
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
	pRenderThread.GetShader().SetCommonDefines( defines );
	
	switch( pConfig.GetLightMode() ){
	case deoglLightShaderConfig::elmPoint:
		defines.SetDefines( "POINT_LIGHT" );
		break;
		
	case deoglLightShaderConfig::elmSpot:
		defines.SetDefines( "SPOT_LIGHT" );
		break;
		
	case deoglLightShaderConfig::elmProjector:
		defines.SetDefines( "PROJECTOR_LIGHT" );
		break;
		
	case deoglLightShaderConfig::elmSky:
		defines.SetDefines( "SKY_LIGHT" );
		break;
		
	case deoglLightShaderConfig::elmParticle:
		defines.SetDefines( "PARTICLE_LIGHT" );
		
		switch( pConfig.GetParticleMode() ){
		case deoglLightShaderConfig::epmRibbon:
			defines.SetDefines( "PARTICLE_RIBBON" );
			break;
			
		case deoglLightShaderConfig::epmBeam:
			defines.SetDefines( "PARTICLE_BEAM" );
			break;
			
		case deoglLightShaderConfig::epmParticle:
			break;
		}
		break;
	}
	
	switch( pConfig.GetMaterialNormalModeDec() ){
	case deoglLightShaderConfig::emnmIntBasic:
		defines.SetDefines( "MATERIAL_NORMAL_DEC_INTBASIC" );
		break;
		
	case deoglLightShaderConfig::emnmFloatBasic:
		defines.SetDefines( "MATERIAL_NORMAL_DEC_FLOATBASIC" );
		break;
		
	case deoglLightShaderConfig::emnmSpheremap:
		defines.SetDefines( "MATERIAL_NORMAL_DEC_SPHEREMAP" );
		break;
		
	case deoglLightShaderConfig::emnmFloat:
		break;
	}
	
	switch( pConfig.GetMaterialNormalModeEnc() ){
	case deoglLightShaderConfig::emnmIntBasic:
		defines.SetDefines( "MATERIAL_NORMAL_ENC_INTBASIC" );
		break;
		
	case deoglLightShaderConfig::emnmFloatBasic:
		defines.SetDefines( "MATERIAL_NORMAL_ENC_FLOATBASIC" );
		break;
		
	case deoglLightShaderConfig::emnmSpheremap:
		defines.SetDefines( "MATERIAL_NORMAL_ENC_SPHEREMAP" );
		break;
		
	case deoglLightShaderConfig::emnmFloat:
		break;
	}
	
	if( pConfig.GetHWDepthCompare() ){
		defines.SetDefines( "HW_DEPTH_COMPARE" );
	}
	
	if( pConfig.GetTextureNoise() ){
		defines.SetDefines( "TEXTURE_NOISE" );
	}
	if( pConfig.GetTextureColor() ){
		defines.SetDefines( "TEXTURE_COLOR" );
	}
	if( pConfig.GetTextureColorOmnidirCube() ){
		defines.SetDefines( "TEXTURE_COLOR_CUBEMAP" );
	}
	if( pConfig.GetTextureColorOmnidirEquirect() ){
		defines.SetDefines( "TEXTURE_COLOR_EQUIRECT" );
	}
	
	if( pConfig.GetTextureShadow1Solid() ){
		defines.SetDefines( "TEXTURE_SHADOW1_SOLID" );
	}
	if( pConfig.GetTextureShadow1Transparent() ){
		defines.SetDefines( "TEXTURE_SHADOW1_TRANSPARENT" );
	}
	if( pConfig.GetTextureShadow2Solid() ){
		defines.SetDefines( "TEXTURE_SHADOW2_SOLID" );
	}
	if( pConfig.GetTextureShadow2Transparent() ){
		defines.SetDefines( "TEXTURE_SHADOW2_TRANSPARENT" );
	}
	if( pConfig.GetTextureShadow1Ambient() ){
		defines.SetDefines( "TEXTURE_SHADOW1_AMBIENT" );
	}
	if( pConfig.GetTextureShadow2Ambient() ){
		defines.SetDefines( "TEXTURE_SHADOW2_AMBIENT" );
	}
	
	if( pConfig.GetDecodeInDepth() ){
		defines.SetDefines( "DECODE_IN_DEPTH" );
	}
	if( pConfig.GetDecodeInShadow() ){
		defines.SetDefines( "DECODE_IN_SHADOW" );
	}
	if( pConfig.GetEncodeOutDepth() ){
		defines.SetDefines( "ENCODE_OUT_DEPTH" );
	}
	if( pConfig.GetShadowMatrix2EqualsMatrix1() ){
		defines.SetDefines( "SHAMAT2_EQUALS_SHAMAT1" );
	}
	if( pConfig.GetShadowInverseDepth() ){
		defines.SetDefines( "SHADOW_INVERSE_DEPTH" );
	}
	if( pConfig.GetAmbientLighting() ){
		defines.SetDefines( "AMBIENT_LIGHTING" );
	}
	if( pConfig.GetFullScreenQuad() ){
		defines.SetDefines( "FULLSCREENQUAD" );
	}
	if( pConfig.GetSubSurface() ){
		defines.SetDefines( "WITH_SUBSURFACE" );
	}
	if( pConfig.GetLuminanceOnly() ){
		defines.SetDefines( "LUMINANCE_ONLY" );
	}
	if( pConfig.GetGIRay() ){
		defines.SetDefines( "GI_RAY" );
	}
	if( pConfig.GetGSRenderStereo() ){
		defines.SetDefines( "GS_RENDER_STEREO" );
	}
	if( pConfig.GetVSRenderStereo() ){
		defines.SetDefines( "VS_RENDER_STEREO" );
	}
	
	switch( pConfig.GetShadowTapMode() ){
	case deoglLightShaderConfig::estmPcf4:
		defines.SetDefines( "PCF_4TAP" );
		break;
		
	case deoglLightShaderConfig::estmPcf9:
		defines.SetDefines( "PCF_9TAP" );
		break;
		
	case deoglLightShaderConfig::estmPcfVariableTap:
		defines.SetDefines( "PCF_VARTAP" );
		break;
		
	case deoglLightShaderConfig::estmSingle:
		break;
	}
	
	if( pConfig.GetTextureNoise() ){
		defines.SetDefines( "NOISE_TAP" );
	}
	
	// this symbol activates an optimization for shadow casting lights. if set fragments facing
	// away from the light source are not send through shadow taping and are simply considered
	// fully in shadow. if no ambient lighting is used this ends up in a quick discard statement
	//defines.SetDefine( "OPTIMIZE_SHADOW_BACKFACE", true );
	
	// Definition of shadow mapping algorithm to use (* = 1 or 2):
	// SMA*_2D: Use 2D texture mapping with sampler2D{Shadow}
	// SMA*_CUBE: Use Cube texture mapping with samplerCube
	// SMA*_DUALPARA: Use dual paraboloid shadow mapping using sampler2D{Shadow}
	// SMA*_PYRAMID: Use pyramid shadow mapping using sampler2D{Shadow}
	switch( pConfig.GetShadowMappingAlgorithm1() ){
	case deoglLightShaderConfig::esma2D:
		defines.SetDefines( "SMA1_2D" );
		break;
		
	case deoglLightShaderConfig::esmaCube:
		defines.SetDefines( "SMA1_CUBE" );
		break;
		
	case deoglLightShaderConfig::esmaDualParaboloid:
		defines.SetDefines( "SMA1_DUALPARA" );
		break;
		
	case deoglLightShaderConfig::esmaPyramid:
		defines.SetDefines( "SMA1_PYRAMID" );
		break;
	}
	
	switch( pConfig.GetShadowMappingAlgorithm2() ){
	case deoglLightShaderConfig::esma2D:
		defines.SetDefines( "SMA2_2D" );
		break;
		
	case deoglLightShaderConfig::esmaCube:
		defines.SetDefines( "SMA2_CUBE" );
		break;
		
	case deoglLightShaderConfig::esmaDualParaboloid:
		defines.SetDefines( "SMA2_DUALPARA" );
		break;
		
	case deoglLightShaderConfig::esmaPyramid:
		defines.SetDefines( "SMA2_PYRAMID" );
		break;
	}
	
	// occlusion tracing
	#ifdef ENABLE_OCCTRACING
	defines.SetDefines( "ENABLE_OCCTRACING" );
	#endif
}

void deoglLightShader::GenerateVertexSC(){
	int unitSourceCodePath;
	
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle ){
		unitSourceCodePath = deoglLightShaderManager::euscpVertexParticle;
		
	}else{
		unitSourceCodePath = deoglLightShaderManager::euscpVertexLight;
	}
	
	pSources->SetPathVertexSourceCode( pRenderThread.GetShader().
		GetLightShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglLightShader::GenerateGeometrySC(){
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmParticle ){
		pSources->SetPathGeometrySourceCode( pRenderThread.GetShader().GetLightShaderManager().
			GetUnitSourceCodePath( deoglLightShaderManager::euscpGeometryParticle ) );
		
	}else if( pConfig.GetGSRenderStereo() ){
		pSources->SetPathGeometrySourceCode( pRenderThread.GetShader().GetLightShaderManager().
			GetUnitSourceCodePath( deoglLightShaderManager::euscpGeometryStereo ) );
	}
}

void deoglLightShader::GenerateFragmentSC(){
	int unitSourceCodePath;
	
	unitSourceCodePath = deoglLightShaderManager::euscpFragmentLight;
	
	pSources->SetPathFragmentSourceCode( pRenderThread.GetShader().
		GetLightShaderManager().GetUnitSourceCodePath( unitSourceCodePath ) );
}

void deoglLightShader::UpdateTextureTargets(){
	int textureUnitNumber = 0, i;
	
	for( i=0; i<ETT_COUNT; i++ ){
		pTextureTargets[ i ] = -1;
	}
	pUsedTextureTargetCount = 0;
	
	if( pConfig.GetGIRay() ){
		pTextureTargets[ ettPosition ] = textureUnitNumber++;
		pTextureTargets[ ettDiffuse ] = textureUnitNumber++;
		pTextureTargets[ ettNormal ] = textureUnitNumber++;
		pTextureTargets[ ettReflectivity ] = textureUnitNumber++;
		pTextureTargets[ ettRoughness ] = textureUnitNumber++;
		
	}else{
		pTextureTargets[ ettDepth ] = textureUnitNumber++;
		pTextureTargets[ ettDiffuse ] = textureUnitNumber++;
		pTextureTargets[ ettNormal ] = textureUnitNumber++;
		pTextureTargets[ ettReflectivity ] = textureUnitNumber++;
		pTextureTargets[ ettRoughness ] = textureUnitNumber++;
		pTextureTargets[ ettAOSolidity ] = textureUnitNumber++;
	}
	
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
	if( pConfig.GetTextureShadow1Ambient() ){
		pTextureTargets[ ettShadow1Ambient ] = textureUnitNumber++;
	}
	if( pConfig.GetTextureShadow2Ambient() ){
		pTextureTargets[ ettShadow2Ambient ] = textureUnitNumber++;
	}
	if( pConfig.GetSubSurface() ){
		if( pConfig.GetTextureShadow1Solid() ){
			pTextureTargets[ ettLightDepth1 ] = textureUnitNumber++;
		}
		if( pConfig.GetTextureShadow2Solid() ){
			pTextureTargets[ ettLightDepth2 ] = textureUnitNumber++;
		}
	}
	
	if( pConfig.GetLightMode() == deoglLightShaderConfig::elmSky ){
		pTextureTargets[ ettOTOcclusion ] = textureUnitNumber++;
		pTextureTargets[ ettOTDistance ] = textureUnitNumber++;
	}
	
	pUsedTextureTargetCount = textureUnitNumber;
	
	if( pUsedTextureTargetCount > OGL_MAX_TEXTURE_STAGES ){
		decString string;
		pConfig.DebugGetConfigString( string );
		pRenderThread.GetLogger().LogErrorFormat( "Too many texture targets: %d", pUsedTextureTargetCount );
		pRenderThread.GetLogger().LogError( string );
		// DETHROW_INFO( deeInvalidAction, "Too many texture targets" );
	}
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
		pInstanceUniformTargets[ eiutLightParams ] = pUsedInstanceUniformTargetCount++;
		
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
		
		if( modeSky ){
			pInstanceUniformTargets[ eiutGIShadowMatrix ] = pUsedInstanceUniformTargetCount++;
			pInstanceUniformTargets[ eiutGIShadowParams ] = pUsedInstanceUniformTargetCount++;
		}
	}
	
	// shared parameter block support. re-map since order is fixed
	for(i=0; i<EIUT_COUNT; i++){
		if(pInstanceUniformTargets[i] != -1){
			pInstanceUniformTargets[i] = i;
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
		pLightUniformTargets[ elutLightColorAmbientGI ] = pUsedLightUniformTargetCount++;
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
	deoglShaderBindingList &textureList = pSources->GetTextureList();
	decStringList &parameterList = pSources->GetParameterList();
	int i;
	
	// texture targets
	for( i=0; i<ETT_COUNT; i++ ){
		if( pTextureTargets[ i ] != -1 ){
			textureList.Add( vTextureTargetNames[ i ], pTextureTargets[ i ] );
		}
	}
	
	// uniforms
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
}
