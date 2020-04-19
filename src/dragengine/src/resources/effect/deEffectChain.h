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

#ifndef _DEEFFECTCHAIN_H_
#define _DEEFFECTCHAIN_H_

#include "../../common/math/decMath.h"


// definitions

class deEffect;
class deEffectVisitor;


/**
 * \brief Effect Chain Class.
 *
 * An effect chain holds a list of effects that are execute in the order
 * they have been added to the effect chain. The effect chain is a
 * convenience class that works with effects of various types so you do
 * not have to implement this every time. As this object is not reference
 * counted in any way the user class is responsible to free the effect
 * chain itself once the user class is destroyed.
 */
class deEffectChain{
private:
	deEffect **pEffects;
	int pEffectCount, pEffectSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new effect chain. */
	deEffectChain();
	
	/** \brief Clean up world. */
	~deEffectChain();
	/*@}*/
	
	
	
	/** \name Effect management */
	/*@{*/
	/** \brief Number of effects in the chain. */
	inline int GetEffectCount() const{ return pEffectCount; }
	
	/** \brief Effect at the given index. */
	deEffect *GetEffectAt( int index ) const;
	
	/** \brief Given effect is already part of the chain. */
	bool HasEffect( deEffect *effect ) const;
	
	/** \brief Position of the given effect in the chain or -1 if not part of it. */
	int IndexOfEffect( deEffect *effect ) const;
	
	/**
	 * \brief Add effect to the chain.
	 * 
	 * Throws an exception if already in the chain.
	 */
	void AddEffect( deEffect *effect );
	
	/**
	 * \brief Insert effect at the given position to the chain.
	 * 
	 * Throws an exception if already already part of it. The new effect is inserted
	 * at the given position moving all effects after it up one position.
	 */
	void InsertEffect( deEffect *effect, int position );
	
	/** \brief Removes the given effect from the chain. */
	void RemoveEffect( deEffect *effect );
	
	/** \brief Removes all effects from the chain. */
	void RemoveAllEffects();
	
	/**
	 * \brief Move given effect to a new position.
	 * 
	 * The effect is placed at the new position moving all effects after it up one position.
	 */
	void MoveEffect( deEffect *effect, int newPosition );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit effects. */
	void VisitEffects( deEffectVisitor &visitor );
	/*@}*/
};

#endif
