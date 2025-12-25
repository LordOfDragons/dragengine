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

#ifndef _SESOURCE_H_
#define _SESOURCE_H_

#include "../controller/seControllerTarget.h"
#include "../effect/seEffect.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

class seLink;
class seLoadSaveSystem;
class seSourceGroup;
class seSynthesizer;

class deEngine;



/**
 * \brief Synthesizer source.
 */
class seSource : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seSource> Ref;
	
	/** \brief Type holding list of objects. */
	typedef decTObjectOrderedSet<seSource> List;


private:
	seSynthesizer *pSynthesizer;
	seSourceGroup *pParentGroup;
	deSynthesizerSource::Ref pEngSource;
	
	decString pName;
	deSynthesizerSourceVisitorIdentify::eSourceTypes pType;
	
	deSynthesizerSource::eMixModes pMixMode;
	float pBlendFactor;
	bool pEnabled;
	
	float pMinVolume;
	float pMaxVolume;
	float pMinPanning;
	float pMaxPanning;
	
	seControllerTarget pTargetBlendFactor;
	seControllerTarget pTargetVolume;
	seControllerTarget pTargetPanning;
	
	seEffect::List pEffects;
	seEffect::Ref pActiveEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	seSource(deSynthesizerSourceVisitorIdentify::eSourceTypes type);
	
	/** \brief Create a copy of an synthesizer source. */
	seSource(const seSource &copy);
	
	/** \brief Clean up the synthesizer source. */
	virtual ~seSource();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	seSynthesizer *GetSynthesizer() const;
	
	/** \brief Set parent synthesizer. */
	void SetSynthesizer(seSynthesizer *synthesizer);
	
	
	
	/** \brief Engine synthesizer source or \em nullptr. */
	inline const deSynthesizerSource::Ref &GetEngineSource() const{ return pEngSource; }
	
	/** \brief Set the engine synthesizer source or \em nullptr. */
	void SetEngineSource(deSynthesizerSource *source);
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource::Ref CreateEngineSource() = 0;
	
	/** \brief Init the given engine source with the default source properties. */
	void InitEngineSource(deSynthesizerSource *engSource) const;
	
	/** \brief Retrieve the source type. */
	inline deSynthesizerSourceVisitorIdentify::eSourceTypes GetType() const{ return pType; }
	
	/** \brief Retrieve the parent group or \em nullptr if there is none. */
	inline seSourceGroup *GetParentGroup() const{ return pParentGroup; }
	
	/** \brief Set the parent group or \em nullptr if there is none. */
	void SetParentGroup(seSourceGroup *group);
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName(const char *filename);
	
	/** \brief Source is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if source is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Mix mode. */
	inline deSynthesizerSource::eMixModes GetMixMode() const{ return pMixMode; }
	
	/** \brief Set mix mode. */
	void SetMixMode(deSynthesizerSource::eMixModes mode);
	
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
	
	
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Number of targets using link. */
	virtual int CountLinkUsage(seLink *link) const;
	
	/** \brief Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets(seLink *link);
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Source factor target. */
	inline seControllerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const seControllerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** \brief Source volume target. */
	inline seControllerTarget &GetTargetVolume(){ return pTargetVolume; }
	inline const seControllerTarget &GetTargetVolume() const{ return pTargetVolume; }
	
	/** \brief Source panning target. */
	inline seControllerTarget &GetTargetPanning(){ return pTargetPanning; }
	inline const seControllerTarget &GetTargetPanning() const{ return pTargetPanning; }
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks(seLink::List& list);
	
	/** \brief Notify engine and listeners that the source changed. */
	void NotifySourceChanged();
	
	/** \brief Notify engine only that the source changed. */
	void NotifyEngineOnlySourceChanged();
	
	/** \brief Create a copy of this source. */
	virtual seSource::Ref CreateCopy() const = 0;
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active effect or \em nullptr if none is active. */
	inline const seEffect::Ref &GetActiveEffect() const{ return pActiveEffect; }
	
	/** \brief Set active effect or \em nullptr if none is active. */
	void SetActiveEffect(seEffect *effect);
	
	/** \brief Effects. */
	inline const seEffect::List &GetEffects() const{ return pEffects; }
	
	/** \brief Add effect. */
	void AddEffect(seEffect *effect);
	
	/** \brief Insert effect. */
	void InsertEffectAt(seEffect *effect, int index);
	
	/** \brief Move effect. */
	void MoveEffectTo(seEffect *effect, int index);
	
	/** \brief Remove effect. */
	void RemoveEffect(seEffect *effect);
	
	/** \brief Remove all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer source to this synthesizer source. */
	seSource &operator=(const seSource &copy);
	/*@}*/
	
	
	
	/** \name Helper */
	/*@{*/
	/** \brief Create a new source from a source type. */
	static seSource::Ref CreateSourceFromType(deEngine *engine, deSynthesizerSourceVisitorIdentify::eSourceTypes type);
	/*@}*/
};

#endif
