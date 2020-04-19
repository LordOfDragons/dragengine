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

#ifndef _MEBASEUNDOMOVE_H_
#define _MEBASEUNDOMOVE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>



/**
 * \brief Base class for undo action moving things around.
 */
class meBaseUndoMove : public igdeUndo{
private:
	decDVector pDistance;
	
	bool pModifyOrientation;
	decDMatrix pMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	meBaseUndoMove();
	
	/** \brief Clean up undo action. */
	virtual ~meBaseUndoMove();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Distance to move. */
	inline const decDVector &GetDistance() const{ return pDistance; }
	
	/** \brief Set distance to move. */
	void SetDistance( const decDVector &distance );
	
	/** \brief Orientation modification enabled. */
	inline bool GetModifyOrientation() const{ return pModifyOrientation; }
	
	/** \brief Set if orientation modification is enabled. */
	void SetModifyOrientation( bool modify );
	
	/** \brief Transformation matrix used only if orientation is modified. */
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	
	/** \brief Set transformation matrix used only if orientation is modified. */
	void SetMatrix( const decDMatrix &matrix );
	
	
	
	/**
	 * \brief Convenience method to transform element.
	 * 
	 * Position and orientation are modified in place.
	 */
	void TransformElement( decDVector &position, decDVector &rotation );
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
