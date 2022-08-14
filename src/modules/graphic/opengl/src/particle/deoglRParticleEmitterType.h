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

#ifndef _DEOGLRPARTICLEEMITTERTYPE_H_
#define _DEOGLRPARTICLEEMITTERTYPE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>

#include "../light/shader/deoglLightShader.h"

class deoglLightShaderConfig;
class deoglPixelBuffer;
class deoglRParticleEmitter;
class deoglRSkin;
class deoglSPBlockUBO;
class deoglTexture;



/**
 * \brief Render particle emitter type.
 */
class deoglRParticleEmitterType : public deObject{
public:
	/** Texture sampler curves. */
	enum eSampleCurves{
		// progress curves
		escSizeProgress,
		escRedProgress,
		escGreenProgress,
		escBlueProgress,
		escTransparencyProgress,
		escEmissivityProgress,
		// beam curves
		escSizeBeam,
		escRedBeam,
		escGreenBeam,
		escBlueBeam,
		escTransparencyBeam,
		escEmissivityBeam,
		// total number of curves
		ESC_COUNT
	};
	
	/** Shader Types. */
	enum eShaderTypes{
		estNoShadow, //<! No shadow casting.
		
		estStereoNoShadow, //<! No shadow casting.
	};
	
	static const int ShaderTypeCount = estStereoNoShadow + 1;
	
private:
	deoglRParticleEmitter &pEmitter;
	
	float pParamFactorLinVelo;
	float pParamFactorAngVelo;
	float *pParameterSamples;
	float pParamFactorMultiply[ ESC_COUNT ];
	float pParamFactorAdd[ ESC_COUNT ];
	deoglPixelBuffer *pPixelBufferSamples;
	deoglTexture *pTextureSamples;
	
	deoglRSkin *pSkin;
	
	bool pEmitLight;
	bool pHasTransparency;
	deoglLightShader::Ref pShaders[ ShaderTypeCount ];
	deoglSPBlockUBO *pParamBlockLight;
	
	deParticleEmitterType::eSimulationTypes pSimulationType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render type. */
	deoglRParticleEmitterType( deoglRParticleEmitter &emitter );
	
	/** \brief Clean up render type. */
	virtual ~deoglRParticleEmitterType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Particle emitter. */
	inline deoglRParticleEmitter &GetEmitter() const{ return pEmitter; }
	
	
	
	/** \brief Linear velocity parameter factor. */
	inline float GetParamFactorLinVelo() const{ return pParamFactorLinVelo; }
	
	/** \brief Angular velocity parameter factor. */
	inline float GetParamFactorAngVelo() const{ return pParamFactorAngVelo; }
	
	/** \brief Parameter multiply factor. */
	inline const float *GetParamFactorMultiply() const{ return &pParamFactorMultiply[ 0 ]; }
	
	/** \brief Parameter add factor. */
	inline const float *GetParamFactorAdd() const{ return &pParamFactorAdd[ 0 ]; }
	
	/** \brief Parameter samples. */
	inline const float *GetParameterSamples() const{ return pParameterSamples; }
	
	/** \brief Texture with the samples. */
	inline deoglTexture *GetTextureSamples() const{ return pTextureSamples; }
	
	
	
	/** \brief Simulation type. */
	inline deParticleEmitterType::eSimulationTypes GetSimulationType() const{ return pSimulationType; }
	
	/** \brief Set simulation type. */
	void SetSimulationType( deParticleEmitterType::eSimulationTypes simulationType );
	
	
	
	/** \brief Update parameter samples. */
	void UpdateParameterSamples( const deParticleEmitterType &type );
	
	/** \brief Sample a parameter. */
	void SampleParameters( eSampleCurves curveProgress, eSampleCurves curveBeam, const deParticleEmitterParameter &parameter );
	
	/** \brief Sampled parameter value using interpolation. */
	float GetSampledParameter( eSampleCurves curve, float location ) const;
	
	/** \brief Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** \brief Skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( deoglRSkin *skin );
	
	/** \brief Particles emit light. */
	inline bool GetEmitLight() const{ return pEmitLight; }
	
	/** \brief Check if particles emit light. */
	void CheckEmitLight( const deParticleEmitterType &type );
	
	/**
	 * \brief Particles have transparency.
	 * 
	 * Returns true if one or more of these conditions are true:
	 * - One or more transparency curves are not empty.
	 * - Transparency cast value is not 1.
	 * - Transparency spread value is not 0.
	 */
	inline bool GetHasTransparency() const{ return pHasTransparency; }
	
	
	
	/** Retrieves the shader for a shader type. */
	deoglLightShader *GetShaderFor( eShaderTypes shaderType );
	
	/** Retrieves the shader configuration for a shader type. */
	bool GetShaderConfigFor( eShaderTypes shaderType, deoglLightShaderConfig &config );
	
	/** Retrieves the light parameter block. */
	deoglSPBlockUBO *GetLightParameterBlock();
	
	/** Drop all light shaders and parameter blocks. */
	void DropLightShaders();
	/*@}*/
};

#endif
