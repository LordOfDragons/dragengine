/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REUADDSHAPE_H_
#define _REUADDSHAPE_H_

#include <deigde/undo/igdeUndo.h>

class reRig;
class reRigBone;
class reRigShape;



/**
 * \brief Undo Add Shape.
 */
class reUAddShape : public igdeUndo{
private:
	reRig *pRig;
	reRigBone *pBone;
	reRigShape *pShape;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reUAddShape( reRig *rig, reRigBone *bone, reRigShape *shape );
	
protected:
	/** \brief Clean up undo. */
	virtual ~reUAddShape();
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
	reRig *pGetRig();
};

#endif
