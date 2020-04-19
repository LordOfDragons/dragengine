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

#ifndef _DETERRAINHEIGHTMODIFIER_H_
#define _DETERRAINHEIGHTMODIFIER_H_

#include "../../../common/math/decMath.h"

class deImage;


/**
 * \brief Terrain Height Modifier.
 *
 * Defines a small grid of height modifiers. The grid is overlayed on an
 * existing height map terrain mesh and alters the heights by adding a
 * height difference from an image. Height modifiers can protrude outside
 * the the original height map. All points outside the valid range are
 * ignored. The height modifier works similar to the height map itself.
 * The position is the upper left corner of the height modifier which is
 * on the negative X and positive Z side if looked from the terrain.
 */
class deTerrainHeightModifier{
private:
	deImage *pHeightImage;
	decPoint pPosition;
	float pScaling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new height modifier. */
	deTerrainHeightModifier( deImage *heightImage );
	
	/** \brief Clean up height modifier. */
	~deTerrainHeightModifier();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Position of the upper left corner. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position of the upper left corner. */
	void SetPosition( const decPoint &position );
	
	/** \brief Scaling. */
	inline float GetScaling() const{ return pScaling; }
	
	/** \brief Set scaling. */
	void SetScaling( float scaling );
	
	/** \brief Height image. */
	inline deImage *GetHeightImage() const{ return pHeightImage; }
	/*@}*/
};

#endif
