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

#ifndef _DEGLDIALOGMODULEPROPS_H_
#define _DEGLDIALOGMODULEPROPS_H_

#include "../foxtoolkit.h"

#include <delauncher/engine/modules/delEngineModule.h>

class deglWindowMain;



/**
 * Module Properties Panel.
 */
class deglDialogModuleProps : public FXDialogBox{
	FXDECLARE(deglDialogModuleProps)
protected:
	deglDialogModuleProps();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		ID_BTN_INFO,
		ID_LAST
	};
	
private:
	deglWindowMain *pWindowMain;
	const delEngineModule::Ref pModule;
	
	FXTabBook *pTabPanels;
	
	FXTextField *pEditName;
	FXTextField *pEditType;
	FXTextField *pEditStatus;
	FXText *pTextDescription;
	FXTextField *pEditAuthor;
	FXTextField *pEditVersion;
	FXTextField *pEditPattern;
	FXCheckButton *pChkIsFallback;
	
	FXTextField *pEditErrorCode;
	FXButton *pBtnInfoErrorCode;
	FXTextField *pEditLibFileName;
	FXTextField *pEditLibFileSizeShould;
	FXTextField *pEditLibFileSizeIs;
	FXTextField *pEditLibFileHashShould;
	FXTextField *pEditLibFileHashIs;
	FXTextField *pEditLibFileEntryPoint;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	deglDialogModuleProps(deglWindowMain *windowMain, delEngineModule *module, FXWindow *owner);
	/** Cleans up the dialog. */
	virtual ~deglDialogModuleProps();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	/** Retrieves the module. */
	inline const delEngineModule::Ref &GetModule() const{ return pModule; }
	
	/** Set values from moduel. */
	void SetFromModule();
	/*@}*/
	
	/** \name Events */
	/*@{*/
	long onBtnInfo(FXObject *sender, FXSelector selector, void *data);
	/*@}*/
};

// end of include only once
#endif
