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

#ifndef _DESYNTHESIZEREFFECT_H_
#define _DESYNTHESIZEREFFECT_H_

#include "../deSynthesizerControllerTarget.h"
#include "../../../deObject.h"

class deSynthesizer;
class deSynthesizerEffectVisitor;


/**
 * \brief Synthesizer effect.
 */
class deSynthesizerEffect : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSynthesizerEffect> Ref;
	
	
	
private:
	deSynthesizer *pSynthesizer;
	float pStrength;
	bool pEnabled;
	
	deSynthesizerControllerTarget pTargetStrength;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer effect. */
	deSynthesizerEffect();
	
protected:
	/**
	 * \brief Clean up synthesizer effect.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSynthesizerEffect();
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
	void SetSynthesizer( deSynthesizer *synthesizer );
	
	/** \brief Effect is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if effect is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Strength. */
	inline float GetStrength() const{ return pStrength; }
	
	/** \brief Set strength. */
	void SetStrength( float strength );
	
	
	
	/** \brief Strength target. */
	inline deSynthesizerControllerTarget &GetTargetStrength(){ return pTargetStrength; }
	inline const deSynthesizerControllerTarget &GetTargetStrength() const{ return pTargetStrength; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effect. */
	virtual void Visit( deSynthesizerEffectVisitor &visitor );
	/*@}*/
};

#endif
