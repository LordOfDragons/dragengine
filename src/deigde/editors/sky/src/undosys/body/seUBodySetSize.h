/* 
 * Drag[en]gine IGDE Sky Editor
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

#ifndef _SEUBODYSETSIZE_H_
#define _SEUBODYSETSIZE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class seBody;


/**
 * \brief Undo action set body size.
 */
class seUBodySetSize : public igdeUndo{
private:
	seBody *pBody;
	
	decVector2 pOldSize;
	decVector2 pNewSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	seUBodySetSize( seBody *body, const decVector2 &newSize );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~seUBodySetSize();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
