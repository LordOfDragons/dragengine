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

#ifndef _IGDEDEPARAMETERS_H_
#define _IGDEDEPARAMETERS_H_

#include "../igdeButton.h"
#include "../igdeComboBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/systems/modules/deModuleParameter.h>


class igdeDialogEngine;
class deModuleParameter;
class deLoadableModule;


/**
 * \brief Panel showing the parameters of modules allowing the user to also change them.
 */
class DE_DLL_EXPORT igdeDEParameters : public igdeContainerFlow{
private:
	igdeDialogEngine &pDialogEngine;
	deModuleParameter pParameterInfo;
	
	igdeComboBox::Ref pCBModule;
	igdeComboBox::Ref pCBParameter;
	igdeTextArea::Ref pEditDescription;
	igdeTextField::Ref pEditType;
	igdeTextArea::Ref pEditAllowedValues;
	igdeTextField::Ref pEditValue;
	
	igdeButton::Ref pBtnSet;
	igdeButton::Ref pBtnReset;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEParameters(igdeDialogEngine &dialogEngine);
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEParameters();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateParameter();
	void UpdateParametersList();
	void UpdateModulesList();
	
	/** \brief Selected module. */
	deLoadableModule *GetSelectedModule() const;
	
	/** \brief Selected parameter. */
	const decString &GetSelectedParameter() const;
	
	/** \brief Set parameter value. */
	void ParameterSetValue();
	
	/** \brief Reset parameter value. */
	void ParameterResetValue();
	/*@}*/
};

#endif
