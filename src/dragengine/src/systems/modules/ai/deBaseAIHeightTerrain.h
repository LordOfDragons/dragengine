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

#ifndef _DEBASEAIHEIGHTTERRAIN_H_
#define _DEBASEAIHEIGHTTERRAIN_H_

#include "../../../common/math/decMath.h"

class deHeightTerrainNavSpace;
class deHeightTerrainSector;


/**
 * \brief AI module height terrain peer.
 */
class DE_DLL_EXPORT deBaseAIHeightTerrain{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseAIHeightTerrain();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAIHeightTerrain();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** \brief Sector added. */
	virtual void SectorAdded( deHeightTerrainSector *sector );
	
	/** \brief Sector removed. */
	virtual void SectorRemoved( int sector );
	
	/** \brief All sector removed. */
	virtual void AllSectorsRemoved();
	
	/** \brief Sector changed. */
	virtual void SectorChanged( int sector );
	
	
	
	/** \brief Sector heights changed. */
	virtual void HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates );
	
	
	
	/** \brief Navigation space added. */
	virtual void NavSpaceAdded( int sector, deHeightTerrainNavSpace *navspace );
	
	/** \brief Navigation space removed. */
	virtual void NavSpaceRemoved( int sector, int index );
	
	/** \brief All navigation spaces removed. */
	virtual void AllNavSpacesRemoved( int sector );
	
	/** \brief Navigation space layer changed. */
	virtual void NavSpaceLayerChanged( int sector, int index );
	
	/** \brief Navigation space type changed. */
	virtual void NavSpaceTypeChanged( int sector, int index );
	
	/** \brief Navigation space snapping parameters changed. */
	virtual void NavSpaceSnappingChanged( int sector, int index );
	
	/** \brief Navigation space layout changed. */
	virtual void NavSpaceLayoutChanged( int sector, int index );
	/*@}*/
};

#endif
