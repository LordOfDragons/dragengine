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

// include only once
#ifndef _RESELECTIONBONES_H_
#define _RESELECTIONBONES_H_

#include "reRigBone.h"

// predefinitions
class reRig;
class deColliderVolume;



/**
 * @brief Rig Bone Selection.
 * Stores a list of selected rig bones as well as the active rig bone.
 */
class reSelectionBones{
private:
	reRig *pRig;
	
	reRigBone::List pBones;
	reRigBone::Ref pActiveBone;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rig bone selection. */
	reSelectionBones(reRig *rig);
	/** Cleans up the rig bone selection. */
	~reSelectionBones();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent rig. */
	inline reRig *GetParentRig() const{ return pRig; }
	
	/** List of selected bones. */
	inline const reRigBone::List &GetBones() const{ return pBones; }
	
	/** Retrieves the index of the bone with the given collider or -1 if not found. */
	int IndexOfBoneWith(deColliderVolume *collider) const;
	/** Adds a bone if not existing already. */
	void AddBone(reRigBone *bone);
	/** Removes a bone if existing. */
	void RemoveBone(reRigBone *bone);
	/** Removes all bones. */
	void RemoveAllBones();
	
	/** Retrieves the active bone or nullptr. */
	inline const reRigBone::Ref &GetActiveBone() const{ return pActiveBone; }
	/** Determines if an active bone exists. */
	bool HasActiveBone() const;
	/** Sets the acitve bone or nullptr. */
	void SetActiveBone(reRigBone *bone);
	
	/** Removes all bones and sets the active bone to nullptr. */
	void Reset();
	/*@}*/
};

// end of include only once
#endif
