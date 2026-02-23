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

#ifndef _DEGLB_DIALOGMODULEPROPS_H_
#define _DEGLB_DIALOGMODULEPROPS_H_

#include <Window.h>
#include <Button.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <TextView.h>
#include <ScrollView.h>
#include <OS.h>

#include <delauncher/engine/modules/delEngineModule.h>

class deglbWindowMain;


/**
 * \brief Module Properties Dialog.
 */
class deglbDialogModuleProps : public BWindow{
public:
	enum eMessages{
		MSG_OK = 'mpok',
		MSG_SHOW_INFO = 'mpsi'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	const delEngineModule::Ref pModule;
	
	BTextControl *pEditName;
	BTextControl *pEditType;
	BTextControl *pEditStatus;
	BTextControl *pEditAuthor;
	BTextControl *pEditVersion;
	BTextControl *pEditPattern;
	BCheckBox *pChkIsFallback;
	BTextView *pTextDescription;
	
	BTextControl *pEditErrorCode;
	BTextControl *pEditLibFileName;
	BTextControl *pEditLibFileSizeShould;
	BTextControl *pEditLibFileSizeIs;
	BTextControl *pEditLibFileHashShould;
	BTextControl *pEditLibFileHashIs;
	BTextControl *pEditLibFileEntryPoint;
	
	sem_id pSem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglbDialogModuleProps(deglbWindowMain *windowMain, delEngineModule *module);
	
	/** \brief Clean up dialog. */
	~deglbDialogModuleProps() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Module. */
	inline const delEngineModule::Ref &GetModule() const{ return pModule; }
	
	/** \brief Run modal dialog. */
	void Go();
	
	/** \brief Set values from module. */
	void SetFromModule();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	bool QuitRequested() override;
	/*@}*/
};

#endif
