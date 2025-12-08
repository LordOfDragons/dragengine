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

#ifndef _DEOGLRPARTICLEEMITTERTYPE_H_
#define _DEOGLRPARTICLEEMITTERTYPE_H_

#include "../light/pipeline/deoglLightPipelinesParticle.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>

class deoglLightShaderConfig;
class deoglRParticleEmitter;
class deoglRSkin;
class deoglTexture;



/**
 * Render particle emitter type.
 */
class deoglRParticleEmitterType : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRParticleEmitterType> Ref;


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
	float pParamFactorMultiply[ESC_COUNT];
	float pParamFactorAdd[ESC_COUNT];
	deoglPixelBuffer::Ref pPixelBufferSamples;
	deoglTexture *pTextureSamples;
	
	deoglRSkin::Ref pSkin;
	
	bool pEmitLight;
	bool pHasTransparency;
	deoglLightPipelinesParticle::Ref pPipelines;
	deoglSPBlockUBO::Ref pParamBlockLight;
	
	deParticleEmitterType::eSimulationTypes pSimulationType;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render type. */
	deoglRParticleEmitterType(deoglRParticleEmitter &emitter);
	
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
	inline const float *GetParamFactorMultiply() const{ return &pParamFactorMultiply[0]; }
	
	/** Parameter add factor. */
	inline const float *GetParamFactorAdd() const{ return &pParamFactorAdd[0]; }
	
	/** Parameter samples. */
	inline const float *GetParameterSamples() const{ return pParameterSamples; }
	
	/** Texture with the samples. */
	inline deoglTexture *GetTextureSamples() const{ return pTextureSamples; }
	
	
	
	/** Simulation type. */
	inline deParticleEmitterType::eSimulationTypes GetSimulationType() const{ return pSimulationType; }
	
	/** Set simulation type. */
	void SetSimulationType(deParticleEmitterType::eSimulationTypes simulationType);
	
	
	
	/** Update parameter samples. */
	void UpdateParameterSamples(const deParticleEmitterType &type);
	
	/** Sample a parameter. */
	void SampleParameters(eSampleCurves curveProgress, eSampleCurves curveBeam, const deParticleEmitterParameter &parameter);
	
	/** Sampled parameter value using interpolation. */
	float GetSampledParameter(eSampleCurves curve, float location) const;
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	
	
	/** Skin. */
	inline deoglRSkin *GetSkin() const{ return pSkin; }
	
	/** Set skin. */
	void SetSkin(deoglRSkin::Ref skin);
	
	/** Particles emit light. */
	inline bool GetEmitLight() const{ return pEmitLight; }
	
	/** Check if particles emit light. */
	void CheckEmitLight(const deParticleEmitterType &type);
	
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
