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

#ifndef _AEWPAPANELRULESTATEMANIPULATOR_H_
#define _AEWPAPANELRULESTATEMANIPULATOR_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditVector.h>



/**
 * State Manipulator Rule Panel.
 */
class aeWPAPanelRuleStateManipulator : public aeWPAPanelRule{
private:
	igdeEditVector::Ref pEditMinPos;
	igdeEditVector::Ref pEditMaxPos;
	igdeEditVector::Ref pEditMinRot;
	igdeEditVector::Ref pEditMaxRot;
	igdeEditVector::Ref pEditMinSize;
	igdeEditVector::Ref pEditMaxSize;
	igdeTextField::Ref pEditMinVertexPositionSet;
	igdeTextField::Ref pEditMaxVertexPositionSet;
	igdeCheckBox::Ref pChkEnablePosition;
	igdeCheckBox::Ref pChkEnableRotation;
	igdeCheckBox::Ref pChkEnableSize;
	igdeCheckBox::Ref pChkEnableVertexPositionSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new panel. */
	aeWPAPanelRuleStateManipulator(aeWPRule &wpRule);
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAPanelRuleStateManipulator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update rule. */
	virtual void UpdateRule();
	
	/** Update target list. */
	virtual void UpdateTargetList();
	/*@}*/
};

#endif
