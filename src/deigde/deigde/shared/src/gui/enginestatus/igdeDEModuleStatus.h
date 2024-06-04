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

#include "../igdeComboBoxReference.h"
#include "../igdeCheckBoxReference.h"
#include "../igdeTextAreaReference.h"
#include "../igdeTextFieldReference.h"
#include "../layout/igdeContainerFlow.h"


class igdeDialogEngine;


/**
 * \brief Panel showing the status of a module in the game engine.
 */
class DE_DLL_EXPORT igdeDEModuleStatus : public igdeContainerFlow{
	igdeDialogEngine &pDialogEngine;
	
	igdeComboBoxReference pCBModule;
	igdeTextFieldReference pEditType;
	igdeTextAreaReference pEditDescription;
	igdeTextFieldReference pEditAuthor;
	igdeTextFieldReference pEditVersion;
	igdeTextFieldReference pEditPattern;
	igdeTextFieldReference pEditDefaultExtension;
	igdeTextFieldReference pEditDirName;
	igdeCheckBoxReference pChkFallback;
	igdeTextAreaReference pEditStatus;
	
	igdeTextFieldReference pEditLibName;
	igdeTextFieldReference pEditLibSize;
	igdeTextFieldReference pEditLibHash;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEModuleStatus( igdeDialogEngine &dialogEngine );
	
	
	
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
