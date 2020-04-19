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

#ifndef _DEMODELVERTEX_H_
#define _DEMODELVERTEX_H_

#include "../../common/math/decMath.h"


/**
 * \brief Model Vertex.
 */
class deModelVertex{
private:
	decVector pPosition;
	int pWeightSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model vertex. */
	deModelVertex();
	
	/** \brief Clean up model vertex. */
	~deModelVertex();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Index of the weight set or -1 if not used. */
	inline int GetWeightSet() const{ return pWeightSet; }
	
	/** \brief Set index of the weight set or -1 if not used. */
	void SetWeightSet( int index );
	/*@}*/
};

#endif
