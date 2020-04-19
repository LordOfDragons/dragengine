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

#ifndef _DEBASEAUDIOHEIGHTTERRAIN_H_
#define _DEBASEAUDIOHEIGHTTERRAIN_H_

#include "../../../common/math/decMath.h"

class deHeightTerrain;
class deHeightTerrainSector;
class deDecal;


/**
 * \brief Audio Module Height Terrain Peer
 *
 * Peer for the height terrain resource used by graphic modules.
 */
class deBaseAudioHeightTerrain{
private:
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseAudioHeightTerrain();
	
	/** \brief Clean up peer. */
	virtual ~deBaseAudioHeightTerrain();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parameter changes. */
	virtual void ParametersChanged();
	
	/** \brief Sector heights changed. */
	virtual void HeightChanged( const decPoint &fromSector, const decPoint &fromCoordinates,
		const decPoint &toSector, const decPoint &toCoordinates );
	
	/** \brief Sector has been added. */
	virtual void SectorAdded( deHeightTerrainSector *sector );
	
	/** \brief Sector has been removed. */
	virtual void SectorRemoved( int index );
	
	/** \brief All sectors have been removed. */
	virtual void AllSectorsRemoved();
	
	/** \brief Sector changed. */
	virtual void SectorChanged( int index );
	
	/** \brief Decal has been added. */
	virtual void DecalAdded( int sector, deDecal *decal );
	
	/** \brief Decal has been removed. */
	virtual void DecalRemoved( int sector, deDecal *decal );
	
	/** \brief All decals have been removed. */
	virtual void AllDecalsRemoved( int sector );
	/*@}*/
};

#endif
