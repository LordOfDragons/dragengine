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

#ifndef _SEEFFECTSTRETCH_H_
#define _SEEFFECTSTRETCH_H_

#include "seEffect.h"

#include <dragengine/resources/synthesizer/effect/deSynthesizerEffectStretch.h>



/**
 * \brief Synthesizer effect stretch time and pitch.
 */
class seEffectStretch : public seEffect{
private:
	float pMinTime;
	float pMaxTime;
	float pMinPitch;
	float pMaxPitch;
	
	seControllerTarget pTargetTime;
	seControllerTarget pTargetPitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create effect. */
	seEffectStretch();
	
	/** \brief Create copy of effect. */
	seEffectStretch( const seEffectStretch &copy );
	
	/** \brief Clean up effect. */
	virtual ~seEffectStretch();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Minimum time stretch factor. */
	inline float GetMinTime() const{ return pMinTime; }
	
	/** \brief Set minimum time stretch factor. */
	void SetMinTime( float stretch );
	
	/** \brief Maximum time stretch factor. */
	inline float GetMaxTime() const{ return pMaxTime; }
	
	/** \brief Set maximum time stretch factor. */
	void SetMaxTime( float stretch );
	
	/** \brief Minimum pitch shift factor. */
	inline float GetMinPitch() const{ return pMinPitch; }
	
	/** \brief Set minimum pitch shift factor. */
	void SetMinPitch( float shift );
	
	/** \brief Maximum pitch shift factor. */
	inline float GetMaxPitch() const{ return pMaxPitch; }
	
	/** \brief Set maximum pitch shift factor. */
	void SetMaxPitch( float shift );
	
	
	
	/** \brief Time stretch target. */
	inline seControllerTarget &GetTargetTime(){ return pTargetTime; }
	inline const seControllerTarget &GetTargetTime() const{ return pTargetTime; }
	
	/** \brief Pitch stretch target. */
	inline seControllerTarget &GetTargetPitch(){ return pTargetPitch; }
	inline const seControllerTarget &GetTargetPitch() const{ return pTargetPitch; }
	
	
	
	/** \brief Create engine synthesizer effect. */
	virtual deSynthesizerEffect *CreateEngineEffect();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( seLink *link ) const;
	
	/** \brief Removes a link from all targets using it. */
	virtual void RemoveLinkFromTargets( seLink *link );
	
	/** \brief Removes all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	/** \brief Create a copy of this effect. */
	virtual seEffect *CreateCopy() const;
	
	/** \brief List all links of all effect targets. */
	virtual void ListLinks( seLinkList& list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another synthesizer difference effect to this synthesizer difference effect. */
	virtual seEffectStretch &operator=( const seEffectStretch &copy );
	/*@}*/
};

#endif
