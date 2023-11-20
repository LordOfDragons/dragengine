/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEHEIGHTTERRAINNAVSPACEFACE_H_
#define _MEHEIGHTTERRAINNAVSPACEFACE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decIntList.h>

class meHeightTerrainNavSpaceType;


/**
 * \brief Height terrain navigation space face.
 */
class meHeightTerrainNavSpaceFace : public deObject{
public:
	typedef deTObjectReference<meHeightTerrainNavSpaceFace> Ref;
	
	
	
private:
	meHeightTerrainNavSpaceType *pType;
	decIntList pNavPoints;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates height terrain navigation space face. */
	meHeightTerrainNavSpaceFace();
	
	/** \brief Clean up height terrain navigation space face. */
	virtual ~meHeightTerrainNavSpaceFace();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent type or \em NULL. */
	inline meHeightTerrainNavSpaceType *GetType() const{ return pType; }
	
	/** \brief Set parent type or \em NULL. */
	void SetType( meHeightTerrainNavSpaceType *type );
	
	
	
	/** \brief Navigation points. */
	decIntList &GetNavPoints(){ return pNavPoints; }
	const decIntList &GetNavPoints() const{ return pNavPoints; }
	
	/** \brief Order navigation points clock-wise. */
	void OrderClockwise( int pointsPerRow );
	
	/** \brief Navigation points are matching order independent. */
	bool NavPointsMatch( const decIntList &navpoints ) const;
	
	/** \brief Face has navigation point. */
	bool HasNavPoint( int navpoint ) const;
	
	/** \brief Face has edge. */
	bool HasNavEdge( int navpoint1, int navpoint2 ) const;
	
	/** \brief All face navigation points are contained in a list in any order. */
	bool HasAllNavPointsIn( const decIntList &navpoints ) const;
	/*@}*/
};

#endif
