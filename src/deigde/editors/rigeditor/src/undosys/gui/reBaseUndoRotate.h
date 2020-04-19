/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REBASEUNDOROTATE_H_
#define _REBASEUNDOROTATE_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>



/**
 * Base class for undo actions rotating things around.
 */
class reBaseUndoRotate : public igdeUndo{
private:
	float pAngle;
	decPoint3 pCenterSector;
	decVector pCenterPosition, pAxis;
	bool pModifyPosition;
	bool pModifyOrientation;
	decMatrix pMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reBaseUndoRotate();
	
protected:
	/** \brief Clean up undo. */
	virtual ~reBaseUndoRotate();
	/*@}*/
	
	
	
	
public:
	/** \name Management */
	/*@{*/
	inline float GetAngle() const{ return pAngle; }
	void SetAngle( float angle );
	inline const decPoint3 &GetCenterSector() const{ return pCenterSector; }
	void SetCenterSector( const decPoint3 &sector );
	inline const decVector &GetCenterPosition() const{ return pCenterPosition; }
	void SetCenterPosition( const decVector &position );
	inline const decVector &GetAxis() const{ return pAxis; }
	void SetAxis( const decVector &axis );
	inline bool GetModifyPosition() const{ return pModifyPosition; }
	void SetModifyPosition( bool modifyPosition );
	inline bool GetModifyOrientation() const{ return pModifyOrientation; }
	void SetModifyOrientation( bool modifyOrientation );
	inline const decMatrix &GetRotationMatrix() const{ return pMatrix; }
	void Update();
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
