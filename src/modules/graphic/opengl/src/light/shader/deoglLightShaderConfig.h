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
 * \brief Light Shader Config.
 */
class deoglLightShaderConfig{
public:
	/** \brief Light modes. */
	enum eLightModes{
		/** \brief Point light mode. */
		elmPoint,
		
		/** \brief Spot light mode. */
		elmSpot,
		
		/** \brief Projector light mode. */
		elmProjector,
		
		/** \brief Sky light mode. */
		elmSky,
		
		/** \brief Particle light mode. */
		elmParticle
	};
	
	/** \brief Shadow tap mode. */
	enum eShadowTapModes{
		/** \brief Single tap. */
		estmSingle,
		
		/** \brief PCF 4-tap. */
		estmPcf4,
		
		/** \brief PCF 9-tap. */
		estmPcf9,
		
		/** \brief PCF with depth distance based variable tap width. */
		estmPcfVariableTap
	};
	
	/** \brief Shadow mapping algorithm. */
	enum eShadowMappingAlgorithms{
		/** \brief 2D texture mapping. */
		esma2D,
		
		/** \brief Cube mapping. */
		esmaCube,
		
		/** \brief Dual paraboloid shadow mapping. */
		esmaDualParaboloid,
		
		/** \brief Pyramid shadow mapping. */
		esmaPyramid
	};
	
	/** \brief Material normal modes. */
	enum eMaterialNormalModes{
		/** \brief Float mode for 3-component float textures. */
		emnmFloat,
		
		/** \brief Basic encoding for 3-component int textures. */
		emnmIntBasic,
		
		/** \brief Spheremap encoding for fitting normals into a 2-component int texture. */
		emnmSpheremap
	};
	
	/** \brief Particle mode. */
	enum eParticleModes{
		/** \brief Particle particle mode. */
		epmParticle,
		
		/** \brief Ribbon particle mode. */
		epmRibbon,
		
		/** \brief Beam particle mode. */
		epmBeam
	};
	
	
	
public:
	eLightModes pLightMode;
	eShadowTapModes pShadowTapMode;
	eShadowMappingAlgorithms pShadowMappingAlgorithm1;
	eShadowMappingAlgorithms pShadowMappingAlgorithm2;
	eMaterialNormalModes pMaterialNormalMode;
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
	
	bool pTextureNoise;
	bool pTextureColor;
	bool pTextureColorOmniCube;
	bool pTextureColorOmniEquirect;
	bool pTextureShadow1Solid;
	bool pTextureShadow1Transparent;
	bool pTextureShadow2Solid;
	bool pTextureShadow2Transparent;
	bool pTextureShadowAmbient;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create light shader configuration. */
	deoglLightShaderConfig();
	
	/** \brief Clean up light shader configuration. */
	~deoglLightShaderConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset configuration. */
	void Reset();
	
	
	
	/** \brief Light mode. */
	inline eLightModes GetLightMode() const{ return pLightMode; }
	
	/** \brief Set light mode. */
	void SetLightMode( eLightModes lightMode );
	
	/** \brief Shadow tap mode. */
	inline eShadowTapModes GetShadowTapMode() const{ return pShadowTapMode; }
	
	/** \brief Set shadow tap mode. */
	void SetShadowTapMode( eShadowTapModes shadowTapMode );
	
	/** \brief Shadow mapping algorithm for the first texture set. */
	inline eShadowMappingAlgorithms GetShadowMappingAlgorithm1() const{
		return pShadowMappingAlgorithm1; }
	
	/** \brief Set shadow mapping algorithm for the first texture set. */
	void SetShadowMappingAlgorithm1( eShadowMappingAlgorithms shadowMappingAlgorithm );
	
	/** \brief Shadow mapping algorithm for the second texture set. */
	inline eShadowMappingAlgorithms GetShadowMappingAlgorithm2() const{
		return pShadowMappingAlgorithm2; }
	
	/** \brief Set shadow mapping algorithm for the second texture set. */
	void SetShadowMappingAlgorithm2( eShadowMappingAlgorithms shadowMappingAlgorithm );
	
	/** \brief Material normal mode. */
	inline eMaterialNormalModes GetMaterialNormalMode() const{ return pMaterialNormalMode; }
	
	/** \brief Set material normal mode. */
	void SetMaterialNormalMode( eMaterialNormalModes materialNormalMode );
	
	/** \brief Particle mode. */
	inline eParticleModes GetParticleMode() const{ return pParticleMode; }
	
	/** \brief Set particle mode. */
	void SetParticleMode( eParticleModes mode );
	
	
	
	/** \brief Input depth has to be decoded. */
	inline bool GetDecodeInDepth() const{ return pDecodeInDepth; }
	
	/** \brief Set if input depth has to be decoded. */
	void SetDecodeInDepth( bool decodeInDepth );
	
	/** \brief Input shadow has to be decoded. */
	inline bool GetDecodeInShadow() const{ return pDecodeInShadow; }
	
