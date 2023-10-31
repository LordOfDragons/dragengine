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

#ifndef _SEEFFECT_H_
#define _SEEFFECT_H_

#include "../controller/seControllerTarget.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffect.h>
#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectVisitorIdentify.h>

class seLinkList;
class seLoadSaveSystem;
class seSource;
class seSynthesizer;

class deEngine;



/**
 * \brief Synthesizer effect.
 */
class seEffect : public deObject{
private:
	seSynthesizer *pSynthesizer;
	seSource *pParentSource;
	deSynthesizerEffect *pEngEffect;
	
	deSynthesizerEffectVisitorIdentify::eEffectTypes pType;
	
	bool pEnabled;
	float pStrength;
	
	seControllerTarget pTargetStrength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer effect. */
	seEffect( deSynthesizerEffectVisitorIdentify::eEffectTypes type );
	
	/** \brief Create a copy of an synthesizer effect. */
	seEffect( const seEffect &copy );
	
	/** \brief Clean up the synthesizer effect. */
	virtual ~seEffect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent synthesizer. */
	seSynthesizer *GetSynthesizer() const;
	
	
	
	/** \brief Engine synthesizer effect or \em NULL. */
	inline deSynthesizerEffect *GetEngineEffect() const{ return pEngEffect; }
	
	/** \brief Set the engine synthesizer effect or \em NULL. */
	void SetEngineEffect( deSynthesizerEffect *effect );
	
	/** \brief Create an engine synthesizer effect. */
	virtual deSynthesizerEffect *CreateEngineEffect() = 0;
	
	/** \brief Init the given engine effect with the default effect properties. */
	void InitEngineEffect( deSynthesizerEffect *engEffect ) const;
	
	/** \brief Retrieve the effect type. */
	inline deSynthesizerEffectVisitorIdentify::eEffectTypes GetType() const{ return pType; }
	
	/** \brief Retrieve the parent source or \em NULL if there is none. */
	inline seSource *GetParentSource() const{ return pParentSource; }
	
	/** \brief Set the parent source or \em NULL if there is none. */
	void SetParentSource( seSource *source );
	
	
	
	/** \brief Effect is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if effect is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Strength. */
	inline float GetStrength() const{ return pStrength; }
	
	/** \brief Set strength. */
	void SetStrength( float strength );
	
	
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Number of targets using link. */
	virtual int CountLinkUsage( seLink *link ) const;
	
	/** \brief Remove link from all targets using it. */
	virtual void RemoveLinkFromTargets( seLink *link );
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Strength target. */
	inline seControllerTarget &GetTargetStrength(){ return pTargetStrength; }
	inline const seControllerTarget &GetTargetStrength() const{ return pTargetStrength; }
	
	/** \brief List all links of all effect targets. */
	virtual void ListLinks( seLinkList& list );
	
	/** \brief Notify engine and listeners that the effect changed. */
	void NotifyEffectChanged();
	
	/** \brief Notify engine only that the effect changed. */
	void NotifyEngineOnlyEffectChanged();
	
	/** \brief Create a copy of this effect. */
	virtual seEffect *CreateCopy() const = 0;
	
	/** \brief Parent synthesizer changed. */
	virtual void SynthesizerChanged();
	
	/** \brief Synthesizer directory changed. */
	virtual void SynthesizerDirectoryChanged();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer effect to this synthesizer effect. */
	seEffect &operator=( const seEffect &copy );
	/*@}*/
	
	
	
	/** \name Helper */
	/*@{*/
	/** \brief Create a new effect from a effect type. */
	static seEffect *CreateEffectFromType( deEngine *engine, deSynthesizerEffectVisitorIdentify::eEffectTypes type );
	/*@}*/
};

#endif
