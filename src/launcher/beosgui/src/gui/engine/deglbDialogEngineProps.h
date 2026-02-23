/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEGLB_DIALOGENGINEPROPS_H_
#define _DEGLB_DIALOGENGINEPROPS_H_

#include <Window.h>
#include <Button.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <TextControl.h>

#include <OS.h>

class deglbWindowMain;


/**
 * \brief Engine Properties Dialog.
 */
class deglbDialogEngineProps : public BWindow{
public:
	enum eMessages{
		MSG_OK = 'epok',
		MSG_PROFILE_CHANGED = 'eppc',
		MSG_EDIT_PROFILES = 'epep'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	
	BTextControl *pEditPathConfig;
	BTextControl *pEditPathShare;
	BTextControl *pEditPathLib;
	
	BMenuField *pMenuProfile;
	BPopUpMenu *pPopupProfile;
	
	sem_id pSem;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglbDialogEngineProps(deglbWindowMain *windowMain);
	
	/** \brief Clean up dialog. */
	~deglbDialogEngineProps() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Run modal dialog. */
	void Go();
	
	/** \brief Set values from engine. */
	void SetFromEngine();
	
	/** \brief Update profile list. */
	void UpdateProfileList();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	bool QuitRequested() override;
	/*@}*/
};

#endif
