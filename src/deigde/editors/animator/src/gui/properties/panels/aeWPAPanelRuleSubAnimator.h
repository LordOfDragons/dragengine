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

#ifndef _AEWPAPANELRULESUBANIMATOR_H_
#define _AEWPAPANELRULESUBANIMATOR_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/composed/igdeEditPathReference.h>


class aeController;


/**
 * Sub Animator Rule Panel.
 */
class aeWPAPanelRuleSubAnimator : public aeWPAPanelRule{
private:
	igdeEditPathReference pEditPathAnimator;
	igdeComboBoxReference pCBConnection;
	igdeComboBoxReference pCBConnectionController;
	igdeCheckBoxReference pChkEnablePosition;
	igdeCheckBoxReference pChkEnableRotation;
	igdeCheckBoxReference pChkEnableSize;
	igdeCheckBoxReference pChkEnableVertexPositionSet;
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPAPanelRuleSubAnimator( aeWPRule &wpRule );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAPanelRuleSubAnimator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update connection target list. */
	void UpdateConnectionList();
	
	/** Animator path changed. */
	virtual void OnAnimatorPathChanged();
	
	/** Update controller list. */
	virtual void UpdateControllerList();
	
	/** Update rule. */
	virtual void UpdateRule();
	
	/** Update connection. */
	virtual void UpdateConnection();
	
	/** Selected combobox connection. */
	int GetCBConnection() const;
	
	/** Selected combobox connection controller. */
	aeController *GetCBConnectionController() const;
	/*@}*/
};

#endif
