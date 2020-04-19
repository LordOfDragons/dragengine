/* 
 * Drag[en]gine SmallTalk Script Module
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

#ifndef _STCLASSPOINT_H_
#define _STCLASSPOINT_H_

#include "../../stCommon.h"

#include <dragengine/common/math/decMath.h>

class ScriptingSmalltalk;



/**
 * \brief Script class immutable point.
 */
class stClassPoint{
private:
	ScriptingSmalltalk &pST;
	OOP pOOPClass;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	stClassPoint( ScriptingSmalltalk &st );
	
	/** \brief Clean up class. */
	virtual ~stClassPoint();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline ScriptingSmalltalk &GetST() const{ return pST; }
	
	
	
	/** \brief Set up links. */
	void SetUpLinks();
	
	/** \brief Point from object. */
	const decPoint &OOPToPoint( OOP object ) const;
	
	/** \brief Push vector2. */
	OOP PointToOOP( const decPoint &vector );
	/*@}*/
	
	
	
	/** \name cCall Methods */
	/*@{*/
	/** \brief Create zero vector2 object. */
	static OOP ccNew( OOP self );
	
	/** \brief Create vector2 object. */
	static OOP ccNewXY( OOP self, int x, int y );
	
	
	
	/** \brief X component. */
	static int ccX( OOP self );
	
	/** \brief Y component. */
	static int ccY( OOP self );
	
	
	
	/** \brief Hash code. */
	static int ccHash( OOP self );
	
	/** \brief String representation. */
	static OOP ccAsString( OOP self );
	/*@}*/
};

#endif
