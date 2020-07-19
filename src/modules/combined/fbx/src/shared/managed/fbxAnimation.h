/* 
 * FBX Modules
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

#ifndef _FBXANIMATION_H_
#define _FBXANIMATION_H_


#include <stdint.h>

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class fbxNode;
class fbxScene;
class fbxRig;
class fbxAnimationMove;

class deBaseModule;


/**
 * \brief FBX managed animation.
 */
class fbxAnimation : public deObject{
private:
	fbxScene &pScene;
	decObjectOrderedSet pMoves;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create animation. */
	fbxAnimation( fbxScene &scene );
	
protected:
	/** \brief Clean up connection. */
	virtual ~fbxAnimation();
	/*@}*/
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Scene. */
	inline fbxScene &GetScene() const{ return pScene; }
	
	
	
	/** \brief Count of moves. */
	int GetMoveCount() const;
	
	/** \brief Move at index. */
	fbxAnimationMove *GetMoveAt( int index ) const;
	
	/** \brief Named move or NULL if absent. */
	fbxAnimationMove *GetMoveNamed( const char *name ) const;
	
	/** \brief Match moves against rig. */
	void MatchRig( const fbxRig &rig );
	
	/** \brief Convert FBX time to seconds. */
	static float ConvTime( int64_t time );
	
	
	
	
	/** \brief Debug print node structure. */
	void DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose = false ) const;
	/*@}*/
};

#endif
