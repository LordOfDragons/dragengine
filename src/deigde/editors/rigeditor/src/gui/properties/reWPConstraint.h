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

#ifndef _REWPCONSTRAINT_H_
#define _REWPCONSTRAINT_H_

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class reWindowProperties;
class reRig;
class reRigConstraint;
class reWPConstraintListener;



/**
 * \brief Constraint panel.
 */
class reWPConstraint : public igdeContainerScroll{
private:
	reWindowProperties &pWindowProperties;
	reRig *pRig;
	reRigConstraint *pConstraint;
	reWPConstraintListener *pListener;
	
	igdeTextField::Ref pEditBoneParent;
	igdeComboBox::Ref pCBBoneTarget;
	igdeEditVector::Ref pEditPosition;
	igdeEditVector::Ref pEditRotation;
	igdeEditVector::Ref pEditOffset;
	
	igdeTextField::Ref pEditDofLower[6];
	igdeTextField::Ref pEditDofUpper[6];
	igdeTextField::Ref pEditDofStaFric[6];
	igdeTextField::Ref pEditDofKinFric[6];
	igdeTextField::Ref pEditDofSprStiff[6];
	
	igdeCheckBox::Ref pChkShowJointError;
	
	igdeTextField::Ref pEditDampLin;
	igdeTextField::Ref pEditDampAng;
	igdeTextField::Ref pEditDampSpr;
	
	igdeCheckBox::Ref pChkRope;
	
	igdeTextField::Ref pEditBreakingThreshold;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	reWPConstraint(reWindowProperties &windowProperties);
	
protected:
	/** \brief Clean up panel. */
	virtual ~reWPConstraint();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Constraint. */
	inline reRigConstraint *GetConstraint() const{return pConstraint;}
	
	/** \brief Rig. */
	inline reRig *GetRig() const{return pRig;}
	
	/** \brief Set rig. */
	void SetRig(reRig *rig);
	
	/** \brief Set constraint. */
	void SetConstraint(reRigConstraint *constraint);
	
	/** \brief Update bone lists. */
	void UpdateBoneLists();
	
	/** \brief Update constaint. */
	void UpdateConstraint();
	/*@}*/
};

#endif
