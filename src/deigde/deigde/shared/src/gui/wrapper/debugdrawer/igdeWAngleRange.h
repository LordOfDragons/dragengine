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

#ifndef _IGDEWANGLERANGE_H_
#define _IGDEWWANGLERANGE_H_

#include "igdeWDebugDrawerShape.h"



/**
 * \brief Debug drawer shape wrapper angle range.
 * 
 * Draws a circle segment representing a 3d angle using a debug drawe shape.
 * The angle disk is oriented along the Z axis of the wrapper object with the
 * lower and upper angle bounds choosable. The limits are measured as rotations
 * around the Z axis.
 */
class igdeWAngleRange{
private:
	igdeWDebugDrawerShape pDDSDisk;
	
	decVector pPosition;
	decQuaternion pOrientation;
	decColor pColorDiskLine;
	decColor pColorDiskArea;
	
	float pLower;
	float pUpper;
	float pRadius;
	
	bool pVisible;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create wrapper. */
	igdeWAngleRange();
	
	/** \brief Clean up wrapper. */
	~igdeWAngleRange();
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
	
	/** \brief Disk line color. */
	inline const decColor &GetColorDiskLine() const{ return pColorDiskLine; }
	
	/** \brief Set disk line color. */
	void SetDiskLineColor( const decColor &color );
	
	/** \brief Disk area color. */
	inline const decColor &GetColorDiskArea() const{ return pColorDiskArea; }
	
	/** \brief Set disk area color. */
	void SetDiskAreaColor( const decColor &color );
	
	/** \brief Lower angle. */
	inline float GetLower() const{ return pLower; }
	
	/** \brief Upper angle. */
	inline float GetUpper() const{ return pUpper; }
	
	/** \brief Set lower and upper angle. */
	void SetRange( float lower, float upper );
	
	/** \brief Disk radius. */
	inline float GetRadius() const{ return pRadius; }
	
	/** \brief Set disk radius. */
	void SetRadius( float radius );
	
	/** \brief Determines if the debug drawer shape is visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** \brief Sets if the debug drawer shape is visible. */
	void SetVisible( bool visible );
	/*@}*/
	
	
	
private:
	void pRebuildShape();
};

#endif
