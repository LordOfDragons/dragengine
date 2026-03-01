/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_DIALOGPROFILELISTPARAMETER_H_
#define _DEGLB_DIALOGPROFILELISTPARAMETER_H_

#include <CheckBox.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <Slider.h>
#include <StringView.h>
#include <TextControl.h>

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/deModuleParameter.h>

class delEMParameter;
class delGameProfile;


/**
 * \brief Profile list module parameter widget.
 */
class deglbDialogProfileListParameter : public deObject{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<deglbDialogProfileListParameter>;
	
	
private:
	delEMParameter &pParameter;
	delGameProfile &pProfile;
	const decString pModuleName;
	decString pDescription;
	
	BStringView *pLabel;
	BMenuField *pMenuField;
	BPopUpMenu *pPopUpMenu;
	BCheckBox *pCheckBox;
	BSlider *pSlider;
	BTextControl *pTextControl;
	
	bool pCustomized;
	uint32 pMsgWhat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create parameter. */
	deglbDialogProfileListParameter(delEMParameter &parameter, delGameProfile &profile,
		const char *moduleName, uint32 msgWhat);
	
protected:
	/** \brief Clean up parameter. */
	~deglbDialogProfileListParameter() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parameter. */
	inline delEMParameter &GetParameter() const{ return pParameter; }
	
	/** \brief Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Label widget. */
	inline BStringView *GetLabel() const{ return pLabel; }
	
	/** \brief Edit widget (menu field, check box, slider, or text control). */
	BView *GetEditWidget() const;
	
	/** \brief Customized (differs from default). */
	inline bool IsCustomized() const{ return pCustomized; }
	
	/** \brief Update edit widget from current parameter value. */
	void Update();
	
	/** \brief Apply value from menu field selection. */
	void ApplyMenuSelection();
	
	/** \brief Apply value from check box. */
	void ApplyCheckBox();
	
	/** \brief Apply value from slider. */
	void ApplySlider();
	
	/** \brief Apply value from text control. */
	void ApplyTextControl();
	
	/** \brief Reset value to default. */
	void Reset();
	
	/** \brief Update visibility. */
	void UpdateVisibility(deModuleParameter::eCategory category);
	/*@}*/
	
	
	
private:
	void pSetParameterValue(const char *value);
	void pSetParameterValue(float value);
};

#endif
