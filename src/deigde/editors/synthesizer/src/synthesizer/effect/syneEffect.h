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

#ifndef _SYNEEFFECT_H_
#define _SYNEEFFECT_H_

#include "../controller/syneControllerTarget.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>

class syneLink;
class syneLoadSaveSystem;
class syneSource;
class syneSynthesizer;

class deEngine;



/**
 * \brief Synthesizer effect.
 */
class syneEffect : public deObject{
private:
	syneSynthesizer *pSynthesizer;
	syneSource *pParentSource;
	deSynthesizerEffect::Ref pEngEffect;
	
	deSynthesizerEffectVisitorIdentify::eEffectTypes pType;
	
	bool pEnabled;
	float pStrength;
	
	syneControllerTarget::Ref pTargetStrength;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<syneEffect> Ref;
	
	/** \brief Type holding list of objects. */
	typedef decTObjectOrderedSet<syneEffect> List;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer effect. */
	syneEffect(deSynthesizerEffectVisitorIdentify::eEffectTypes type);
	
	/** \brief Create a copy of an synthesizer effect. */
	syneEffect(const syneEffect &copy);
	
	/** \brief Clean up the synthesizer effect. */
protected:
	virtual ~syneEffect();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	syneSynthesizer *GetSynthesizer() const;
	
	
	
	/** \brief Engine synthesizer effect or \em nullptr. */
	inline const deSynthesizerEffect::Ref GetEngineEffect() const{ return pEngEffect; }
	
	/** \brief Set the engine synthesizer effect or \em nullptr. */
	void SetEngineEffect(deSynthesizerEffect *effect);
	
	/** \brief Create an engine synthesizer effect. */
	virtual deSynthesizerEffect::Ref CreateEngineEffect() = 0;
	
	/** \brief Init the given engine effect with the default effect properties. */
	void InitEngineEffect(deSynthesizerEffect *engEffect) const;
	
	/** \brief Retrieve the effect type. */
	inline deSynthesizerEffectVisitorIdentify::eEffectTypes GetType() const{ return pType; }
	
	/** \brief Retrieve the parent source or \em nullptr if there is none. */
	inline syneSource *GetParentSource() const{ return pParentSource; }
	
	/** \brief Set the parent source or \em nullptr if there is none. */
	void SetParentSource(syneSource *source);
	
	
	
	/** \brief Effect is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if effect is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Strength. */
	inline float GetStrength() const{ return pStrength; }
	
	/** \brief Set strength. */
	void SetStrength(float strength);
	
	
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Number of targets using link. */
	virtual int CountLinkUsage(syneLink *link) const;
	
	/** \brief Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets(syneLink *link);
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Strength target. */
	inline const syneControllerTarget::Ref &GetTargetStrength() const{ return pTargetStrength; }
	
	/** \brief List all links of all effect targets. */
	virtual void ListLinks(syneLink::List& list);
	
	/** \brief Notify engine and listeners that the effect changed. */
	void NotifyEffectChanged();
	
	/** \brief Notify engine only that the effect changed. */
	void NotifyEngineOnlyEffectChanged();
	
	/** \brief Create a copy of this effect. */
	virtual syneEffect::Ref CreateCopy() const = 0;
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer effect to this synthesizer effect. */
	syneEffect &operator=(const syneEffect &copy);
	/*@}*/
	
	
	
	/** \name Helper */
	/*@{*/
	/** \brief Create a new effect from a effect type. */
	static syneEffect::Ref CreateEffectFromType(deEngine *engine, deSynthesizerEffectVisitorIdentify::eEffectTypes type);
	/*@}*/
};

#endif
