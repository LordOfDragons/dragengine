/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESYNTHESIZEREFFECSTRETCH_H_
#define _DESYNTHESIZEREFFECSTRETCH_H_

#include "deSynthesizerEffect.h"


/**
 * \brief Synthesizer effect stretch time and pitch.
 */
class deSynthesizerEffectStretch : public deSynthesizerEffect{
private:
	float pMinTime;
	float pMaxTime;
	float pMinPitch;
	float pMaxPitch;
	
	deSynthesizerControllerTarget pTargetTime;
	deSynthesizerControllerTarget pTargetPitch;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer stretch effect. */
	deSynthesizerEffectStretch();
	
protected:
	/**
	 * \brief Clean up synthesizer stretch effect.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerEffectStretch();
	/*@}*/
	
	
	
public:
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
	inline deSynthesizerControllerTarget &GetTargetTime(){ return pTargetTime; }
	inline const deSynthesizerControllerTarget &GetTargetTime() const{ return pTargetTime; }
	
	/** \brief Pitch stretch target. */
	inline deSynthesizerControllerTarget &GetTargetPitch(){ return pTargetPitch; }
	inline const deSynthesizerControllerTarget &GetTargetPitch() const{ return pTargetPitch; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deSynthesizerEffectVisitor &visitor );
	/*@}*/
};

#endif
