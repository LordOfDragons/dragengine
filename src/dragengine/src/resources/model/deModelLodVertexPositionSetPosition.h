/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEMODELLODVERTEXPOSITIONSETPOSITION_H_
#define _DEMODELLODVERTEXPOSITIONSETPOSITION_H_

#include "../../common/math/decMath.h"


/**
 * \brief Model vertex position set position.
 * 
 * Position in a vertex position set.
 */
class DE_DLL_EXPORT deModelLodVertexPositionSetPosition{
private:
	int pVertex;
	decVector pPosition;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new model vertex position set position. */
	deModelLodVertexPositionSetPosition();
	
	/** \brief Clean up model vertex position set position. */
	~deModelLodVertexPositionSetPosition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of vertex. */
	inline int GetVertex() const{ return pVertex; }
	
	/** \brief Set index of vertex. */
	void SetVertex( int vertex );
	
	/** \brief Vertex position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set vertex position. */
	void SetPosition( const decVector &position );
	/*@}*/
};

#endif
