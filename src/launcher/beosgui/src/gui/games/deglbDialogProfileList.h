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

#ifndef _DEGLB_DIALOGPROFILELIST_H_
#define _DEGLB_DIALOGPROFILELIST_H_

#include <Window.h>
#include <Button.h>
#include <ListView.h>
#include <ScrollView.h>
#include <MenuField.h>
#include <PopUpMenu.h>
#include <TextControl.h>
#include <CheckBox.h>
#include <OS.h>

#include <delauncher/game/delGame.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/collection/decTObjectOrderedSet.h>
#include <dragengine/common/utils/decUuid.h>
#include <dragengine/deObject.h>
#include <dragengine/deObjectReference.h>

class deglbWindowMain;


/**
 * \brief Profile List Dialog.
 */
class deglbDialogProfileList : public BWindow{
public:
	enum eMessages{
		MSG_OK = 'plok',
		MSG_CANCEL = 'plca',
		MSG_PROFILE_SEL = 'plps',
		MSG_PROF_ADD = 'plpa',
		MSG_PROF_DUP = 'plpd',
		MSG_PROF_DEL = 'plpx',
		MSG_PROF_RENAME = 'plpr',
		MSG_MOD_GRA_CHANGED = 'plmg',
		MSG_MOD_INP_CHANGED = 'plmi',
		MSG_MOD_PHY_CHANGED = 'plmp',
		MSG_MOD_AMR_CHANGED = 'plma',
		MSG_MOD_AI_CHANGED = 'plmb',
		MSG_MOD_CR_CHANGED = 'plmc',
		MSG_MOD_AUD_CHANGED = 'plmd',
		MSG_MOD_NET_CHANGED = 'plme',
		MSG_MOD_SYN_CHANGED = 'plmf',
		MSG_MOD_VR_CHANGED = 'plmh',
		MSG_RUNARGS_CHANGED = 'plra',
		MSG_WIDTH_CHANGED = 'plwc',
		MSG_HEIGHT_CHANGED = 'plhc',
		MSG_FULLSCREEN_CHANGED = 'plfc'
	};
	
private:
	/** \brief Profile edit wrapper. */
	class cEditProfile : public deObject{
	public:
		using Ref = deTObjectReference<cEditProfile>;
		
		delGameProfile::Ref original, edit;
		
		explicit cEditProfile(const char *name);
		explicit cEditProfile(delGameProfile *profile);
		
	protected:
		~cEditProfile() override;
	};
	
	struct sSystem{
		BMenuField *menuField;
		BPopUpMenu *popup;
		int type;
		uint32 msgWhat;
	};
	
	deglbWindowMain *pWindowMain;
	
	decTObjectOrderedSet<cEditProfile> pProfiles;
	
	BListView *pListProfiles;
	
	sSystem pSysGraphic;
	sSystem pSysInput;
	sSystem pSysPhysics;
	sSystem pSysAnimator;
	sSystem pSysAI;
	sSystem pSysCrashRecovery;
	sSystem pSysAudio;
	sSystem pSysSynthesizer;
	sSystem pSysNetwork;
	sSystem pSysVR;
	
	BTextControl *pEditRunArgs;
	BCheckBox *pChkReplaceRunArgs;
	BTextControl *pEditWidth;
	BTextControl *pEditHeight;
	
	sem_id pSem;
	bool pResult;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	deglbDialogProfileList(deglbWindowMain *windowMain, delGameProfile *selectProfile);
	
	/** \brief Clean up dialog. */
	~deglbDialogProfileList() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Run dialog, returns true if changes were made. */
	bool Go();
	
	/** \brief Update profile list. */
	void UpdateProfileList();
	
	/** \brief Update selected profile. */
	void UpdateProfile();
	
	/** \brief Update system module list. */
	void UpdateSystemModuleList(sSystem &system, const char *moduleName,
		const char *moduleVersion);
	
	/** \brief Apply changes to profile. */
	void ApplyChanges();
	
	/** \brief Save profiles back to manager. */
	void SaveProfiles();
	/*@}*/
	
	
	
	/** \name BWindow */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	bool QuitRequested() override;
	/*@}*/
	
	
	
private:
	cEditProfile *pGetSelectedProfile() const;
	void pSetSelectedProfile(cEditProfile *profile);
	void pCreateSystem(sSystem &system, const char *label, int type, uint32 msgWhat, BView *container);
	void pLoadProfiles(delGameProfile *selectProfile);
};

#endif
