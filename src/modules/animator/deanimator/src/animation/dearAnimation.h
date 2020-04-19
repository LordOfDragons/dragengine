/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARANIMATION_H_
#define _DEARANIMATION_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorAnimation.h>
#include <dragengine/common/collection/decObjectList.h>

class dearAnimationMove;
class deDEAnimator;
class deAnimation;



/**
 * \brief DEAnimator animation peer.
 */
class dearAnimation : public deBaseAnimatorAnimation{
private:
	deDEAnimator *pModule;
	
	deAnimation *pAnimation;
	
	decObjectList pMoves;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new peer. */
	dearAnimation( deDEAnimator *module, deAnimation *animation );
	/** \brief Cleans up the peer. */
	virtual ~dearAnimation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the module. */
	inline deDEAnimator *GetModule() const{ return pModule; }
	/** \brief Retrieve the animation. */
	inline deAnimation *GetAnimation() const{ return pAnimation; }
	
	/** \brief Retrieve the number of moves. */
	int GetMoveCount() const;
	/** \brief Retrieve move by index. */
	dearAnimationMove *GetMoveAt( int index ) const;
	/** \brief Retrieve move by name or NULL if not found. */
	dearAnimationMove *GetMoveNamed( const char *name ) const;
	/*@}*/
	
private:
	void pCleanUp();
	
	void pCreateMoves();
};

#endif
