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

#ifndef _IGDEDEMODULESTATUS_H_
#define _IGDEDEMODULESTATUS_H_

#include "../igdeComboBox.h"
#include "../igdeCheckBox.h"
#include "../igdeTextArea.h"
#include "../igdeTextField.h"
#include "../layout/igdeContainerFlow.h"


class igdeDialogEngine;


/**
 * \brief Panel showing the status of a module in the game engine.
 */
class DE_DLL_EXPORT igdeDEModuleStatus : public igdeContainerFlow{
public:
	typedef deTObjectReference<igdeDEModuleStatus> Ref;
	
	
private:
	igdeDialogEngine &pDialogEngine;
	
	igdeComboBox::Ref pCBModule;
	igdeTextField::Ref pEditType;
	igdeTextArea::Ref pEditDescription;
	igdeTextField::Ref pEditAuthor;
	igdeTextField::Ref pEditVersion;
	igdeTextField::Ref pEditPattern;
	igdeTextField::Ref pEditDefaultExtension;
	igdeTextField::Ref pEditDirName;
	igdeCheckBox::Ref pChkFallback;
	igdeTextArea::Ref pEditStatus;
	
	igdeTextField::Ref pEditLibName;
	igdeTextField::Ref pEditLibSize;
	igdeTextField::Ref pEditLibHash;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEModuleStatus(igdeDialogEngine &dialogEngine);
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEModuleStatus();
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateModuleStatus();
	void UpdateModulesList();
};

#endif
