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

#ifndef _DEGLDIALOGPROFILELIST_H_
#define _DEGLDIALOGPROFILELIST_H_

#include "../foxtoolkit.h"

#include <delauncher/game/delGame.h>
#include <delauncher/game/delGameList.h>
#include <delauncher/game/profile/delGameProfile.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/systems/modules/deModuleParameter.h>

class deglDialogProfileListParameter;
class delEngineModule;
class delEMParameter;
class deglWindowMain;


/**
 * Profiles Panel.
 */
class deglDialogProfileList : public FXDialogBox{
	FXDECLARE( deglDialogProfileList )
protected:
	deglDialogProfileList();
	
public:
	enum eFoxIDs{
		ID_SELF = FXDialogBox::ID_LAST,
		ID_LIST_PROFILES,
		ID_BTN_PROF_ADD,
		ID_BTN_PROF_DUP,
		ID_BTN_PROF_DEL,
		ID_BTN_PROF_RENAME,
		
		ID_CB_MOD_GRA,
		ID_CB_MOD_GRA_VERSION,
		ID_BTN_GRAMODINFO,
		ID_CB_MOD_INP,
		ID_CB_MOD_INP_VERSION,
		ID_BTN_INPMODINFO,
		ID_CB_MOD_PHY,
		ID_CB_MOD_PHY_VERSION,
		ID_BTN_PHYMODINFO,
		ID_CB_MOD_AMR,
		ID_CB_MOD_AMR_VERSION,
		ID_BTN_AMRMODINFO,
		ID_CB_MOD_AI,
		ID_CB_MOD_AI_VERSION,
		ID_BTN_AIMODINFO,
		ID_CB_MOD_CR,
		ID_CB_MOD_CR_VERSION,
		ID_BTN_CRMODINFO,
		ID_CB_MOD_AUD,
		ID_CB_MOD_AUD_VERSION,
		ID_BTN_AUDMODINFO,
		ID_CB_MOD_NET,
		ID_CB_MOD_NET_VERSION,
		ID_BTN_NETMODINFO,
		ID_CB_MOD_SYN,
		ID_CB_MOD_SYN_VERSION,
		ID_BTN_SYNMODINFO,
		ID_CB_MOD_VR,
		ID_CB_MOD_VR_VERSION,
		ID_BTN_VRMODINFO,
		
		ID_LIST_MP_MODULES,
		ID_MPPARAM_LABEL,
		ID_MPPARAM_VALUE,
		ID_MPPARAM_OPT_BASIC,
		ID_MPPARAM_OPT_ADVANCED,
		ID_MPPARAM_OPT_EXPERT,
		
		ID_EDIT_RUNARGS,
		ID_CHK_REPLACE_RUNARGS,
		ID_EDIT_WIDTH,
		ID_EDIT_HEIGHT,
		ID_CB_FULLSCREEN_RESOLUTIONS,
		ID_CB_DISABLE_MODULE_VERSION_MODULE,
		ID_CB_DISABLE_MODULE_VERSION_VERSION,
		ID_BTN_DISABLE_MODULE_VERSION_ADD,
		ID_BTN_DISABLE_MODULE_VERSION_REMOVE,
		ID_LIST_DISABLE_MODULE_VERSIONS,
		
		ID_PU_PARAM_RESET,
		ID_PU_PARAM_SETVALUE,
		
		ID_LAST
	};
	
private:
	class cEditProfile : public deObject{
	private:
		deglDialogProfileList &pDialog;
		const delGameProfile::Ref pOriginal;
		const delGameProfile::Ref pEdit;
		const delGame::Ref pGameCustom;
		
	protected:
		virtual ~cEditProfile();
		
	public:
		typedef deTObjectReference<cEditProfile> Ref;
		cEditProfile( deglDialogProfileList &dialog, const char *name );
		cEditProfile( deglDialogProfileList &dialog, delGameProfile *profile, delGame *gameCustom );
		inline delGameProfile *GetOriginal() const{ return pOriginal; }
		inline delGameProfile *GetEdit() const{ return pEdit; }
		inline delGame *GetGameCustom() const{ return pGameCustom; }
		FXString GetText() const;
		FXIcon *GetIcon() const;
	};
	
	struct sSystem{
		FXLabel *label;
		FXLabel *icon;
		FXComboBox *combobox;
		FXComboBox *comboboxVersion;
		FXButton *modinfo;
		FXLabel *problem;
		int type;
	};
	
	class Parameter : public deObject{
	public:
		typedef deTObjectReference<Parameter> Ref;
		delEMParameter *parameter;
		FXLabel *label;
		FXComboBox *combobox;
		FXRealSlider *slider;
		FXCheckButton *checkbox;
		FXTextField *textfield;
	};
	
	deglWindowMain *pWindowMain;
	bool pSystemsValid;
	
	decObjectOrderedSet pProfiles;
	
	FXList *pListProfiles;
	FXButton *pBtnProfAdd;
	FXButton *pBtnProfDup;
	FXButton *pBtnProfDel;
	FXButton *pBtnProfRename;
	
	FXTabBook *pTabPanels;
	FXTabItem *pTabSystems;
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
	
