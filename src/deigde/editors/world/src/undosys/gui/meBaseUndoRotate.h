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

// include only once
#ifndef _MEBASEUNDOROTATE_H_
#define _MEBASEUNDOROTATE_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include "dragengine/common/math/decMath.h"



/**
 * Base class for undo actions rotating things around.
 */
class meBaseUndoRotate : public igdeUndo{
private:
	float pAngle;
	decDVector pPivot;
	decDVector pAxis;
	bool pModifyPosition;
	bool pModifyOrientation;
	decDMatrix pMatrix;
	
public:
	// constructor, destructor
	meBaseUndoRotate();
	~meBaseUndoRotate();
	
	// management
	/** Retrieves the angle to rotate in degrees. */
	inline float GetAngle() const{ return pAngle; }
	/** Sets the angle to rotate in degrees. */
	void SetAngle( float angle );
	/** Retrieves the pivot point. */
	inline const decDVector &GetPivot() const{ return pPivot; }
	/** Sets the pivot point. */
	void SetPivot( const decDVector &pivot );
	/** Retrieves the rotation axis. */
	inline const decDVector &GetAxis() const{ return pAxis; }
	/** Sets the rotation axis. */
	void SetAxis( const decDVector &axis );
	
	/** Determines if the position has to be modified. */
	inline bool GetModifyPosition() const{ return pModifyPosition; }
	/** Sets if the position has to be modified. */
	void SetModifyPosition( bool modifyPosition );
	/** Determines if the orientation has to be modified. */
	inline bool GetModifyOrientation() const{ return pModifyOrientation; }
	/** Sets if the orientation has to be modified. */
	void SetModifyOrientation( bool modifyOrientation );
	
	/** Retrieves the rotation matrix. */
	inline const decDMatrix &GetRotationMatrix() const{ return pMatrix; }
	/** Updates the rotation matrix. */
	void UpdateRotationMatrix();
	
	/**
	 * Convenience method to rotate one undo element. The position and
	 * rotation are modified in place.
	 */
	void TransformElement( decDVector &position, decDVector &rotation );
	
	// undo and redo actions
	virtual void ProgressiveRedo();
};

// end of include only once
#endif
