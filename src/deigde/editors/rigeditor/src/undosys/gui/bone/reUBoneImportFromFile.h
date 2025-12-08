/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
 * constraints but not the geometry information like the position or rotation.
 * Existing shapes and constraints are replaced with the imported ones.
 */
class reUBoneImportFromFile : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<reUBoneImportFromFile> Ref;
	
	
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
		reRigBone::Ref bone;
		reRigBone::Ref importBone;
		
		decVector oldCMP;
		float oldMass;
		bool oldDynamic;
		reRigShapeList *oldShapes;
		reRigConstraintList *oldConstraints;
		decVector oldIKLimitsLower;
		decVector oldIKLimitsUpper;
		decVector oldIKResistance;
		bool oldIKLocked[3];
	};
	
	
	
private:
	reRig::Ref pRig;
	
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
	reUBoneImportFromFile(reRig::Ref rig, reRig *importedRig);
	
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
	void SetScale(float scale);
	
	/** \brief Bone properties are imported. */
	inline bool GetImportBoneProperties() const{ return pImportBoneProperties; }
	
	/** \brief Set if bone properties are imported. */
	void SetImportBoneProperties(bool import);
	
	/** \brief Shapes are imported. */
	inline bool GetImportShapes() const{ return pImportShapes; }
	
	/** \brief Set if shapes are imported. */
	void SetImportShapes(bool import);
	
	/** \brief Constraints are imported. */
	inline bool GetImportConstraints() const{ return pImportConstraints; }
	
	/** \brief Set if constraints are imported. */
	void SetImportConstraints(bool import);
	
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