	FXIconList *pListMPModules;
	FXMatrix *pFrameMPParameters;
	decObjectOrderedSet pMPParameters;
	FXText *pTextMPParameterInfo;
	deModuleParameter::eCategory pMPParameterCategory;
	static bool pAllowExpertMode;
	
	FXTextField *pEditRunArgs;
	FXCheckButton *pChkReplaceRunArgs;
	FXTextField *pEditWidth;
	FXTextField *pEditHeight;
	FXComboBox *pCBFullScreenResolutions;
	FXComboBox *pCBDisableModuleVersionModule;
	FXComboBox *pCBDisableModuleVersionVersion;
	FXButton *pBtnDisableModuleVersionAdd;
	FXButton *pBtnDisableModuleVersionRemove;
	FXList *pListDisableModuleVersions;
	
	deglDialogProfileListParameter *pPUMPParameter;
	delGameList pDropCustomGameProfiles;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	deglDialogProfileList( deglWindowMain *windowMain, FXWindow *owner, delGameProfile *selectProfile );
	/** Cleans up the dialog. */
	virtual ~deglDialogProfileList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the main window. */
	inline deglWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** Update full screen resolution list. */
	void UpdateFullScreenResolutions();
	
	/** Update system module lists. */
	void UpdateSystemModuleLists();
	
	/** Updates the list of profiles. */
	void UpdateProfileList();
	/** Update the selected profile if any. */
	void UpdateProfile();
	/** Update a system. */
	void UpdateSystem( sSystem &system, const char *moduleName, const char *moduleVersion );
	/** Disable a system. */
	void DisableSystem( sSystem &system );
	
	/** Update module versions. */
	void UpdateModuleVersions( sSystem &system, const char *moduleName, const char *moduleVersion );
	
	/** Get selected module name. */
	bool GetSelectedMPModuleName( FXString &name );
	/** Update modules. */
	void UpdateMPModuleList();
	/** Update parameters list. */
	void UpdateMPParameterList();
	
	/** Update disabled module versions list. */
	void UpdateListDisabledModuleVersions();
	/** Update disabled module versions module combo box. */
	void UpdateCBDisabledModuleVersionsModule();
	/** Update disabled module versions version combo box. */
	void UpdateCBDisabledModuleVersionsVersion();
	
	/** Run modal invocation of the dialog. */
	virtual FXuint execute( FXuint placement = PLACEMENT_OWNER );
	/*@}*/
	
	/** \name Events */
	/*@{*/
	long onListProfilesChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnProfAdd( FXObject *sender, FXSelector selector, void *data );
	long onBtnProfDup( FXObject *sender, FXSelector selector, void *data );
	long onBtnProfDel( FXObject *sender, FXSelector selector, void *data );
	long updateBtnProfDel( FXObject *sender, FXSelector selector, void *data );
	long onBtnProfRename( FXObject *sender, FXSelector selector, void *data );
	long updateBtnProfRename( FXObject *sender, FXSelector selector, void *data );
	
	long onCBModGraChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModGraVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnGraModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModInpChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModInpVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnInpModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModPhyChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModPhyVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnPhyModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModAmrChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModAmrVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnAmrModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModAIChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModAIVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnAIModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModCRChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModCRVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnCRModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModAudChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModAudVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnAudModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModNetChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModNetVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnNetModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModSynChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModSynVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnSynModInfo( FXObject *sender, FXSelector selector, void *data );
	long onCBModVRChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModVRVersionChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnVRModInfo( FXObject *sender, FXSelector selector, void *data );
	
	long onListMPModulesChanged( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterValueCommand( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterValueChanged( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterValueFocusIn( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterLabelLMPress( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterLabelLMRelease( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterLabelRMPress( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterLabelRMRelease( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterOptBasic( FXObject *sender, FXSelector selector, void *data );
	long updateMPParameterOptBasic( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterOptAdvanced( FXObject *sender, FXSelector selector, void *data );
	long updateMPParameterOptAdvanced( FXObject *sender, FXSelector selector, void *data );
	long onMPParameterOptExpert( FXObject *sender, FXSelector selector, void *data );
	long updateMPParameterOptExpert( FXObject *sender, FXSelector selector, void *data );
	
	long onEditRunArgsChanged( FXObject *sender, FXSelector selector, void *data );
	long onChkReplaceRunArgsChanged( FXObject *sender, FXSelector selector, void *data );
	long onEditWidthChanged( FXObject *sender, FXSelector selector, void *data );
	long onEditHeightChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBFullScreenResolutionsChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBDisableModuleVersionModuleChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnDisableModuleVersionAdd( FXObject *sender, FXSelector selector, void *data );
	long onBtnDisableModuleVersionRemove( FXObject *sender, FXSelector selector, void *data );
	
	long onPUParamReset( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
private:
	void pCreateSystem( sSystem &system, const char *textLabel, const char *toolText,
		int comboBoxSelector, int comboBoxVersionSelector, int buttonSelector,
		FXComposite *container );
	void pUpdateMPParamVisiblity();
	cEditProfile *pGetSelectedProfile() const;
	void pSetSelectedProfile( delGameProfile *profile );
	void pSetSelectedProfile( cEditProfile *profile );
	cEditProfile *pGetProfileNamed( const char *name ) const;
};

// end of include only once
#endif
