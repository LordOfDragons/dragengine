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

#ifndef _AEWPAPANELRULEANIMATIONDIFFERENCE_H_
#define _AEWPAPANELRULEANIMATIONDIFFERENCE_H_

#include "aeWPAPanelRule.h"



/**
 * Animation Difference Rule Panel.
 */
class aeWPAPanelRuleAnimationDifference : public aeWPAPanelRule{
public:
	typedef deTObjectReference<aeWPAPanelRuleAnimationDifference> Ref;
	
	
private:
	igdeComboBoxFilter::Ref pCBLMoveName;
	igdeTextField::Ref pEditLMoveTime;
	
	igdeComboBoxFilter::Ref pCBRMoveName;
	igdeTextField::Ref pEditRMoveTime;
	
	igdeCheckBox::Ref pChkEnablePosition;
	igdeCheckBox::Ref pChkEnableRotation;
	igdeCheckBox::Ref pChkEnableSize;
	igdeCheckBox::Ref pChkEnableVertexPositionSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPAPanelRuleAnimationDifference(aeWPRule &wpRule);
	
protected:
	/** Clean up panel. */
	~aeWPAPanelRuleAnimationDifference() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update animation move list. */
	void UpdateAnimMoveList() override;
	
	/** Update rule. */
	void UpdateRule() override;
	
	/** Update target list. */
	void UpdateTargetList() override;
	/*@}*/
};

#endif
