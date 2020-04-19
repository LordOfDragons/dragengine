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

#ifndef _MEUOBJECTREMOVEPROPERTY_H_
#define _MEUOBJECTREMOVEPROPERTY_H_

#include <deigde/undo/igdeUndo.h>

class meObject;



/**
 * \brief Undo Action Remove Object Property.
 */
class meUObjectRemoveProperty : public igdeUndo{
private:
	meObject *pObject;
	
	decString pKey;
	decString pValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo object. */
	meUObjectRemoveProperty( meObject *object, const char *key, const char *value );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjectRemoveProperty();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
