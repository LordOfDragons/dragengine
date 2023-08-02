/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _SEUMAPPEDSETRENDERABLE_H_
#define _SEUMAPPEDSETRENDERABLE_H_

#include "../../skin/mapped/seMapped.h"

#include <deigde/undo/igdeUndo.h>



/**
 * Undo action set mapped renderable.
 */
class seUMappedSetRenderable : public igdeUndo{
private:
	const seMapped::Ref pMapped;
	
	const decString pOldValue;
	const decString pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	seUMappedSetRenderable( seMapped *mapped, const char *newRenderable );
	
protected:
	/** Clean up undo. */
	virtual ~seUMappedSetRenderable();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo action. */
	virtual void Undo();
	
	/** Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
