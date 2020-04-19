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


#ifndef _GDEUOCBILLBOARDSETSIZE_H_
#define _GDEUOCBILLBOARDSETSIZE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class gdeOCBillboard;
class gdeObjectClass;



/**
 * \brief Undo action object class billboard set size.
 */
class gdeUOCBillboardSetSize : public igdeUndo{
private:
	gdeObjectClass *pObjectClass;
	gdeOCBillboard *pBillboard;
	
	decVector2 pOldValue;
	decVector2 pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCBillboardSetSize( gdeObjectClass *objectClass,
		gdeOCBillboard *billboard, const decVector2 &newValue );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCBillboardSetSize();
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
