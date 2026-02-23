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

#ifndef _DEGLB_DIALOGRUNWITH_H_
#define _DEGLB_DIALOGRUNWITH_H_

#include <Window.h>
#include <Button.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <StringView.h>
#include <OS.h>

#include <delauncher/game/delGame.h>
#include <delauncher/game/profile/delGameProfile.h>

class deglbWindowMain;


/**
 * \brief Run Game With Dialog.
 */
class deglbDialogRunGameWith : public BWindow{
public:
	enum eMessages{
		MSG_RUN = 'rgwr',
		MSG_CANCEL = 'rgwc',
		MSG_PROFILE_CHANGED = 'rgwp',
		MSG_EDIT_PROFILES = 'rgwe'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	const delGame::Ref pGame;
	delGameProfile::Ref pProfile;
	
	BMenuField *pMenuProfile;
	BPopUpMenu *pPopupProfile;
	BStringView *pLabProblem;
	
	sem_id pSem;
	bool pResult;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglbDialogRunGameWith(deglbWindowMain *windowMain, delGame *game);
	
	/** \brief Clean up dialog. */
	~deglbDialogRunGameWith() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Game. */
	inline const delGame::Ref &GetGame() const{ return pGame; }
	
	/** \brief Profile or nullptr for default. */
	inline const delGameProfile::Ref &GetProfile() const{ return pProfile; }
	
	/** \brief Set profile. */
	void SetProfile(delGameProfile *profile);
	
	/** \brief Run dialog, returns true if user confirmed, false if cancelled. */
	bool Go();
	
	/** \brief Update profile list. */
	void UpdateGame();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	bool QuitRequested() override;
	/*@}*/
};

#endif
