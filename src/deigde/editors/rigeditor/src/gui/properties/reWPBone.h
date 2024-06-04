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

#ifndef _REWPBONE_H_
#define _REWPBONE_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reRig;
class reRigBone;
class reWindowProperties;
class reWPBoneListener;



/**
 * \brief Rig Panel
 */
class reWPBone : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reRigBone *pBone;
	reWPBoneListener *pListener;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBParent;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditCentralMassPoint;
	igdeEditVectorReference pEditIKLimitsLower;
	igdeEditVectorReference pEditIKLimitsUpper;
	igdeEditVectorReference pEditIKResistance;
	igdeTextFieldReference pEditMass;
	igdeCheckBoxReference pChkDynamic;
	igdeCheckBoxReference pChkIKLockedX;
	igdeCheckBoxReference pChkIKLockedY;
	igdeCheckBoxReference pChkIKLockedZ;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPBone( reWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPBone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rig. */
	inline reRig *GetRig() const{ return pRig; }
	
	/** \brief Set rig. */
	void SetRig( reRig *rig );
	
	/** \brief Bone. */
	inline reRigBone *GetBone() const{ return pBone; }
	
	/** \brief Set bone. */
	void SetBone( reRigBone *bone );
	
	/** \brief Update list of possible parent bone names. */
	void UpdateParentBoneList();
	
	/** \brief Update bone. */
	void UpdateBone();
	/*@}*/
};

#endif
