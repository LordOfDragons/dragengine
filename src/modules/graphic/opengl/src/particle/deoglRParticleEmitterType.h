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

#include "../light/pipeline/deoglLightPipelinesParticle.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"

class deoglLightShaderConfig;
class deoglPixelBuffer;
class deoglRParticleEmitter;
class deoglRSkin;
class deoglTexture;



/**
 * Render particle emitter type.
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
	deoglLightPipelinesParticle::Ref pPipelines;
	deoglSPBlockUBO::Ref pParamBlockLight;
	
	deParticleEmitterType::eSimulationTypes pSimulationType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render type. */
	deoglRParticleEmitterType( deoglRParticleEmitter &emitter );
	
	/** Clean up render type. */
	virtual ~deoglRParticleEmitterType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Particle emitter. */
	inline deoglRParticleEmitter &GetEmitter() const{ return pEmitter; }
	
	
	
	/** Linear velocity parameter factor. */
	inline float GetParamFactorLinVelo() const{ return pParamFactorLinVelo; }
	
	/** Angular velocity parameter factor. */
	inline float GetParamFactorAngVelo() const{ return pParamFactorAngVelo; }
	
	/** Parameter multiply factor. */
	inline const float *GetParamFactorMultiply() const{ return &pParamFactorMultiply[ 0 ]; }
	
	/** Parameter add factor. */
	inline const float *GetParamFactorAdd() const{ return &pParamFactorAdd[ 0 ]; }
	
	/** Parameter samples. */
	inline const float *GetParameterSamples() const{ return pParameterSamples; }
	
	/** Texture with the samples. */
	inline deoglTexture *GetTextureSamples() const{ return pTextureSamples; }
	
	
	
	/** Simulation type. */
	inline deParticleEmitterType::eSimulationTypes GetSimulationType() const{ return pSimulationType; }
	
	/** Set simulation type. */
	void SetSimulationType( deParticleEmitterType::eSimulationTypes simulationType );
	
	
	
	/** Update parameter samples. */
	void UpdateParameterSamples( const deParticleEmitterType &type );
	
	/** Sample a parameter. */
	void SampleParameters( eSampleCurves curveProgress, eSampleCurves curveBeam, const deParticleEmitterParameter &parameter );
	
	/** Sampled parameter value using interpolation. */
	float GetSampledParameter( eSampleCurves curve, float location ) const;
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** Skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin. */
	void SetSkin( deoglRSkin *skin );
	
	/** Particles emit light. */
	inline bool GetEmitLight() const{ return pEmitLight; }
	
	/** Check if particles emit light. */
	void CheckEmitLight( const deParticleEmitterType &type );
	
	/**
	 * Particles have transparency.
	 * 
	 * Returns true if one or more of these conditions are true:
	 * - One or more transparency curves are not empty.
	 * - Transparency cast value is not 1.
	 * - Transparency spread value is not 0.
	 */
	inline bool GetHasTransparency() const{ return pHasTransparency; }
	
	
	
	/** Pipelines. */
	deoglLightPipelines &GetPipelines();
	
	/** Retrieves the light parameter block. */
	deoglSPBlockUBO &GetLightParameterBlock();
	
	/** Drop all light pipelines and parameter blocks. */
	void DropPipelines();
	/*@}*/
};

#endif
