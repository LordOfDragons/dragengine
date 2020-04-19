/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSNAVIGATIONINFO_H_
#define _DEDSNAVIGATIONINFO_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deScriptingDragonScript;

class dsValue;
class dsRealObject;



/**
 * \brief Navigation information.
 */
class dedsNavigationInfo : public deObject{
private:
	deScriptingDragonScript &pDS;
	
	decDVector pPosition;
	int pCostType;
	
	int pPathIndex;
	float pPathFactor;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation information. */
	dedsNavigationInfo( deScriptingDragonScript &ds );
	
	/** \brief Create copy of a navigation information. */
	dedsNavigationInfo( const dedsNavigationInfo &info );
	
	/** \brief Clean up the navigation information. */
	virtual ~dedsNavigationInfo();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Cost type at position if position is not \em null. */
	inline int GetCostType() const{ return pCostType; }
	
	/** \brief Set cost type at position. */
	void SetCostType( int costType );
	
	
	
	/**
	 * \brief Index along navigation path.
	 * \details -1 indicates navigation start position towards first path point.
	 */
	inline int GetPathIndex() const{ return pPathIndex; }
	
	/**
	 * \brief Set index along navigation path.
	 * \details -1 indicates navigation start position towards first path point.
	 * \throws dueEInvalidParam \em index is less than -1.
	 */
	void SetPathIndex( int index );
	
	/** \brief Factor along path between getPathIndex() and getPathIndex()+1 from 0 to 1. */
	inline float GetPathFactor() const{ return pPathFactor; }
	
	/** \brief Set factor along path between getPathIndex() and getPathIndex()+1 from 0 to 1. */
	void SetPathFactor( float factor );
	/*@}*/
};

#endif
