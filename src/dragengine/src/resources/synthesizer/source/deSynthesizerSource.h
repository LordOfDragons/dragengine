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

#ifndef _DESYNTHESIZERSOURCE_H_
#define _DESYNTHESIZERSOURCE_H_

#include "../deSynthesizerControllerTarget.h"
#include "../../../deObject.h"
#include "../../../common/collection/decObjectOrderedSet.h"

class deSynthesizer;
class deSynthesizerEffect;
class deSynthesizerSourceVisitor;


/**
 * \brief Synthesizer source.
 */
class DE_DLL_EXPORT deSynthesizerSource : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerSource> Ref;
	
	
public:
	/** \brief Mixing modes. */
	enum eMixModes{
		/** \brief Blend sound output of this source over the previous sound. */
		emmBlend,
		
		/** \brief Add sound output to the previous sounds clipping if too loud. */
		emmAdd
	};
	
	
	
private:
	deSynthesizer *pSynthesizer;
	eMixModes pMixMode;
	float pBlendFactor;
	bool pEnabled;
	
	float pMinVolume;
	float pMaxVolume;
	float pMinPanning;
	float pMaxPanning;
	
	deSynthesizerControllerTarget pTargetBlendFactor;
	deSynthesizerControllerTarget pTargetVolume;
	deSynthesizerControllerTarget pTargetPanning;
	
	decObjectOrderedSet pEffects;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	deSynthesizerSource();
	
protected:
	/**
	 * \brief Clean up synthesizer source.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deSynthesizerSource() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	inline deSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/**
	 * \brief Set parent synthesizer.
	 * 
	 * This function has to be used only by the deSynthesizer class to
	 * assign itself as a parent. Do not call this function directly.
	 */
	void SetSynthesizer(deSynthesizer *synthesizer);
	
	/** \brief Source is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if source is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Mixing mode. */
	inline eMixModes GetMixMode() const{ return pMixMode; }
	
	/** \brief Set mixing mode. */
	void SetMixMode(eMixModes mode);
	
	/** \brief Blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	
	/** \brief Set blend factor. */
	void SetBlendFactor(float factor);
	
	
	
	/** \brief Volume minimum relative to speaker volume in the range from 0 to 1. */
	inline float GetMinVolume() const{ return pMinVolume; }
	
	/** \brief Set minimum volume relative to speaker volume in the range from 0 to 1. */
	void SetMinVolume(float volume);
	
	/** \brief Maximum volume relative to speaker volume in the range from 0 to 1. */
	inline float GetMaxVolume() const{ return pMaxVolume; }
	
	/** \brief Set maximum volume relative to speaker volume in the range from 0 to 1. */
	void SetMaxVolume(float volume);
	
	/** \brief Minimum panning in the range from -1(left) to 1(right). */
	inline float GetMinPanning() const{ return pMinPanning; }
	
	/** \brief Set minimum panning in the range from -1(left) to 1(right). */
	void SetMinPanning(float panning);
	
	/** \brief Maximum panning in the range from -1(left) to 1(right). */
	inline float GetMaxPanning() const{ return pMaxPanning; }
	
	/** \brief Set maximum panning in the range from -1(left) to 1(right). */
	void SetMaxPanning(float panning);
	
	
	
	/** \brief Blend factor target. */
	inline deSynthesizerControllerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const deSynthesizerControllerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** \brief Volume target. */
	inline deSynthesizerControllerTarget &GetTargetVolume(){ return pTargetVolume; }
	inline const deSynthesizerControllerTarget &GetTargetVolume() const{ return pTargetVolume; }
	
	/** \brief Panning target. */
	inline deSynthesizerControllerTarget &GetTargetPanning(){ return pTargetPanning; }
	inline const deSynthesizerControllerTarget &GetTargetPanning() const{ return pTargetPanning; }
	/*@}*/
	
	
	
	/** \name Effects */
	/*@{*/
	/** \brief Number of effects. */
	int GetEffectCount() const;
	
	/** \brief Effect at index. */
	deSynthesizerEffect *GetEffectAt(int index) const;
	
	/** \brief Add effect. */
	void AddEffect(deSynthesizerEffect *effect);
	
	/** \brief Remove effect. */
	void RemoveEffect(deSynthesizerEffect *effect);
	
	/** \brief Remove all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit source. */
	virtual void Visit(deSynthesizerSourceVisitor &visitor);
	/*@}*/
};

#endif
