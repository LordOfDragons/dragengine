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

#ifndef _REUBONEMIRROR_H_
#define _REUBONEMIRROR_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class reRig;
class reRigBone;
class reRigShapeList;
class reRigConstraintList;



/**
 * \brief Undo Mirror Bone.
 *
 * Undo action to mirror bones. This includes the physics parameters,
 * shapes and constraints but not the geometry information like the
 * position or rotation since these usually have to stay the same.
 * Bones have to be end with .l or .r for right respective left. For
 * each bone to mirror the opposite bone is looked for and the
 * parameters copied but mirrored along the x axis.
 */
class reUBoneMirror : public igdeUndo{
private:
	struct sBone{
		reRigBone *boneSource;
		
		reRigBone *boneTarget;
		decVector oldCMP;
		float oldMass;
		bool oldDynamic;
		reRigShapeList *oldShapes;
		reRigConstraintList *oldConstraints;
	};
	
	
	
private:
	reRig *pRig;
	
	sBone *pBones;
	int pBoneCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new undo. */
	reUBoneMirror( reRig *rig );
	
protected:
	/** \brief Clean up the undo. */
	virtual ~reUBoneMirror();
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
	
	reRigBone *pGetBoneWithMirroredName( reRig *rig, reRigBone *bone ) const;
};

#endif
