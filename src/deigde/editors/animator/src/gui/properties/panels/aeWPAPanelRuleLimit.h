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

#ifndef _AEWPAPANELRULELIMIT_H_
#define _AEWPAPANELRULELIMIT_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>


/**
 * Limit rule panel.
 */
class aeWPAPanelRuleLimit : public aeWPAPanelRule{
private:
	igdeEditVector::Ref pEditMinPos;
	igdeEditVector::Ref pEditMaxPos;
	igdeEditVector::Ref pEditMinRot;
	igdeEditVector::Ref pEditMaxRot;
	igdeEditVector::Ref pEditMinScale;
	igdeEditVector::Ref pEditMaxScale;
	igdeTextField::Ref pEditMinVertexPositionSet;
	igdeTextField::Ref pEditMaxVertexPositionSet;
	igdeComboBox::Ref pCBCoordFrame;
	igdeCheckBox::Ref pChkEnablePosXMin;
	igdeCheckBox::Ref pChkEnablePosXMax;
	igdeCheckBox::Ref pChkEnablePosYMin;
	igdeCheckBox::Ref pChkEnablePosYMax;
	igdeCheckBox::Ref pChkEnablePosZMin;
	igdeCheckBox::Ref pChkEnablePosZMax;
	igdeCheckBox::Ref pChkEnableRotXMin;
	igdeCheckBox::Ref pChkEnableRotXMax;
	igdeCheckBox::Ref pChkEnableRotYMin;
	igdeCheckBox::Ref pChkEnableRotYMax;
	igdeCheckBox::Ref pChkEnableRotZMin;
	igdeCheckBox::Ref pChkEnableRotZMax;
	igdeCheckBox::Ref pChkEnableScaleXMin;
	igdeCheckBox::Ref pChkEnableScaleXMax;
	igdeCheckBox::Ref pChkEnableScaleYMin;
	igdeCheckBox::Ref pChkEnableScaleYMax;
	igdeCheckBox::Ref pChkEnableScaleZMin;
	igdeCheckBox::Ref pChkEnableScaleZMax;
	igdeCheckBox::Ref pChkEnableVertexPositionSetMin;
	igdeCheckBox::Ref pChkEnableVertexPositionSetMax;
	igdeComboBoxFilter::Ref pCBTargetBone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new panel. */
	aeWPAPanelRuleLimit( aeWPRule &wpRule );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAPanelRuleLimit();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** Updates the rule. */
	virtual void UpdateRule();
	/*@}*/
};

#endif
