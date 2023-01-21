/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLLODCALCULATOR_H_
#define _DEOGLLODCALCULATOR_H_

#include <dragengine/common/math/decMath.h>

class deoglCollideList;
class deoglCollideListComponent;



/**
 * Helper class calculating LOD levels to use for different objects. Supports storing results
 * accross multiple frames if required.
 */
class deoglLODCalculator{
private:
	int pMaxPixelError;
	int pLodOffset;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new lod calculator. */
	deoglLODCalculator();
	/** Cleans up the lod calculator. */
	~deoglLODCalculator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Set the maximum pixel error on screen. */
	void SetMaxPixelError( int maxPixelError );
	
	/** Set lod offset. */
	void SetLodOffset( int offset );
	
	/** Set lod level of all components to 0. */
	void SetComponentLOD0( const deoglCollideList &collideList );
	
	/** Set lod level of all components to highest. */
	void SetComponentLODMax( const deoglCollideList &collideList );
	
	/** Calculate projective lod levels for all components. */
	void SetComponentLODProjection( const deoglCollideList &collideList, const decDVector &position,
		const decDVector &view, float fovX, float fovY, int screenWidth, int screenHeight );
	
	/** Calculate orthographic lod levels for all components. */
	void SetComponentLODOrtho( const deoglCollideList &collideList, float boxWidth, float boxHeight,
		int screenWidth, int screenHeight );
	
	/** Calculate orthographic lod levels for all components. */
	void SetComponentLODOrtho( deoglCollideListComponent &clistComponent, float boxWidth,
		float boxHeight, int screenWidth, int screenHeight );
	
	/** Calculate omnidirection lod levels for all components. */
	void SetComponentLODOmniDir( const deoglCollideList &collideList, const decDVector &position, int size );
	/*@}*/
};

#endif
