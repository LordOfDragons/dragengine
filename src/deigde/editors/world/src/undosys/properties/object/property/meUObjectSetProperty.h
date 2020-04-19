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

#ifndef _MEUOBJECTSETPROPERTY_H_
#define _MEUOBJECTSETPROPERTY_H_

#include <deigde/undo/igdeUndo.h>

class meObject;



/**
 * \brief Undo Action Set Object Property.
 */
class meUObjectSetProperty : public igdeUndo{
private:
	meObject *pObject;
	
	decString pKey;
	decString pOldValue;
	decString pNewValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectSetProperty( meObject *object, const char *key, const char *oldValue, const char *newValue );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectSetProperty();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Sets the new value. */
	void SetNewValue( const char *value );
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/** Progressive redo. */
	void ProgressiveRedo();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
