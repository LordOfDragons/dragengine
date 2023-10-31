/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESOURCE_H_
#define _SESOURCE_H_

#include "../controller/seControllerTarget.h"
#include "../effect/seEffectList.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

class seLinkList;
class seLoadSaveSystem;
class seSourceGroup;
class seSynthesizer;

class deEngine;



/**
 * \brief Synthesizer source.
 */
class seSource : public deObject{
private:
	seSynthesizer *pSynthesizer;
	seSourceGroup *pParentGroup;
	deSynthesizerSource *pEngSource;
	
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
	
	seEffectList pEffects;
	seEffect *pActiveEffect;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer source. */
	seSource( deSynthesizerSourceVisitorIdentify::eSourceTypes type );
	
	/** \brief Create a copy of an synthesizer source. */
	seSource( const seSource &copy );
	
	/** \brief Clean up the synthesizer source. */
	virtual ~seSource();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	seSynthesizer *GetSynthesizer() const;
	
	/** \brief Set parent synthesizer. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	
	
	/** \brief Engine synthesizer source or \em NULL. */
	inline deSynthesizerSource *GetEngineSource() const{ return pEngSource; }
	
	/** \brief Set the engine synthesizer source or \em NULL. */
	void SetEngineSource( deSynthesizerSource *source );
	
	/** \brief Create an engine synthesizer source. */
	virtual deSynthesizerSource *CreateEngineSource() = 0;
	
	/** \brief Init the given engine source with the default source properties. */
	void InitEngineSource( deSynthesizerSource *engSource ) const;
	
	/** \brief Retrieve the source type. */
	inline deSynthesizerSourceVisitorIdentify::eSourceTypes GetType() const{ return pType; }
	
	/** \brief Retrieve the parent group or \em NULL if there is none. */
	inline seSourceGroup *GetParentGroup() const{ return pParentGroup; }
	
	/** \brief Set the parent group or \em NULL if there is none. */
	void SetParentGroup( seSourceGroup *group );
	
	
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *filename );
	
	/** \brief Source is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if source is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Mix mode. */
	inline deSynthesizerSource::eMixModes GetMixMode() const{ return pMixMode; }
	
	/** \brief Set mix mode. */
	void SetMixMode( deSynthesizerSource::eMixModes mode );
	
	/** \brief Blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	
	/** \brief Set blend factor. */
	void SetBlendFactor( float factor );
	
	
	
	/** \brief Volume minimum relative to speaker volume in the range from 0 to 1. */
	inline float GetMinVolume() const{ return pMinVolume; }
	
	/** \brief Set minimum volume relative to speaker volume in the range from 0 to 1. */
	void SetMinVolume( float volume );
	
	/** \brief Maximum volume relative to speaker volume in the range from 0 to 1. */
	inline float GetMaxVolume() const{ return pMaxVolume; }
	
	/** \brief Set maximum volume relative to speaker volume in the range from 0 to 1. */
	void SetMaxVolume( float volume );
	
	/** \brief Minimum panning in the range from -1(left) to 1(right). */
	inline float GetMinPanning() const{ return pMinPanning; }
	
	/** \brief Set minimum panning in the range from -1(left) to 1(right). */
	void SetMinPanning( float panning );
	
	/** \brief Maximum panning in the range from -1(left) to 1(right). */
	inline float GetMaxPanning() const{ return pMaxPanning; }
	
	/** \brief Set maximum panning in the range from -1(left) to 1(right). */
	void SetMaxPanning( float panning );
	
	
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Number of targets using link. */
	virtual int CountLinkUsage( seLink *link ) const;
	
	/** \brief Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets( seLink *link );
	
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
	virtual void ListLinks( seLinkList& list );
	
	/** \brief Notify engine and listeners that the source changed. */
	void NotifySourceChanged();
	
	/** \brief Notify engine only that the source changed. */
	void NotifyEngineOnlySourceChanged();
	
	/** \brief Create a copy of this source. */
	virtual seSource *CreateCopy() const = 0;
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Active effect or \em NULL if none is active. */
	inline seEffect *GetActiveEffect() const{ return pActiveEffect; }
	
	/** \brief Set active effect or \em NULL if none is active. */
	void SetActiveEffect( seEffect *effect );
	
	/** \brief Effects. */
	inline const seEffectList &GetEffects() const{ return pEffects; }
	
	/** \brief Add effect. */
	void AddEffect( seEffect *effect );
	
	/** \brief Insert effect. */
	void InsertEffectAt( seEffect *effect, int index );
	
	/** \brief Move effect. */
	void MoveEffectTo( seEffect *effect, int index );
	
	/** \brief Remove effect. */
	void RemoveEffect( seEffect *effect );
	
	/** \brief Remove all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer source to this synthesizer source. */
	seSource &operator=( const seSource &copy );
	/*@}*/
	
	
	
	/** \name Helper */
	/*@{*/
	/** \brief Create a new source from a source type. */
	static seSource *CreateSourceFromType( deEngine *engine, deSynthesizerSourceVisitorIdentify::eSourceTypes type );
	/*@}*/
};

#endif
