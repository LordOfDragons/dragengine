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

#ifndef _MEUOBJTEXADDPROPERTY_H_
#define _MEUOBJTEXADDPROPERTY_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class meObjectTexture;



/**
 * \brief Undo action object texture add property.
 */
class meUObjTexAddProperty : public igdeUndo{
private:
	meObjectTexture *pTexture;
	
	decString pKey;
	decString pValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo object. */
	meUObjTexAddProperty( meObjectTexture *texture, const char *key, const char *value );
	
protected:
	/** \brief Clean up undo object. */
	virtual ~meUObjTexAddProperty();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set value. */
	void SetValue( const char *value );
	
	/** \brief Undo action. */
	virtual void Undo();
	
	/** \brief Redo action. */
	virtual void Redo();
	
	/** \brief Progressive redo. */
	void ProgressiveRedo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
