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

#ifndef _SYNESOURCE_H_
#define _SYNESOURCE_H_

#include "../controller/syneControllerTarget.h"
#include "../effect/syneEffect.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

class syneLink;
class syneLoadSaveSystem;
class syneSourceGroup;
class syneSynthesizer;

class deEngine;



/**
 * \brief Synthesizer source.
 */
class syneSource : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<syneSource> Ref;
	
	/** \brief Type holding list of objects. */
	typedef decTObjectOrderedSet<syneSource> List;


private:
	syneSynthesizer *pSynthesizer;
	syneSourceGroup *pParentGroup;
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
	
	syneControllerTarget::Ref pTargetBlendFactor;
	syneControllerTarget::Ref pTargetVolume;
	syneControllerTarget::Ref pTargetPanning;
	
	syneEffect::List pEffects;
	syneEffect::Ref pActiveEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	syneSource(deSynthesizerSourceVisitorIdentify::eSourceTypes type);
	
	/** \brief Create a copy of an synthesizer source. */
	syneSource(const syneSource &copy);
	
	/** \brief Clean up the synthesizer source. */
protected:
	virtual ~syneSource();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	syneSynthesizer *GetSynthesizer() const;
	
	/** \brief Set parent synthesizer. */
	void SetSynthesizer(syneSynthesizer *synthesizer);
	
	
	
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
	inline syneSourceGroup *GetParentGroup() const{ return pParentGroup; }
	
	/** \brief Set the parent group or \em nullptr if there is none. */
	void SetParentGroup(syneSourceGroup *group);
	
	
	
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
	virtual int CountLinkUsage(syneLink *link) const;
	
	/** \brief Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets(syneLink *link);
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Source factor target. */
	inline const syneControllerTarget::Ref &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	
	/** \brief Source volume target. */
	inline const syneControllerTarget::Ref &GetTargetVolume() const{ return pTargetVolume; }
	
	/** \brief Source panning target. */
	inline const syneControllerTarget::Ref &GetTargetPanning() const{ return pTargetPanning; }
	
	/** \brief List all links of all source targets. */
	virtual void ListLinks(syneLink::List& list);
	
	/** \brief Notify engine and listeners that the source changed. */
	void NotifySourceChanged();
	
	/** \brief Notify engine only that the source changed. */
	void NotifyEngineOnlySourceChanged();
	
	/** \brief Create a copy of this source. */
	virtual syneSource::Ref CreateCopy() const = 0;
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active effect or \em nullptr if none is active. */
	inline const syneEffect::Ref &GetActiveEffect() const{ return pActiveEffect; }
	
	/** \brief Set active effect or \em nullptr if none is active. */
	void SetActiveEffect(syneEffect *effect);
	
	/** \brief Effects. */
	inline const syneEffect::List &GetEffects() const{ return pEffects; }
	
	/** \brief Add effect. */
	void AddEffect(syneEffect *effect);
	
	/** \brief Insert effect. */
	void InsertEffectAt(syneEffect *effect, int index);
	
	/** \brief Move effect. */
	void MoveEffectTo(syneEffect *effect, int index);
	
	/** \brief Remove effect. */
	void RemoveEffect(syneEffect *effect);
	
	/** \brief Remove all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer source to this synthesizer source. */
	syneSource &operator=(const syneSource &copy);
	/*@}*/
	
	
	
	/** \name Helper */
	/*@{*/
	/** \brief Create a new source from a source type. */
	static syneSource::Ref CreateSourceFromType(deEngine *engine, deSynthesizerSourceVisitorIdentify::eSourceTypes type);
	/*@}*/
};

#endif
