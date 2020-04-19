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

#ifndef _REUBONEIMPORTFROMFILE_H_
#define _REUBONEIMPORTFROMFILE_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

class reRig;
class reRigBone;
class reRigShapeList;
class reRigConstraintList;



/**
 * \brief Undo Import Bones From File.
 *
 * Undo action to import bones from file. This includes the physics  parameters, shapes and
 * constraints but not the geometry informations like the position or rotation.
 * Existing shapes and constraints are replaced with the imported ones.
 */
class reUBoneImportFromFile : public igdeUndo{
private:
	struct sBoneState{
		decVector cmp;
		float mass;
		bool dynamic;
		reRigBone *parentBone;
		reRigShapeList *shapes;
		reRigConstraintList *constraints;
	};
	
	struct sBone{
		reRigBone *bone;
		reRigBone *importBone;
		
		decVector oldCMP;
		float oldMass;
		bool oldDynamic;
		reRigShapeList *oldShapes;
		reRigConstraintList *oldConstraints;
	};
	
	
	
private:
	reRig *pRig;
	
	float pScale;
	bool pImportBoneProperties;
	bool pImportShapes;
	bool pImportConstraints;
	
	sBone *pBones;
	int pBoneCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	reUBoneImportFromFile( reRig *rig, reRig *importedRig );
	
protected:
	/** \brief Clean up undo. */
	virtual ~reUBoneImportFromFile();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Scale factor. */
	inline float GetScale() const{ return pScale; }
	
	/** \brief Set scale factor. */
	void SetScale( float scale );
	
	/** \brief Bone properties are imported. */
	inline bool GetImportBoneProperties() const{ return pImportBoneProperties; }
	
	/** \brief Set if bone properties are imported. */
	void SetImportBoneProperties( bool import );
	
	/** \brief Shapes are imported. */
	inline bool GetImportShapes() const{ return pImportShapes; }
	
	/** \brief Set if shapes are imported. */
	void SetImportShapes( bool import );
	
	/** \brief Constraints are imported. */
	inline bool GetImportConstraints() const{ return pImportConstraints; }
	
	/** \brief Set if constraints are imported. */
	void SetImportConstraints( bool import );
	
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
