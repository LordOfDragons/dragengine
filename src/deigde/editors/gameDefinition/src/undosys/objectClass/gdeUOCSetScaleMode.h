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


#ifndef _GDEUOBJECTCLASSSETSCALEMODE_H_
#define _GDEUOBJECTCLASSSETSCALEMODE_H_

#include "../../gamedef/objectClass/gdeObjectClass.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>



/**
 * \brief Undo action object class set scale mode.
 */
class gdeUOCSetScaleMode : public igdeUndo{
private:
	gdeObjectClass *pObjectClass;
	
	gdeObjectClass::eScaleModes pOldValue;
	gdeObjectClass::eScaleModes pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCSetScaleMode( gdeObjectClass *objectClass, gdeObjectClass::eScaleModes newValue );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCSetScaleMode();
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
