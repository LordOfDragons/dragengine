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

#ifndef _AEWPAPANELRULEBONTRANSFORMATOR_H_
#define _AEWPAPANELRULEBONTRANSFORMATOR_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>


/**
 * Bone transformator rule panel.
 */
class aeWPAPanelRuleBoneTransformator : public aeWPAPanelRule{
private:
	igdeEditVectorReference pEditMinTrans, pEditMaxTrans, pEditMinRot, pEditMaxRot;
	igdeEditVectorReference pEditMinScale, pEditMaxScale, pEditAxis;
	igdeTextFieldReference pEditMinAngle, pEditMaxAngle;
	igdeComboBoxReference pCBCoordFrame, pCBInputSource;
	igdeCheckBoxReference pChkEnablePosition, pChkEnableRotation, pChkEnableSize, pChkUseAxis;
	igdeComboBoxFilterReference pCBTargetBone, pCBInputBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPAPanelRuleBoneTransformator( aeWPRule &wpRule );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAPanelRuleBoneTransformator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** Update rule. */
	virtual void UpdateRule();
	
	/** Update target list. */
	virtual void UpdateTargetList();
	/*@}*/
};

#endif
