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

#ifndef _MEUWORLDSETPROPERTY_H_
#define _MEUWORLDSETPROPERTY_H_

#include <deigde/undo/igdeUndo.h>

class meWorld;



/**
 * \brief Undo action world set property.
 */
class meUWorldSetProperty : public igdeUndo{
private:
	meWorld *pWorld;
	
	decString pKey;
	decString pOldValue;
	decString pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	meUWorldSetProperty( meWorld *world, const char *key, const char *oldValue, const char *newValue );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUWorldSetProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set new value. */
	void SetNewValue( const char *value );
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	
	/** \brief Progressive redo. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
