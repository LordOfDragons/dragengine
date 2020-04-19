/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEWCOORDSYSARROWS_H_
#define _IGDEWCOORDSYSARROWS_H_

#include "igdeWDebugDrawerShape.h"



/**
 * \brief Coordinate system arrows wrapper.
 * 
 * Draws a coordinate system using debug drawer shapes. Each arrow has an own
 * color to indicate the coordinate axis it refers to. The coordinate system
 * is given by a position and orientation.
 */
class igdeWCoordSysArrows{
private:
	igdeWDebugDrawerShape pDDSAxisX;
	igdeWDebugDrawerShape pDDSAxisY;
	igdeWDebugDrawerShape pDDSAxisZ;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decVector pScale;
	
	decColor pColorAxisX;
	decColor pColorAxisY;
	decColor pColorAxisZ;
	
	float pArrowLength;
	float pArrowSize;
	
	bool pVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWCoordSysArrows();
	
	/** \brief Clean up wrapper. */
	~igdeWCoordSysArrows();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Set parent debug drawer or NULL. */
	void SetParentDebugDrawer( deDebugDrawer *debugDrawer );
	
	/** \brief Position. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Scale. */
	inline const decVector &GetScale() const{ return pScale; }
	
	/** \brief Set scale. */
	void SetScale( const decVector &scale );
	
	/** \brief X axis color. */
	inline const decColor &GetColorAxisX() const{ return pColorAxisX; }
	
	/** \brief Set x axis color. */
	void SetAxisXColor( const decColor &color );
	
	/** \brief Y axis color. */
	inline const decColor &GetColorAxisY() const{ return pColorAxisY; }
	
	/** \brief Set y axis color. */
	void SetAxisYColor( const decColor &color );
	
	/** \brief Z axis color. */
	inline const decColor &GetColorAxisZ() const{ return pColorAxisZ; }
	
	/** \brief Set z axis color. */
	void SetAxisZColor( const decColor &color );
	
	/** \brief Length of the arrows. */
	inline float GetArrowLength() const{ return pArrowLength; }
	
	/** \brief Set length of the arrows. */
	void SetArrowLength( float length );
	
	/** \brief Size of the arrow heads. */
	inline float GetArrowSize() const{ return pArrowSize; }
	
	/** \brief Set size of the arrow heads. */
	void SetArrowSize( float size );
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible( bool visible );
	/*@}*/
	
	
	
private:
	void pRebuildArrows();
};

#endif
