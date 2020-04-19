/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEUPARAMETERSETVALUE_H_
#define _PEEUPARAMETERSETVALUE_H_

#include <deigde/undo/igdeUndo.h>

class peeType;
class peeParameter;



/**
 * \brief Undo Action Set Parameter Value.
 */
class peeUParameterSetValue : public igdeUndo{
private:
	peeType *pType;
	peeParameter *pParameter;
	
	float pOldValue;
	float pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new undo action. */
	peeUParameterSetValue( peeType *type, peeParameter *parameter, float newValue );
	
protected:
	/** \brief Clean up the undo action. */
    virtual ~peeUParameterSetValue();
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
