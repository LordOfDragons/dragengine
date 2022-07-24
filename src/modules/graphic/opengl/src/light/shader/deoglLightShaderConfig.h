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

#ifndef _DEOGLLIGHTSHADERCONFIG_H_
#define _DEOGLLIGHTSHADERCONFIG_H_

class decString;



/**
 * Light Shader Config.
 */
class deoglLightShaderConfig{
public:
	/** Light modes. */
	enum eLightModes{
		/** Point light mode. */
		elmPoint,
		
		/** Spot light mode. */
		elmSpot,
		
		/** Projector light mode. */
		elmProjector,
		
		/** Sky light mode. */
		elmSky,
		
		/** Particle light mode. */
		elmParticle
	};
	
	/** Shadow tap mode. */
	enum eShadowTapModes{
		/** Single tap. */
		estmSingle,
		
		/** PCF 4-tap. */
		estmPcf4,
		
		/** PCF 9-tap. */
		estmPcf9,
		
		/** PCF with depth distance based variable tap width. */
		estmPcfVariableTap
	};
	
	/** Shadow mapping algorithm. */
	enum eShadowMappingAlgorithms{
		/** 2D texture mapping. */
		esma2D,
		
		/** Cube mapping. */
		esmaCube,
		
		/** Dual paraboloid shadow mapping. */
		esmaDualParaboloid,
		
		/** Pyramid shadow mapping. */
		esmaPyramid
	};
	
	/** Material normal modes. */
	enum eMaterialNormalModes{
		/** Float mode for 3-component float textures. */
		emnmFloat,
		
		/** Basic encoding for 3-component int textures. */
		emnmIntBasic,
		
		/** Basic encoding for 3-component float textures. */
		emnmFloatBasic,
		
		/** Spheremap encoding for fitting normals into a 2-component int texture. */
		emnmSpheremap
	};
	
	/** Particle mode. */
	enum eParticleModes{
		/** Particle particle mode. */
		epmParticle,
		
		/** Ribbon particle mode. */
		epmRibbon,
		
		/** Beam particle mode. */
		epmBeam
	};
	
	
	
public:
	eLightModes pLightMode;
	eShadowTapModes pShadowTapMode;
	eShadowMappingAlgorithms pShadowMappingAlgorithm1;
	eShadowMappingAlgorithms pShadowMappingAlgorithm2;
	eMaterialNormalModes pMaterialNormalModeDec;
	eMaterialNormalModes pMaterialNormalModeEnc;
	eParticleModes pParticleMode;
	
	bool pDecodeInDepth;
	bool pDecodeInShadow;
	bool pEncodeOutDepth;
	bool pHWDepthCompare;
	bool pShadowMatrix2EqualsMatrix1;
	bool pShadowInverseDepth;
	bool pAmbientLighting;
	bool pFullScreenQuad;
	bool pSubSurface;
	bool pLuminanceOnly;
	bool pGIRay;
	bool pRenderStereo;
	
	bool pTextureNoise;
	bool pTextureColor;
	bool pTextureColorOmniCube;
	bool pTextureColorOmniEquirect;
	bool pTextureShadow1Solid;
	bool pTextureShadow1Transparent;
	bool pTextureShadow2Solid;
	bool pTextureShadow2Transparent;
	bool pTextureShadow1Ambient;
	bool pTextureShadow2Ambient;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light shader configuration. */
	deoglLightShaderConfig();
	
	/** Clean up light shader configuration. */
	~deoglLightShaderConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset configuration. */
	void Reset();
	
	
	
	/** Light mode. */
	inline eLightModes GetLightMode() const{ return pLightMode; }
	
	/** Set light mode. */
	void SetLightMode( eLightModes lightMode );
	
	/** Shadow tap mode. */
	inline eShadowTapModes GetShadowTapMode() const{ return pShadowTapMode; }
	
	/** Set shadow tap mode. */
	void SetShadowTapMode( eShadowTapModes shadowTapMode );
	
	/** Shadow mapping algorithm for the first texture set. */
	inline eShadowMappingAlgorithms GetShadowMappingAlgorithm1() const{
		return pShadowMappingAlgorithm1; }
	
	/** Set shadow mapping algorithm for the first texture set. */
	void SetShadowMappingAlgorithm1( eShadowMappingAlgorithms shadowMappingAlgorithm );
	
	/** Shadow mapping algorithm for the second texture set. */
	inline eShadowMappingAlgorithms GetShadowMappingAlgorithm2() const{
		return pShadowMappingAlgorithm2; }
	
	/** Set shadow mapping algorithm for the second texture set. */
	void SetShadowMappingAlgorithm2( eShadowMappingAlgorithms shadowMappingAlgorithm );
	
	/** Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalModeDec() const{ return pMaterialNormalModeDec; }
	
	/** Set material normal mode. */
	void SetMaterialNormalModeDec( eMaterialNormalModes materialNormalMode );
	
	/** Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalModeEnc() const{ return pMaterialNormalModeEnc; }
	
	/** Set material normal mode. */
	void SetMaterialNormalModeEnc( eMaterialNormalModes materialNormalMode );
	
	/** Particle mode. */
	inline eParticleModes GetParticleMode() const{ return pParticleMode; }
	
	/** Set particle mode. */
	void SetParticleMode( eParticleModes mode );
	
	
	
	/** Input depth has to be decoded. */
	inline bool GetDecodeInDepth() const{ return pDecodeInDepth; }
	
	/** Set if input depth has to be decoded. */
	void SetDecodeInDepth( bool decodeInDepth );
	
