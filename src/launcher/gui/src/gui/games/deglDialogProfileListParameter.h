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

#ifndef _DEGLDIALOGPROFILELISTPARAMETER_H_
#define _DEGLDIALOGPROFILELISTPARAMETER_H_

#include "../foxtoolkit.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/deModuleParameter.h>

class delEMParameter;
class delGameProfile;


/**
 * Profiles panel module parameter.
 */
class deglDialogProfileListParameter : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglDialogProfileListParameter> Ref;
	
	
private:
	delEMParameter &pParameter;
	delGameProfile &pProfile;
	decString pModuleName;
	
	decString pDescription;
	
	FXLabel *pLabel;
	FXComboBox *pComboBox;
	FXHorizontalFrame *pFrameSlider;
	FXRealSlider *pSlider;
	FXTextField *pSliderText;
	FXCheckButton *pCheckBox;
	FXTextField *pTextField;
	
	bool pCustomized;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter. */
	deglDialogProfileListParameter(delEMParameter &parameter, delGameProfile &profile,
		const char *moduleName, FXMatrix *container, FXObject *target, 
		int labelSelector, int valueSelector);
	
protected:
	/** Clean up parameter. */
	virtual ~deglDialogProfileListParameter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Description. */
	inline const decString &GetDescription() const{return pDescription;}
	
	/** Sender matches label. */
	bool SenderMatchesLabel(FXObject *sender) const;
	
	/** Sender matches edit widget. */
	bool SenderMatchesEdit(FXObject *sender) const;
	
	/**
	 * Process command message if sender is edit widget of this parameter.
	 * \returns true if processed or false if not destined for this parameter.
	 */
	bool ProcessSelCommand(FXObject *sender);
	
	/**
	 * Process changed message if sender is edit widget of this parameter.
	 * \returns true if processed or false if not destined for this parameter.
	 */
	bool ProcessSelChanged(FXObject *sender);
	
	/** Set parameter value. */
	void SetParameterValue(const char *value);
	
	/** Set parameter value. */
	void SetParameterValue(FXdouble value);
	
	/** Update edit widget value from parameter value. */
	void Update();
	
	/** Reset value to default. */
	void Reset();
	
	/** Update visibility. */
	void UpdateVisibility(deModuleParameter::eCategory category);
	/*@}*/
};

#endif
