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

#ifndef _DEHEIGHTTERRAINNAVSPACEEDGE_H_
#define _DEHEIGHTTERRAINNAVSPACEEDGE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Height terrain navigation space edge.
 */
class DE_DLL_EXPORT deHeightTerrainNavSpaceEdge{
private:
	unsigned int pPoint1;
	unsigned int pPoint2;
	unsigned short pType1;
	unsigned short pType2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create edge. */
	deHeightTerrainNavSpaceEdge();
	
	/** \brief Clean up edge. */
	~deHeightTerrainNavSpaceEdge();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief First navpoint. */
	inline unsigned int GetPoint1() const{ return pPoint1; }
	
	/** \brief Set first navpoint. */
	void SetPoint1( unsigned int navpoint );
	
	/** \brief Second navpoint. */
	inline unsigned int GetPoint2() const{ return pPoint2; }
	
	/** \brief Set second navpoint. */
	void SetPoint2( unsigned int navpoint );
	
	/** \brief Type to use for edge to travel from first to second navpoint. */
	inline unsigned short GetType1() const{ return pType1; }
	
	/** \brief Set type to use for edge to travel from first to second navpoint. */
	void SetType1( unsigned short type );
	
	/** \brief Type to use for edge to travel from second to first navpoint. */
	inline unsigned short GetType2() const{ return pType2; }
	
	/** \brief Set type to use for edge to travel from second to first navpoint. */
	void SetType2( unsigned short type );
	/*@}*/
};

#endif
