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

#ifndef _REUBONESCALEMASS_H_
#define _REUBONESCALEMASS_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class decObjectOrderedSet;
class reRig;
class reRigBone;



/**
 * \brief Undo scale bone mass.
 * Undo action scaling the mass of a list of bones to match a new summed mass. The mass of
 * each bone is scale relative to the summed mass of all bones.
 */
class reUBoneScaleMass : public igdeUndo{
private:
	struct sBone{
		reRigBone *bone;
		float oldMass;
		float newMass;
	};
	
private:
	reRig *pRig;
	
	float pNewMass;
	
	sBone *pBones;
	int pBoneCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new undo. */
	reUBoneScaleMass( reRig *rig, const decObjectOrderedSet &bones, float newMass );
	
protected:
	/** \brief Clean up the undo. */
	virtual ~reUBoneScaleMass();
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
