/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEUCONTROLLERSETDEFAULTVALUE_H_
#define _AEUCONTROLLERSETDEFAULTVALUE_H_

#include "../../animator/controller/aeController.h"
#include <deigde/undo/igdeUndo.h>
#include <dragengine/deObjectReference.h>


/**
 * Undo action controller set default value.
 */
class aeUControllerSetDefaultValue : public igdeUndo{
private:
	const aeController::Ref pController;
	float pOldValue;
	float pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeUControllerSetDefaultValue( aeController *controller, float newValue );
	
protected:
	/** Clean up undo. */
	virtual ~aeUControllerSetDefaultValue();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