	/** Input shadow has to be decoded. */
	inline bool GetDecodeInShadow() const{ return pDecodeInShadow; }
	
	/** Set if input shadow has to be decoded. */
	void SetDecodeInShadow( bool decodeInShadow );
	
	/** Output depth has to be encoded. */
	inline bool GetEncodeOutDepth() const{ return pEncodeOutDepth; }
	
	/** Set if output depth has to be encoded. */
	void SetEncodeOutDepth( bool encodeOutDepth );
	
	/** Hardware depth comparison is used. */
	inline bool GetHWDepthCompare() const{ return pHWDepthCompare; }
	
	/** Set hardware depth comparison is used. */
	void SetHWDepthCompare( bool hwDepthCompare );
	
	/** Second shadow matrix equals the first one. */
	inline bool GetShadowMatrix2EqualsMatrix1() const{ return pShadowMatrix2EqualsMatrix1; }
	
	/** Set if second shadow matrix equals the first one. */
	void SetShadowMatrix2EqualsMatrix1( bool shadowMatrix2EqualsMatrix1 );
	
	/** Use inverse depth for shadow maps. */
	inline bool GetShadowInverseDepth() const{ return pShadowInverseDepth; }
	
	/** Set if inverse depth is used for shadow maps. */
	void SetShadowInverseDepth( bool shadowInverseDepth );
	
	/** Ambient lighting has to be used. */
	inline bool GetAmbientLighting() const{ return pAmbientLighting; }
	
	/** Set ambient lighting has to be used. */
	void SetAmbientLighting( bool ambientLighting );
	
	/** Full screen quad rendering has to be used. */
	inline bool GetFullScreenQuad() const{ return pFullScreenQuad; }
	
	/** Set if full screen quad rendering has to be used. */
	void SetFullScreenQuad( bool fullScreenQuad );
	
	/** Sub-surface separation has to be used. */
	inline bool GetSubSurface() const{ return pSubSurface; }
	
	/** Set if sub-surface separation has to be used. */
	void SetSubSurface( bool subSurface );
	
	/** Luminance only. */
	inline bool GetLuminanceOnly() const{ return pLuminanceOnly; }
	
	/** Set luminance only. */
	void SetLuminanceOnly( bool luminanceOnly );
	
	/** GI ray. */
	inline bool GetGIRay() const{ return pGIRay; }
	
	/** Set GI ray. */
	void SetGIRay( bool giRay );
	
	/** Render stereo. */
	inline bool GetRenderStereo() const{ return pRenderStereo; }
	
	/** Set render stereo. */
	void SetRenderStereo( bool renderStereo );
	
	
	
	/** Noise texture is used. */
	inline bool GetTextureNoise() const{ return pTextureNoise; }
	
	/** Set if noise texture is used. */
	void SetTextureNoise( bool isUsed );
	
	
	
	/** Color texture is used. */
	inline bool GetTextureColor() const{ return pTextureColor; }
	
	/** Set if color texture is used. */
	void SetTextureColor( bool useTexture );
	
	/** Omni-direction color cube texture is used. */
	inline bool GetTextureColorOmnidirCube() const{ return pTextureColorOmniCube; }
	
	/** Set if omni-direction color cube texture is used. */
	void SetTextureColorOmnidirCube( bool useTexture );
	
	/** Omni-direction color equi-rectangular texture is used. */
	inline bool GetTextureColorOmnidirEquirect() const{ return pTextureColorOmniEquirect; }
	
	/** Set if omni-direction color equi-rectangular texture is used. */
	void SetTextureColorOmnidirEquirect( bool useTexture );
	
	
	
	/** First shadow map solid texture are used. */
	inline bool GetTextureShadow1Solid() const{ return pTextureShadow1Solid; }
	
	/** Set if first shadow map solid texture is used. */
	void SetTextureShadow1Solid( bool isUsed );
	
	/** First shadow map transparent textures is used. */
	inline bool GetTextureShadow1Transparent() const{ return pTextureShadow1Transparent; }
	
	/** Set if first shadow map transparent textures is used. */
	void SetTextureShadow1Transparent( bool isUsed );
	
	/** Second shadow map solid texture is used. */
	inline bool GetTextureShadow2Solid() const{ return pTextureShadow2Solid; }
	
	/** Set if second shadow map solid texture is used. */
	void SetTextureShadow2Solid( bool isUsed );
	
	/** Second shadow map transparent textures is used. */
	inline bool GetTextureShadow2Transparent() const{ return pTextureShadow2Transparent; }
	
	/** Set if second shadow map transparent textures is used. */
	void SetTextureShadow2Transparent( bool isUsed );
	
	/** Shadow ambient map texture is used. */
	inline bool GetTextureShadow1Ambient() const{ return pTextureShadow1Ambient; }
	
	/** Set if shadow ambient map is used. */
	void SetTextureShadow1Ambient( bool isUsed );
	
	/** Shadow ambient map texture is used. */
	inline bool GetTextureShadow2Ambient() const{ return pTextureShadow2Ambient; }
	
	/** Set if shadow ambient map is used. */
	void SetTextureShadow2Ambient( bool isUsed );
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Create short string list with the configuration. */
	void DebugGetConfigString( decString &string ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Copy the shader configuration to this shader configuration. */
	deoglLightShaderConfig &operator=( const deoglLightShaderConfig &config );
	
	/** Shader configuration is the same as this shader configuration. */
	bool operator==( const deoglLightShaderConfig &config ) const;
	/*@}*/
};

#endif