	/** \brief Set if input shadow has to be decoded. */
	void SetDecodeInShadow( bool decodeInShadow );
	
	/** \brief Output depth has to be encoded. */
	inline bool GetEncodeOutDepth() const{ return pEncodeOutDepth; }
	
	/** \brief Set if output depth has to be encoded. */
	void SetEncodeOutDepth( bool encodeOutDepth );
	
	/** \brief Hardware depth comparison is used. */
	inline bool GetHWDepthCompare() const{ return pHWDepthCompare; }
	
	/** \brief Set hardware depth comparison is used. */
	void SetHWDepthCompare( bool hwDepthCompare );
	
	/** \brief Second shadow matrix equals the first one. */
	inline bool GetShadowMatrix2EqualsMatrix1() const{ return pShadowMatrix2EqualsMatrix1; }
	
	/** \brief Set if second shadow matrix equals the first one. */
	void SetShadowMatrix2EqualsMatrix1( bool shadowMatrix2EqualsMatrix1 );
	
	/** \brief Use inverse depth for shadow maps. */
	inline bool GetShadowInverseDepth() const{ return pShadowInverseDepth; }
	
	/** \brief Set if inverse depth is used for shadow maps. */
	void SetShadowInverseDepth( bool shadowInverseDepth );
	
	/** \brief Ambient lighting has to be used. */
	inline bool GetAmbientLighting() const{ return pAmbientLighting; }
	
	/** \brief Set ambient lighting has to be used. */
	void SetAmbientLighting( bool ambientLighting );
	
	/** \brief Full screen quad rendering has to be used. */
	inline bool GetFullScreenQuad() const{ return pFullScreenQuad; }
	
	/** \brief Set if full screen quad rendering has to be used. */
	void SetFullScreenQuad( bool fullScreenQuad );
	
	/** \brief Sub-surface separation has to be used. */
	inline bool GetSubSurface() const{ return pSubSurface; }
	
	/** \brief Set if sub-surface separation has to be used. */
	void SetSubSurface( bool subSurface );
	
	/** \brief Luminance only. */
	inline bool GetLuminanceOnly() const{ return pLuminanceOnly; }
	
	/** \brief Set luminance only. */
	void SetLuminanceOnly( bool luminanceOnly );
	
	
	
	/** \brief Noise texture is used. */
	inline bool GetTextureNoise() const{ return pTextureNoise; }
	
	/** \brief Set if noise texture is used. */
	void SetTextureNoise( bool isUsed );
	
	
	
	/** \brief Color texture is used. */
	inline bool GetTextureColor() const{ return pTextureColor; }
	
	/** \brief Set if color texture is used. */
	void SetTextureColor( bool useTexture );
	
	/** \brief Omni-direction color cube texture is used. */
	inline bool GetTextureColorOmnidirCube() const{ return pTextureColorOmniCube; }
	
	/** \brief Set if omni-direction color cube texture is used. */
	void SetTextureColorOmnidirCube( bool useTexture );
	
	/** \brief Omni-direction color equi-rectangular texture is used. */
	inline bool GetTextureColorOmnidirEquirect() const{ return pTextureColorOmniEquirect; }
	
	/** \brief Set if omni-direction color equi-rectangular texture is used. */
	void SetTextureColorOmnidirEquirect( bool useTexture );
	
	
	
	/** \brief First shadow map solid texture are used. */
	inline bool GetTextureShadow1Solid() const{ return pTextureShadow1Solid; }
	
	/** \brief Set if first shadow map solid texture is used. */
	void SetTextureShadow1Solid( bool isUsed );
	
	/** \brief First shadow map transparent textures is used. */
	inline bool GetTextureShadow1Transparent() const{ return pTextureShadow1Transparent; }
	
	/** \brief Set if first shadow map transparent textures is used. */
	void SetTextureShadow1Transparent( bool isUsed );
	
	/** \brief Second shadow map solid texture is used. */
	inline bool GetTextureShadow2Solid() const{ return pTextureShadow2Solid; }
	
	/** \brief Set if second shadow map solid texture is used. */
	void SetTextureShadow2Solid( bool isUsed );
	
	/** \brief Second shadow map transparent textures is used. */
	inline bool GetTextureShadow2Transparent() const{ return pTextureShadow2Transparent; }
	
	/** \brief Set if second shadow map transparent textures is used. */
	void SetTextureShadow2Transparent( bool isUsed );
	
	/** \brief Shadow ambient map texture is used. */
	inline bool GetTextureShadowAmbient() const{ return pTextureShadowAmbient; }
	
	/** \brief Set if shadow ambient map is used. */
	void SetTextureShadowAmbient( bool isUsed );
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** \brief Create short string list with the configuration. */
	void DebugGetConfigString( decString &string ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy the shader configuration to this shader configuration. */
	deoglLightShaderConfig &operator=( const deoglLightShaderConfig &config );
	
	/** \brief Shader configuration is the same as this shader configuration. */
	bool operator==( const deoglLightShaderConfig &config ) const;
	/*@}*/
};

#endif
