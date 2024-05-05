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

#ifndef _DEGLDIALOGENGINEPROPS_H_
#define _DEGLDIALOGENGINEPROPS_H_

#include "../foxtoolkit.h"

class deglWindowMain;



/**
 * Engine Properties Panel.
 */
class deglDialogEngineProps : public FXDialogBox{
	FXDECLARE( deglDialogEngineProps )
protected:
	deglDialogEngineProps();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		ID_CB_ACTIVE_PROFILE,
		ID_BTN_EDIT_PROFILES,
		ID_LAST
	};
	
private:
	deglWindowMain *pWindowMain;
	
	FXTabBook *pTabPanels;
	
	FXTextField *pEditPathConfig;
	FXTextField *pEditPathShare;
	FXTextField *pEditPathLib;
	
	FXComboBox *pCBActiveProfile;
	FXButton *pBtnEditProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDialogEngineProps( deglWindowMain *windowMain, FXWindow *owner );
	
	/** Clean up dialog. */
	virtual ~deglDialogEngineProps();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Set values from engine. */
	void SetFromEngine();
	
	/** Update profile list. */
	void UpdateProfileList();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCBProfilesChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnEditProfiles( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
