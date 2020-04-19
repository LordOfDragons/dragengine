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

#ifndef _REBASEUNDOSCALE_H_
#define _REBASEUNDOSCALE_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>



/**
 * \brief Base class for undo actions scaling things.
 */
class reBaseUndoScale : public igdeUndo{
private:
	bool pModifyPosition;
	bool pModifySize;
	decVector pFactors;
	decVector pCenter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reBaseUndoScale();
	
protected:
	virtual ~reBaseUndoScale();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	inline bool GetModifyPosition() const{ return pModifyPosition; }
	void SetModifyPosition( bool modifyPosition );
	inline bool GetModifySize() const{ return pModifySize; }
	void SetModifySize( bool modifySize );
	inline const decVector &GetFactors() const{ return pFactors; }
	void SetFactors( const decVector &factors );
	inline const decVector &GetCenter() const{ return pCenter; }
	void SetCenter( const decVector &center );
	
	void Update();
	
	/** \brief Progressive redo. */
	virtual void ProgressiveRedo();
	/*@}*/
};

#endif
