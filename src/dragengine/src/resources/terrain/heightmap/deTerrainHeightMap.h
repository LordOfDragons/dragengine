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

#ifndef _DETERRAINHEIGHTMAP_H_
#define _DETERRAINHEIGHTMAP_H_

#include "../../../common/math/decMath.h"

class deImage;

// definitions
/** 8-bit image value corresponding to 0 level. */
#define THM_8BIT_BASE		127
/** 16-bit image value corresponding to 0 level. */
#define THM_16BIT_BASE		32767

/** 8-bit scaling factor height to pixel. */
#define THM_8BIT_HTOP		255.0f
/** 16-bit scaling factor height to pixel. */
#define THM_16BIT_HTOP		65535.0f

/** 8-bit scaling factor pixel to height. */
#define THM_8BIT_PTOH		( 1.0f / 255.0f )
/** 16-bit scaling factor pixel to height. */
#define THM_16BIT_PTOH		( 1.0f / 65535.0f )


/**
 * \brief Terrain Height Map.
 *
 * Defines a grid like terrain mesh using a height map. The height of grid vertices
 * is defined by the corresponding pixel in a height image. As height image only
 * 1-component images can be used. Any bit depth is possible allowing for fine
 * grained resolution if required. The optional scaling and offset parameter alter
 * the height retrieved from the height map before being applied to grid points.
 * Individual grid faces can be hidden using a list of rectangular areas. All grid
 * faces inside the union of these rectangular areas are not drawn and do not
 * influence physics or audio propagation. If the height map is missing all grid
 * points take on the height of the offset parameter.
 */
class deTerrainHeightMap{
private:
	decPoint pPointCount;
	decVector2 pSize;
	float pScaling;
	
	char *pPathHeightImage;
	deImage *pHeightImage;
	
	char *pPathVisImage;
	unsigned char *pVisibleFaces;
	int pVFByteCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new terrain mesh grid object. */
	deTerrainHeightMap();
	
	/** \brief Clean up terrain grid object. */
	~deTerrainHeightMap();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of points. */
	inline const decPoint &GetPointCount() const{ return pPointCount; }
	
	/** \brief Set number of points. */
	void SetPointCount( const decPoint &pointCount );
	
	/** \brief Size of the grid. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** \brief Set size of the grid. */
	void SetSize( const decVector2 &size );
	
	/** \brief Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( float scaling );
	
	/** \brief Path of the image used to store the height map or NULL if not used. */
	inline const char *GetPathHeightImage() const{ return ( const char * )pPathHeightImage; }
	
	/** \brief Set path of the image used to store the height map or NULL if not used. */
	void SetPathHeightImage( const  char *path );
	
	/** \brief Height image or NULL if not set. */
	inline deImage *GetHeightImage() const{ return pHeightImage; }
	
	/** \brief Set height image or NULL if not set. */
	void SetHeightImage( deImage *heightImage );
	
	/** \brief Calculates the extends. */
	void CalculateExtends( decVector &minExtend, decVector &maxExtend );
	/*@}*/
	
	
	
	/** \name Face Visibility */
	/*@{*/
	/** \brief Path of the image used to store the visibility map or NULL if not used. */
	inline const char *GetPathVisibilityImage() const{ return ( const char * )pPathVisImage; }
	
	/** \brief Set path of the image used to store the visibility map or NULL if not used. */
	void SetPathVisibilityImage( const  char *path );
	
	/** \brief Face with the given upper left coordinate is visible. */
	bool GetFaceVisibleAt( int x, int y );
	
	/** \brief Set if face at the given upper left coordinate is visible. */
	void SetFaceVisibleAt( int x, int y, bool visible );
	
	/** \brief Set visibility for all faces. */
	void SetAllFacesVisible( bool visible );
	/*@}*/
	
	
	
private:
	void pCreateVisibleFaces();
};

#endif
