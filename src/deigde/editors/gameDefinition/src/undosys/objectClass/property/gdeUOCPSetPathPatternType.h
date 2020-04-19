/* 
 * Drag[en]gine IGDE Game Definition Editor
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


#ifndef _GDEUOCPSETPATHPATTERNTYPE_H_
#define _GDEUOCPSETPATHPATTERNTYPE_H_

#include "../../../gamedef/property/gdeProperty.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class gdeObjectClass;



/**
 * \brief Undo action object class property set path pattern type.
 */
class gdeUOCPSetPathPatternType : public igdeUndo{
private:
	gdeObjectClass *pObjectClass;
	gdeProperty *pProperty;
	
	gdeProperty::ePathPatternTypes pOldValue;
	gdeProperty::ePathPatternTypes pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCPSetPathPatternType( gdeObjectClass *objectClass, gdeProperty *property, gdeProperty::ePathPatternTypes newValue );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCPSetPathPatternType();
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
