/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEDAIHEIGHTTERRAINSECTOR_H_
#define _DEDAIHEIGHTTERRAINSECTOR_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class dedaiHeightTerrain;
class dedaiHeightTerrainNavSpace;
class dedaiSpace;

class deHeightTerrainSector;



/**
 * \brief Navigation space peer.
 */
class dedaiHeightTerrainSector : public deObject{
private:
	dedaiHeightTerrain &pHeightTerrain;
	const deHeightTerrainSector &pSector;
	
	decDVector pPosition;
	float *pHeights;
	
	decObjectOrderedSet pNavSpaces;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sector. */
	dedaiHeightTerrainSector( dedaiHeightTerrain &heightTerrain, const deHeightTerrainSector &sector );
	
protected:
	/** \brief Clean up sector. */
	virtual ~dedaiHeightTerrainSector();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height terrain. */
	inline dedaiHeightTerrain &GetHeightTerrain() const{ return pHeightTerrain; }
	
	/** \brief Sector. */
	inline const deHeightTerrainSector &GetSector() const{ return pSector; }
	
	
	
	/** \brief World position of sector center. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Heights. */
	inline const float *GetHeights() const{ return pHeights; }
	/*@}*/
	
	
	
	/** \name Navigation spaces */
	/*@{*/
	/** \brief Number of navigation spaces. */
	int GetNavSpaceCount() const;
	
	/** \brief Navigation space at index. */
	dedaiHeightTerrainNavSpace *GetNavSpaceAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Parent world changed. */
	void ParentWorldChanged();
	
	/** \brief Sector changed. */
	void SectorChanged();
	
	/** \brief Sector heights changed. */
	void HeightChanged( const decPoint &from, const decPoint &to );
	
	/** \brief Navigation space added. */
	void NavSpaceAdded( deHeightTerrainNavSpace *navspace );
	
	/** \brief Navigation space removed. */
	void NavSpaceRemoved( int index );
	
	/** \brief All navigation spaces removed. */
	void AllNavSpacesRemoved();
	
	/** \brief Navigation space layer changed. */
	void NavSpaceLayerChanged( int index );
	
	/** \brief Navigation space type changed. */
	void NavSpaceTypeChanged( int index );
	
	/** \brief Navigation space snapping parameters changed. */
	void NavSpaceSnappingChanged( int index );
	
	/** \brief Navigation space layout changed. */
	void NavSpaceLayoutChanged( int index );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pUpdateHeights();
	void pUpdateHeights( const decPoint &from, const decPoint &to );
};

#endif
