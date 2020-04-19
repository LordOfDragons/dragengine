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
#ifndef _MEBASEUNDOSCALE_H_
#define _MEBASEUNDOSCALE_H_

// includes
#include <deigde/undo/igdeUndo.h>
#include "dragengine/common/math/decMath.h"



/**
 * Base class for undo actions scaling things.
 */
class meBaseUndoScale : public igdeUndo{
private:
	bool pModifyPosition;
	bool pModifySize;
	decVector pFactors;
	float pUniformFactor;
	decDVector pPivot;
	bool pScaleUniform;
	
public:
	// constructor, destructor
	meBaseUndoScale();
	~meBaseUndoScale();
	
	// management
	/** Determines if the position has to be modified. */
	inline bool GetModifyPosition() const{ return pModifyPosition; }
	/** Sets if the position has to be modified. */
	void SetModifyPosition( bool modifyPosition );
	/** Determines if the size has to be modified. */
	inline bool GetModifySize() const{ return pModifySize; }
	/** Sets if the size has to be modified. */
	void SetModifySize( bool modifySize );
	/** Retrieves the scaling factors. */
	inline const decVector &GetFactors() const{ return pFactors; }
	/** Sets the scaling factors. */
	void SetFactors( const decVector &factors );
	/** Retrieves the pivot point. */
	inline const decDVector &GetPivot() const{ return pPivot; }
	/** Sets the pivot point. */
	void SetPivot( const decDVector &pivot );
	
	/** Retrieves the uniform scaling factor. */
	inline float GetUniformFactor() const{ return pUniformFactor; }
	/** Sets the uniform scaling factor. */
	void SetUniformFactor( float factor );
	/** Determines if the scaling has to be done uniformly. */
	inline bool GetScaleUniform() const{ return pScaleUniform; }
	/** Sets if the scaling has to be done uniformly. */
	void SetScaleUniform( bool scaleUniform );
	
	void Update();
	
	/**
	 * Convenience method to scale one undo element. The position and
	 * scaling are modified in place.
	 */
	void TransformElement( decDVector &position, decVector &scaling );
	
	// undo and redo actions
	virtual void ProgressiveRedo();
};

// end of include only once
#endif
