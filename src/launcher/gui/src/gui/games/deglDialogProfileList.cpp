/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "deglDialogProfileList.h"
#include "deglDialogProfileListParameter.h"
#include "../deglWindowMain.h"
#include "../deglGuiBuilder.h"
#include "../engine/deglDialogModuleProps.h"
#include "../../deglLauncher.h"
#include "../../config/deglConfiguration.h"
#include "../../engine/deglEngine.h"
#include "../../engine/modules/deglEngineModule.h"
#include "../../engine/modules/deglEngineModuleList.h"
#include "../../engine/modules/parameter/deglEMParameter.h"
#include "../../game/deglGame.h"
#include "../../game/deglGameManager.h"
#include "../../game/fileformat/deglFileFormat.h"
#include "../../game/profile/deglGameProfile.h"
#include "../../game/profile/deglGPModule.h"
#include "../../game/profile/deglGPMParameter.h"
#include "../../game/profile/deglGPDisableModuleVersion.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/deModuleSystem.h>



// Events
///////////

FXDEFMAP( deglDialogProfileList ) deglDialogProfileListMap[]={
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_LIST_PROFILES, deglDialogProfileList::onListProfilesChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_LIST_PROFILES, deglDialogProfileList::onListProfilesChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_PROF_ADD, deglDialogProfileList::onBtnProfAdd ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_PROF_DUP, deglDialogProfileList::onBtnProfDup ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_PROF_DEL, deglDialogProfileList::onBtnProfDel ),
	FXMAPFUNC( SEL_UPDATE, deglDialogProfileList::ID_BTN_PROF_DEL, deglDialogProfileList::updateBtnProfDel ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_PROF_RENAME, deglDialogProfileList::onBtnProfRename ),
	FXMAPFUNC( SEL_UPDATE, deglDialogProfileList::ID_BTN_PROF_RENAME, deglDialogProfileList::updateBtnProfRename ),
	
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_GRA, deglDialogProfileList::onCBModGraChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_GRA, deglDialogProfileList::onCBModGraChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_GRAMODINFO, deglDialogProfileList::onBtnGraModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_INP, deglDialogProfileList::onCBModInpChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_INP, deglDialogProfileList::onCBModInpChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_INPMODINFO, deglDialogProfileList::onBtnInpModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_PHY, deglDialogProfileList::onCBModPhyChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_PHY, deglDialogProfileList::onCBModPhyChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_PHYMODINFO, deglDialogProfileList::onBtnPhyModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_AMR, deglDialogProfileList::onCBModAmrChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_AMR, deglDialogProfileList::onCBModAmrChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_AMRMODINFO, deglDialogProfileList::onBtnAmrModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_AI, deglDialogProfileList::onCBModAIChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_AI, deglDialogProfileList::onCBModAIChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_AIMODINFO, deglDialogProfileList::onBtnAIModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_CR, deglDialogProfileList::onCBModCRChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_CR, deglDialogProfileList::onCBModCRChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_CRMODINFO, deglDialogProfileList::onBtnCRModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_AUD, deglDialogProfileList::onCBModAudChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_AUD, deglDialogProfileList::onCBModAudChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_AUDMODINFO, deglDialogProfileList::onBtnAudModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_NET, deglDialogProfileList::onCBModNetChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_NET, deglDialogProfileList::onCBModNetChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_NETMODINFO, deglDialogProfileList::onBtnNetModInfo ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_MOD_SYN, deglDialogProfileList::onCBModSynChanged ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_CB_MOD_SYN, deglDialogProfileList::onCBModSynChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_SYNMODINFO, deglDialogProfileList::onBtnSynModInfo ),
	
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_LIST_MP_MODULES, deglDialogProfileList::onListMPModulesChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_MPPARAM_VALUE, deglDialogProfileList::onMPParameterValueCommand ),
	FXMAPFUNC( SEL_CHANGED, deglDialogProfileList::ID_MPPARAM_VALUE, deglDialogProfileList::onMPParameterValueChanged ),
	FXMAPFUNC( SEL_FOCUSIN, deglDialogProfileList::ID_MPPARAM_VALUE, deglDialogProfileList::onMPParameterValueFocusIn ),
	FXMAPFUNC( SEL_LEFTBUTTONPRESS, deglDialogProfileList::ID_MPPARAM_LABEL, deglDialogProfileList::onMPParameterLabelLMPress ),
	FXMAPFUNC( SEL_LEFTBUTTONRELEASE, deglDialogProfileList::ID_MPPARAM_LABEL, deglDialogProfileList::onMPParameterLabelLMRelease ),
	FXMAPFUNC( SEL_RIGHTBUTTONPRESS, deglDialogProfileList::ID_MPPARAM_LABEL, deglDialogProfileList::onMPParameterLabelRMPress ),
	FXMAPFUNC( SEL_RIGHTBUTTONRELEASE, deglDialogProfileList::ID_MPPARAM_LABEL, deglDialogProfileList::onMPParameterLabelRMRelease ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_MPPARAM_OPT_BASIC, deglDialogProfileList::onMPParameterOptBasic ),
	FXMAPFUNC( SEL_UPDATE, deglDialogProfileList::ID_MPPARAM_OPT_BASIC, deglDialogProfileList::updateMPParameterOptBasic ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_MPPARAM_OPT_ADVANCED, deglDialogProfileList::onMPParameterOptAdvanced ),
	FXMAPFUNC( SEL_UPDATE, deglDialogProfileList::ID_MPPARAM_OPT_ADVANCED, deglDialogProfileList::updateMPParameterOptAdvanced ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_MPPARAM_OPT_EXPERT, deglDialogProfileList::onMPParameterOptExpert ),
	FXMAPFUNC( SEL_UPDATE, deglDialogProfileList::ID_MPPARAM_OPT_EXPERT, deglDialogProfileList::updateMPParameterOptExpert ),
	
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_EDIT_RUNARGS, deglDialogProfileList::onEditRunArgsChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CHK_REPLACE_RUNARGS, deglDialogProfileList::onChkReplaceRunArgsChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_EDIT_WIDTH, deglDialogProfileList::onEditWidthChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_EDIT_HEIGHT, deglDialogProfileList::onEditHeightChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_FULLSCREEN_RESOLUTIONS, deglDialogProfileList::onCBFullScreenResolutionsChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_CB_DISABLE_MODULE_VERSION_MODULE, deglDialogProfileList::onCBDisableModuleVersionModuleChanged ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_DISABLE_MODULE_VERSION_ADD, deglDialogProfileList::onBtnDisableModuleVersionAdd ),
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_BTN_DISABLE_MODULE_VERSION_REMOVE, deglDialogProfileList::onBtnDisableModuleVersionRemove ),
	
	FXMAPFUNC( SEL_COMMAND, deglDialogProfileList::ID_PU_PARAM_RESET, deglDialogProfileList::onPUParamReset ),
};



// Class deglDialogProfileList::cEditProfile
//////////////////////////////////////////////

deglDialogProfileList::cEditProfile::cEditProfile( deglDialogProfileList &dialog, const char *name ) :
pDialog( dialog ),
pEdit( new deglGameProfile( name ) ){
}

deglDialogProfileList::cEditProfile::cEditProfile( deglDialogProfileList &dialog,
	deglGameProfile *profile, deglGame *gameCustom ) :
pDialog( dialog ),
pOriginal( profile ),
pEdit( new deglGameProfile( *profile ) ),
pGameCustom( gameCustom ){
}

deglDialogProfileList::cEditProfile::~cEditProfile(){ }

FXString deglDialogProfileList::cEditProfile::GetText() const{
	if( pGameCustom ){
		FXString text;
		text.format( "<Custom: %s>", GetGameCustom()->GetTitle().ToUTF8().GetString() );
		return text;
		
	}else{
		return FXString( GetEdit()->GetName().GetString() );
	}
}

FX::FXIcon *deglDialogProfileList::cEditProfile::GetIcon() const{
	return GetEdit()->GetValid() ? pDialog.GetWindowMain()->GetIconValidSmall()
		: pDialog.GetWindowMain()->GetIconInvalidSmall();
}



// Class deglDialogProfileList
////////////////////////////////

FXIMPLEMENT( deglDialogProfileList, FXDialogBox, deglDialogProfileListMap, ARRAYNUMBER( deglDialogProfileListMap ) )

bool deglDialogProfileList::pAllowExpertMode = false;


// Constructor, destructor
////////////////////////////

deglDialogProfileList::deglDialogProfileList(){ }

deglDialogProfileList::deglDialogProfileList( deglWindowMain *windowMain, FXWindow *owner,
	deglGameProfile *selectProfile ) :
FXDialogBox( owner, "Profiles", DECOR_TITLE | DECOR_BORDER | DECOR_RESIZE | DECOR_CLOSE,
0, 0, 1100, 600, 10, 10, 10, 5 ){
	const deglGuiBuilder &guiBuilder = *windowMain->GetGuiBuilder();
	FXHorizontalFrame *frameSuper, *frameTabHorz, *frameLine, *frameLine2;
	FXVerticalFrame *content, *scrollContent, *frameTab, *frameGroup, *groupBox, *frameVertical;
	FXPacker *frameDeco;
	FXMatrix *block;
	FXScrollWindow *scrollWindow;
	FXSplitter *splitter;
	const char *toolTip;
	
	pWindowMain = windowMain;
	pPUMPParameter = NULL;
	pMPParameterCategory = deModuleParameter::ecBasic;
	
	pSysAI.type = deModuleSystem::emtAI;
	pSysAnimator.type = deModuleSystem::emtAnimator;
	pSysAudio.type = deModuleSystem::emtAudio;
	pSysCrashRecovery.type = deModuleSystem::emtCrashRecovery;
	pSysGraphic.type = deModuleSystem::emtGraphic;
	pSysInput.type = deModuleSystem::emtInput;
	pSysNetwork.type = deModuleSystem::emtNetwork;
	pSysPhysics.type = deModuleSystem::emtPhysics;
	pSysSynthesizer.type = deModuleSystem::emtSynthesizer;
	
	
	content =  new FXVerticalFrame( this, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 );
	frameSuper = new FXHorizontalFrame( content, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	
	// list of profiles
	frameTab = new FXVerticalFrame( frameSuper, /*LAYOUT_FIX_WIDTH | */LAYOUT_FILL_Y, 0, 0, 250, 0, 10, 10, 10, 10, 0, 3 );
	
	frameDeco = new FXPacker( frameTab, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 230, 0, 0, 0, 0, 0, 0, 0 );
	pListProfiles = new FXList( frameDeco, this, ID_LIST_PROFILES, LIST_BROWSESELECT | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pListProfiles->setSortFunc( deglGuiBuilder::SortListItemByName );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	pBtnProfAdd = guiBuilder.CreateButton( block, "Add", NULL, this, ID_BTN_PROF_ADD, "Add a profile" );
	pBtnProfAdd->setLayoutHints( pBtnProfAdd->getLayoutHints() | LAYOUT_FILL_COLUMN | LAYOUT_FILL_X );
	pBtnProfDel = guiBuilder.CreateButton( block, "Remove", NULL, this, ID_BTN_PROF_DEL, "Remove profile" );
	pBtnProfDel->setLayoutHints( pBtnProfDel->getLayoutHints() | LAYOUT_FILL_COLUMN | LAYOUT_FILL_X );
	pBtnProfRename = guiBuilder.CreateButton( block, "Rename", NULL, this, ID_BTN_PROF_RENAME, "Rename profile" );
	pBtnProfRename->setLayoutHints( pBtnProfRename->getLayoutHints() | LAYOUT_FILL_COLUMN | LAYOUT_FILL_X );
	pBtnProfDup = guiBuilder.CreateButton( block, "Duplicate", NULL, this, ID_BTN_PROF_DUP, "Duplicate a profile" );
	pBtnProfDup->setLayoutHints( pBtnProfDup->getLayoutHints() | LAYOUT_FILL_COLUMN | LAYOUT_FILL_X );
	
	
	// Systems
	pTabPanels = new FXTabBook( frameSuper, windowMain->GetIconValidSmall(), 0, TABBOOK_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pTabSystems = new FXTabItem( pTabPanels, "Systems", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	scrollWindow = new FXScrollWindow( frameTab, LAYOUT_FILL_X | LAYOUT_FILL_Y | SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK );
	scrollContent = new FXVerticalFrame( scrollWindow, LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( scrollContent, 2 );
	
	pCreateSystem( pSysGraphic, "Graphic Module:", "Select the Graphic Module to use", ID_CB_MOD_GRA, ID_BTN_GRAMODINFO, block );
	pCreateSystem( pSysInput, "Input Module:", "Select the Input Module to use", ID_CB_MOD_INP, ID_BTN_INPMODINFO, block );
	pCreateSystem( pSysPhysics, "Physics Module:", "Select the Physics Module to use", ID_CB_MOD_PHY, ID_BTN_PHYMODINFO, block );
	pCreateSystem( pSysAnimator, "Animator Module:", "Select the Animator Module to use", ID_CB_MOD_AMR, ID_BTN_AMRMODINFO, block );
	pCreateSystem( pSysAI, "AI Module:", "Select the AI Module to use", ID_CB_MOD_AI, ID_BTN_AIMODINFO, block );
	pCreateSystem( pSysCrashRecovery, "Crash Recovery Module:", "Select the Crash Recovery Module to use", ID_CB_MOD_CR, ID_BTN_CRMODINFO, block );
	pCreateSystem( pSysAudio, "Audio Module:", "Select the Audio Module to use", ID_CB_MOD_AUD, ID_BTN_AUDMODINFO, block );
	pCreateSystem( pSysNetwork, "Network Module:", "Select the Network Module to use", ID_CB_MOD_NET, ID_BTN_NETMODINFO, block );
	pCreateSystem( pSysSynthesizer, "Synthesizer Module:", "Select Synthesizer Module to use", ID_CB_MOD_SYN, ID_BTN_SYNMODINFO, block );
	
	
	// Module Parameters
	new FXTabItem( pTabPanels, "Module Parameters", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTabHorz = new FXHorizontalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 3, 0 );
	
	frameDeco = new FXPacker( frameTabHorz, LAYOUT_FIX_WIDTH | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 290, 0, 0, 0, 0, 0, 0, 0 );
	pListMPModules = new FXIconList( frameDeco, this, ID_LIST_MP_MODULES, ICONLIST_BROWSESELECT | ICONLIST_DETAILED | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	//pListMPModules->setSortFunc( deglGuiBuilder::SortListItemByName );
	pListMPModules->appendHeader( "Module", NULL, 150 );
	pListMPModules->appendHeader( "Type", NULL, 120 );
	
	FXVerticalFrame *vframe = new FXVerticalFrame( frameTabHorz, LAYOUT_FILL_X | LAYOUT_FILL_Y, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3 );
	frameDeco = new FXPacker( vframe, LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	
	FXScrollWindow *sframe = new FXScrollWindow( frameDeco, LAYOUT_FILL_X | LAYOUT_FILL_Y );
	sframe->setBackColor( getApp()->getBaseColor() );
	pFrameMPParameters = new FXMatrix( sframe, 2, LAYOUT_FILL_X | MATRIX_BY_COLUMNS );
	
	frameDeco = new FXPacker( vframe, LAYOUT_FILL_X | FRAME_SUNKEN, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
	pTextMPParameterInfo = new FXText( frameDeco, NULL, 0, LAYOUT_FILL_X | LAYOUT_FILL_Y | TEXT_READONLY | TEXT_WORDWRAP );
	pTextMPParameterInfo->setVisibleRows( 6 );
	
	FXMatrix *optLine = new FXMatrix( vframe, 3, MATRIX_BY_COLUMNS | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0 );
	new FXRadioButton( optLine, "Basic", this, ID_MPPARAM_OPT_BASIC, RADIOBUTTON_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	new FXRadioButton( optLine, "Advanced", this, ID_MPPARAM_OPT_ADVANCED, RADIOBUTTON_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	new FXRadioButton( optLine, "Expert", this, ID_MPPARAM_OPT_EXPERT, RADIOBUTTON_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_COLUMN );
	
	
	// Run Parameters
	new FXTabItem( pTabPanels, "Run Parameters", NULL, TAB_TOP_NORMAL, 0, 0, 0, 0, 10, 10, 2, 2 );
	
	frameTab = new FXVerticalFrame( pTabPanels, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X, 0, 0, 0, 0, 10, 10, 10, 10, 0, 3 );
	
	block = guiBuilder.CreateMatrixPacker( frameTab, 2 );
	
	toolTip = "Sets the arguments to use";
	guiBuilder.CreateLabel( block, "Run Arguments:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditRunArgs = guiBuilder.CreateTextField( frameLine, this, ID_EDIT_RUNARGS, toolTip, 20, false );
	
	toolTip = "Determines if the run arguments replace the\ngame run arguments or are added to them";
	guiBuilder.CreateLabel( block, "", "" );
	frameLine = guiBuilder.CreateHFrame( block );
	pChkReplaceRunArgs = guiBuilder.CreateCheckBox( frameLine, "Replace Game Run Arguments", this, ID_CHK_REPLACE_RUNARGS, toolTip );
	
	toolTip = "Sets the initial size of game window";
	guiBuilder.CreateLabel( block, "Window Size:", toolTip );
	frameLine = guiBuilder.CreateHFrame( block );
	pEditWidth = guiBuilder.CreateNumberField( frameLine, this, ID_EDIT_WIDTH, toolTip, 6, true );
	guiBuilder.CreateLabel( frameLine, " x ", toolTip, 0 );
	pEditHeight = guiBuilder.CreateNumberField( frameLine, this, ID_EDIT_HEIGHT, toolTip, 6, true );
	guiBuilder.CreateLabel( frameLine, " Full-Screen:", toolTip, 10 );
	pCBFullScreenResolutions = guiBuilder.CreateComboBox( frameLine, this, ID_CB_FULLSCREEN_RESOLUTIONS,
		"Full screen resolutions", false, 12, 8, true );
	UpdateFullScreenResolutions();
	
	groupBox = guiBuilder.CreateGroupBox( frameTab, "Disable Module Versions:", false );
	splitter = new FXSplitter( groupBox, LAYOUT_FILL_X | SPLITTER_NORMAL | SPLITTER_HORIZONTAL );
	
	frameVertical = new FXVerticalFrame( splitter, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X );
	pCBDisableModuleVersionModule = guiBuilder.CreateComboBox( frameVertical, this, ID_CB_DISABLE_MODULE_VERSION_MODULE,
		"Name of module to disable", true, 10, 6, false );
	//pCBDisableModuleVersionModule->setSortFunc( );
	
	pCBDisableModuleVersionVersion = guiBuilder.CreateComboBox( frameVertical, this, ID_CB_DISABLE_MODULE_VERSION_VERSION,
		"Version of module to disable", true, 10, 6, false );
	//pCBDisableModuleVersionVersion->setSortFunc( );
	
	frameLine2 = guiBuilder.CreateHFrame( frameVertical );
	pBtnDisableModuleVersionRemove = guiBuilder.CreateButton( frameLine2, "<< Remove", NULL, this, ID_BTN_DISABLE_MODULE_VERSION_REMOVE,
		"Remove module version from the list of module versions to disable" );
	pBtnDisableModuleVersionRemove->setFrameStyle( pBtnDisableModuleVersionRemove->getFrameStyle() | LAYOUT_FILL_X );
	
	pBtnDisableModuleVersionAdd = guiBuilder.CreateButton( frameLine2, "Add >>", NULL, this, ID_BTN_DISABLE_MODULE_VERSION_ADD,
		"Add module version to the list of module versions to disable" );
	pBtnDisableModuleVersionAdd->setFrameStyle( pBtnDisableModuleVersionAdd->getFrameStyle() | LAYOUT_FILL_X );
	
	frameVertical = new FXVerticalFrame( splitter, FRAME_RAISED | LAYOUT_FILL_Y | LAYOUT_FILL_X );
	pListDisableModuleVersions = new FXList( frameVertical, this, ID_LIST_DISABLE_MODULE_VERSIONS, LIST_NORMAL | LAYOUT_FILL_X | LAYOUT_FILL_Y );
	pListDisableModuleVersions->setNumVisible( 4 );
	
	splitter->setSplit( 0, 250 );
	
	// buttons below
	frameGroup =  new FXVerticalFrame( content, LAYOUT_SIDE_TOP | LAYOUT_FILL_X, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 );
	
	new FXSeparator( frameGroup );
	
	frameLine = new FXHorizontalFrame( frameGroup, LAYOUT_CENTER_X, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0 );
	new FXButton( frameLine, "Accept", NULL, this, ID_ACCEPT, LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	new FXButton( frameLine, "Cancel", NULL, this, ID_CANCEL, LAYOUT_CENTER_X | FRAME_RAISED | JUSTIFY_NORMAL | ICON_BEFORE_TEXT, 0, 0, 0, 0, 30, 30 );
	
	
	// create editing copy of profiles
	const deglGameManager &gameManager = *pWindowMain->GetLauncher()->GetGameManager();
	deObjectReference refObject;
	int i;
	
	for( i=0; i<gameManager.GetProfileList().GetProfileCount(); i++ ){
		refObject.TakeOver( new cEditProfile( *this, gameManager.GetProfileList().GetProfileAt( i ), NULL ) );
		pProfiles.Add( refObject );
	}
	
	for( i=0; i<gameManager.GetGameList().GetCount(); i++ ){
		deglGame * const game = gameManager.GetGameList().GetAt( i );
		if( game->GetCustomProfile() ){
			refObject.TakeOver( new cEditProfile( *this, game->GetCustomProfile(), game ) );
			pProfiles.Add( refObject );
		}
	}
	
	UpdateProfileList();
	   pSetSelectedProfile( selectProfile );
}

deglDialogProfileList::~deglDialogProfileList(){
}



// Management
///////////////

void deglDialogProfileList::UpdateFullScreenResolutions(){
	const deglEngine &engine = *pWindowMain->GetLauncher()->GetEngine();
	const int count = engine.GetResolutionCount();
	FXString text;
	int i;
	
	pCBFullScreenResolutions->clearItems();
	pCBFullScreenResolutions->appendItem( "Window" );
	for( i=0; i<count; i++ ){
		const decPoint &resolution = engine.GetResolutionAt( i );
		text.format( "%d x %d", resolution.x, resolution.y );
		pCBFullScreenResolutions->appendItem( text );
	}
	
	pCBFullScreenResolutions->setCurrentItem( 0 );
}

void deglDialogProfileList::UpdateSystemModuleLists(){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	int i, count = moduleList.GetModuleCount();
	
	pSysGraphic.combobox->clearItems();
	pSysInput.combobox->clearItems();
	pSysPhysics.combobox->clearItems();
	pSysAnimator.combobox->clearItems();
	pSysAI.combobox->clearItems();
	pSysCrashRecovery.combobox->clearItems();
	pSysAudio.combobox->clearItems();
	pSysNetwork.combobox->clearItems();
	pSysSynthesizer.combobox->clearItems();
	
	for( i=0; i<count; i++ ){
		deglEngineModule * const module = moduleList.GetModuleAt( i );
		switch( ( deModuleSystem::eModuleTypes )module->GetType() ){
		case deModuleSystem::emtGraphic:
			pSysGraphic.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtInput:
			pSysInput.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtPhysics:
			pSysPhysics.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtAnimator:
			pSysAnimator.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtAI:
			pSysAI.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtCrashRecovery:
			pSysCrashRecovery.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtAudio:
			pSysAudio.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtSynthesizer:
			pSysSynthesizer.combobox->appendItem( module->GetName().GetString() );
			break;
			
		case deModuleSystem::emtNetwork:
			pSysNetwork.combobox->appendItem( module->GetName().GetString() );
			break;
			
		default:
			break;
		}
	}
	
	pSysGraphic.combobox->sortItems();
	pSysInput.combobox->sortItems();
	pSysPhysics.combobox->sortItems();
	pSysAnimator.combobox->sortItems();
	pSysAI.combobox->sortItems();
	pSysCrashRecovery.combobox->sortItems();
	pSysAudio.combobox->sortItems();
	pSysSynthesizer.combobox->sortItems();
	pSysNetwork.combobox->sortItems();
	
	UpdateProfile();
}

void deglDialogProfileList::UpdateProfileList(){
	cEditProfile * const selected = pGetSelectedProfile();
	
	pListProfiles->clearItems();
	
	const int count = pProfiles.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		cEditProfile * const profile = ( cEditProfile* )pProfiles.GetAt( i );
		pListProfiles->appendItem( profile->GetText(), profile->GetIcon(), profile );
	}
	
	pListProfiles->sortItems();
	
	pListProfiles->setCurrentItem( pListProfiles->findItemByData( selected ) );
	if( pListProfiles->getCurrentItem() == -1 && count > 0 ){
		pListProfiles->setCurrentItem( 0 );
	}
	
	UpdateProfile();
}

void deglDialogProfileList::UpdateProfile(){
	pSystemsValid = true;
	
	if( pGetSelectedProfile() ){
		deglGameProfile &profile = *pGetSelectedProfile()->GetEdit();
		FXString text;
		
		UpdateSystem( pSysGraphic, profile.GetModuleGraphic() );
		UpdateSystem( pSysInput, profile.GetModuleInput() );
		UpdateSystem( pSysPhysics, profile.GetModulePhysics() );
		UpdateSystem( pSysAnimator, profile.GetModuleAnimator() );
		UpdateSystem( pSysAI, profile.GetModuleAI() );
		UpdateSystem( pSysCrashRecovery, profile.GetModuleCrashRecovery() );
		UpdateSystem( pSysAudio, profile.GetModuleAudio() );
		UpdateSystem( pSysSynthesizer, profile.GetModuleSynthesizer() );
		UpdateSystem( pSysNetwork, profile.GetModuleNetwork() );
		
		pEditRunArgs->setText( profile.GetRunArguments().GetString() );
		pEditRunArgs->enable();
		
		pChkReplaceRunArgs->setCheck( profile.GetReplaceRunArguments() );
		pChkReplaceRunArgs->enable();
		
		text.format( "%d", profile.GetWidth() );
		pEditWidth->setText( text );
		
		text.format( "%d", profile.GetHeight() );
		pEditHeight->setText( text );
		
		if( profile.GetFullScreen() ){
			// not found is index -1 which becomes 0 which is correct
			pCBFullScreenResolutions->setCurrentItem( pWindowMain->GetLauncher()->GetEngine()->
				IndexOfClosestResolutionTo( profile.GetWidth(), profile.GetHeight() ) + 1 );
			
		}else{
			pCBFullScreenResolutions->setCurrentItem( 0 );
		}
		pCBFullScreenResolutions->enable();
		
		if( pCBFullScreenResolutions->getCurrentItem() == 0 ){
			pEditWidth->enable();
			pEditHeight->enable();
			
		}else{
			pEditWidth->disable();
			pEditHeight->disable();
		}
		
	}else{
		DisableSystem( pSysGraphic );
		DisableSystem( pSysInput );
		DisableSystem( pSysPhysics );
		DisableSystem( pSysAnimator );
		DisableSystem( pSysAI );
		DisableSystem( pSysCrashRecovery );
		DisableSystem( pSysAudio );
		DisableSystem( pSysSynthesizer );
		DisableSystem( pSysNetwork );
		
		pEditRunArgs->setText( "" );
		pEditRunArgs->disable();
		
		pChkReplaceRunArgs->setCheck( false );
		pChkReplaceRunArgs->disable();
		
		pEditWidth->setText( "" );
		pEditWidth->disable();
		
		pEditHeight->setText( "" );
		pEditHeight->disable();
		
		pCBFullScreenResolutions->setCurrentItem( 0 );
		pCBFullScreenResolutions->disable();
	}
	
	UpdateMPModuleList();
	UpdateMPParameterList();
	
	pTabSystems->setIcon( pSystemsValid ? pWindowMain->GetIconValidSmall() : pWindowMain->GetIconInvalidSmall() );
	
	UpdateListDisabledModuleVersions();
	UpdateCBDisabledModuleVersionsModule();
	UpdateCBDisabledModuleVersionsVersion();
}

void deglDialogProfileList::UpdateSystem( sSystem &system, const char *moduleName ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule *module;
	bool working = true;
	
	system.combobox->setText( moduleName );
	module = moduleList.GetModuleNamed( moduleName );
	
	if( ! module ){
		system.problem->setText( "No module exists with the given name." );
		working = false;
		
	}else if( module->GetType() != system.type ){
		system.problem->setText( "Module is not of the correct type." );
		working = false;
		
	}else if( module->GetStatus() != deglEngineModule::emsReady ){
		system.problem->setText( "Module has problems and can not be used." );
		working = false;
	}
	
	if( working ){
		system.icon->setIcon( pWindowMain->GetIconValidSmall() );
		system.problem->setText( "" );
		system.problem->hide();
		
	}else{
		system.icon->setIcon( pWindowMain->GetIconInvalidSmall() );
		system.problem->show();
	}
	
	system.combobox->enable();
	
	pSystemsValid &= working;
}

void deglDialogProfileList::DisableSystem( sSystem &system ){
	system.combobox->setText( "" );
	system.problem->setText( "" );
	system.problem->hide();
	system.icon->setIcon( pWindowMain->GetIconValidSmall() );
	system.combobox->disable();
}



bool deglDialogProfileList::GetSelectedMPModuleName( FXString &name ){
	int selection = pListMPModules->getCurrentItem();
	
	if( selection == -1 ){
		return false;
	}
	
	name = pListMPModules->getItemText( selection ).section( '\t', 0 );
	return true;
}

void deglDialogProfileList::UpdateMPModuleList(){
	deglEngine &engine = *pWindowMain->GetLauncher()->GetEngine();
	const deglEngineModuleList &moduleList = engine.GetModuleList();
	int m, moduleCount = moduleList.GetModuleCount();
	deglEngineModule *module;
	FXString text;
	
	pListMPModules->clearItems();
	
	for( m=0; m<moduleCount; m++ ){
		module = moduleList.GetModuleAt( m );
		
		text.format( "%s\t%s", module->GetName().GetString(), engine.GetModuleTypeText( module->GetType() ) );
		pListMPModules->appendItem( text );
	}
}

void deglDialogProfileList::UpdateMPParameterList(){
	pMPParameters.RemoveAll();
	
	FXString moduleName;
	if( ! GetSelectedMPModuleName( moduleName ) || ! pGetSelectedProfile() ){
		return;
	}
	
	// add parameters defined by the engine module
	const deglEngineModule * const engineModule = pWindowMain->GetLauncher()->
		GetEngine()->GetModuleList().GetModuleNamed( moduleName.text() );
	if( ! engineModule ){
		return;
	}
	
	const deglEMParameterList &list = engineModule->GetParameterList();
	const int count = list.GetParameterCount();
	deObjectReference dplparam;
	decStringList names;
	int i;
	
	for( i=0; i<count; i++ ){
		names.Add( list.GetParameterAt( i )->GetInfo().GetName() );
	}
	names.SortAscending();
	
	deglGameProfile &profile = *pGetSelectedProfile()->GetEdit();
	for( i=0; i<count; i++ ){
		dplparam.TakeOver( new deglDialogProfileListParameter(
			*list.GetParameterNamed( names.GetAt( i ) ), profile, moduleName.text(),
			pFrameMPParameters, this, ID_MPPARAM_LABEL, ID_MPPARAM_VALUE ) );
		pMPParameters.Add( dplparam );
	}
	
	pUpdateMPParamVisiblity();
}



void deglDialogProfileList::UpdateListDisabledModuleVersions(){
	pListDisableModuleVersions->clearItems();
	
	if( pGetSelectedProfile() ){
		const deglGPDisableModuleVersionList &list = pGetSelectedProfile()->GetEdit()->GetDisableModuleVersionList();
		const int count = list.GetCount();
		FXString selection;
		FXString text;
		int i;
		
		if( pListDisableModuleVersions->getCurrentItem() != -1 ){
			selection = pListDisableModuleVersions->getItemText( pListDisableModuleVersions->getCurrentItem() );
		}
		
		for( i=0; i<count; i++ ){
			deglGPDisableModuleVersion * const version = list.GetAt( i );
			text.format( "%s (%s)", version->GetName().GetString(), version->GetVersion().GetString() );
			pListDisableModuleVersions->appendItem( text, NULL, version );
		}
		
		pListDisableModuleVersions->setCurrentItem( pListDisableModuleVersions->findItem( selection ) );
	}
}

void deglDialogProfileList::UpdateCBDisabledModuleVersionsModule(){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	const int moduleCount = moduleList.GetModuleCount();
	decStringSet moduleNames;
	FXString selection;
	int i;
	
	pCBDisableModuleVersionModule->clearItems();
	
	for( i=0; i<moduleCount; i++ ){
		const deglEngineModule &module = *moduleList.GetModuleAt( i );
		moduleNames.Add( module.GetName() );
	}
	moduleNames.SortAscending();
	
	const int nameCount = moduleNames.GetCount();
	for( i=0; i<nameCount; i++ ){
		pCBDisableModuleVersionModule->appendItem( moduleNames.GetAt( i ).GetString() );
	}
	
	if( pCBDisableModuleVersionModule->getNumItems() > 0 ){
		pCBDisableModuleVersionModule->setCurrentItem( 0 );
	}
	
	UpdateCBDisabledModuleVersionsVersion();
}

void deglDialogProfileList::UpdateCBDisabledModuleVersionsVersion(){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	const FXString selectedModule = pCBDisableModuleVersionModule->getText();
	const int moduleCount = moduleList.GetModuleCount();
	decStringSet versions;
	FXString selection;
	int i;
	
	pCBDisableModuleVersionVersion->clearItems();
	
	for( i=0; i<moduleCount; i++ ){
		const deglEngineModule &module = *moduleList.GetModuleAt( i );
		if( module.GetName() == selectedModule.text() ){
			versions.Add( module.GetVersion() );
		}
	}
	
    versions.SortAscending();
	
	const int versionCount = versions.GetCount();
	for( i=0; i<versionCount; i++ ){
		pCBDisableModuleVersionVersion->appendItem( versions.GetAt( i ).GetString() );
	}
}



FXuint deglDialogProfileList::execute( FXuint placement ){
	UpdateSystemModuleLists();
	
	if( ! FXDialogBox::execute( placement ) ){
		return 0;
	}
	
	deglGameManager &gameManager = *pWindowMain->GetLauncher()->GetGameManager();
	const int profileCount = pProfiles.GetCount();
	int i;
	
	gameManager.GetProfileList().RemoveAllProfiles();
	
	for( i=0; i<profileCount; i++ ){
		const cEditProfile &editProfile = *( ( cEditProfile* )pProfiles.GetAt( i ) );
		
		if( editProfile.GetGameCustom() ){
			editProfile.GetGameCustom()->GetCustomProfile()->CopyFrom( *editProfile.GetEdit() );
			
		}else if( editProfile.GetOriginal() ){
			editProfile.GetOriginal()->CopyFrom( *editProfile.GetEdit() );
			editProfile.GetOriginal()->SetName( editProfile.GetEdit()->GetName() );
			gameManager.GetProfileList().AddProfile( editProfile.GetOriginal() );
			
		}else{
			gameManager.GetProfileList().AddProfile( editProfile.GetEdit() );
		}
	}
	
	return 1;
}



// Events
///////////

long deglDialogProfileList::onListProfilesChanged( FXObject*, FXSelector, void* ){
	const int selectedModule = pListMPModules->getCurrentItem();
	UpdateProfile();
	pListMPModules->setCurrentItem( selectedModule );
	UpdateMPParameterList();
	return 1;
}

long deglDialogProfileList::onBtnProfAdd( FXObject*, FXSelector, void* ){
	FXString name( "New Profile" );
	while( FXInputDialog::getString( name, this, "Add Profile", "Name:" ) ){
		if( pGetProfileNamed( name.text() ) ){
			FXMessageBox::error( this, MBOX_OK, "Invalid Name", "A profile with this name exists already." );
			
		}else if( name.length() < 1 ){
			FXMessageBox::error( this, MBOX_OK, "Invalid Name", "Profiles with empty name are not allowed." );
			
		}else{
			try{
				deObjectReference profile;
				profile.TakeOver( new cEditProfile( *this, name.text() ) );
				( ( cEditProfile& )( deObject& )profile ).GetEdit()->CopyFrom(
					*pWindowMain->GetLauncher()->GetGameManager()->GetDefaultProfile() );
				pProfiles.Add( profile );
				
				UpdateProfileList();
				pSetSelectedProfile( ( cEditProfile* )( deObject* )profile );
				UpdateProfile();
				
			}catch( const deException &e ){
				pWindowMain->DisplayException( e );
			}
			break;
		}
	}
	return 1;
}

long deglDialogProfileList::onBtnProfDup( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	FXString name( "New Profile" );
	while( FXInputDialog::getString( name, this, "Add Profile", "Name:" ) ){
		if( pGetProfileNamed( name.text() ) ){
			FXMessageBox::error( this, MBOX_OK, "Invalid Name", "A profile with this name exists already. Choose another name." );
			
		}else if( name.length() < 1 ){
			FXMessageBox::error( this, MBOX_OK, "Invalid Name", "Profiles with empty name are not allowed." );
			
		}else{
			try{
				deObjectReference profile;
				profile.TakeOver( new cEditProfile( *this, name.text() ) );
				( ( cEditProfile& )( deObject& )profile ).GetEdit()->CopyFrom( *pGetSelectedProfile()->GetEdit() );
				pProfiles.Add( profile );
				
				UpdateProfileList();
				pSetSelectedProfile( ( cEditProfile* )( deObject* )profile );
				UpdateProfile();
				
			}catch( const deException &e ){
				pWindowMain->DisplayException( e );
			}
			break;
		}
	}
	return 1;
}

long deglDialogProfileList::onBtnProfDel( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() || pGetSelectedProfile()->GetGameCustom() ){
		return 1;
	}
	
	if( FXMessageBox::question( this, MBOX_YES_NO, "Delete Profile", "Do you really want to delete the profile '%s'?",
	pGetSelectedProfile()->GetEdit()->GetName().GetString() ) == MBOX_CLICKED_YES ){
		pProfiles.Remove( pGetSelectedProfile() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::updateBtnProfDel( FXObject *sender, FXSelector, void* ){
	return sender->tryHandle( sender, FXSEL( SEL_COMMAND, pGetSelectedProfile()
		&& ! pGetSelectedProfile()->GetGameCustom() ? ID_ENABLE : ID_DISABLE ), NULL );
}

long deglDialogProfileList::onBtnProfRename( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() || pGetSelectedProfile()->GetGameCustom() ){
		return 1;
	}
	
	deglGameProfile &profile = *pGetSelectedProfile()->GetEdit();
	FXString name( profile.GetName().GetString() );
	
	while( FXInputDialog::getString( name, this, "Rename Profile", "Name:" ) ){
		if( profile.GetName() == name.text() ){
			break;
			
		}else if( pGetProfileNamed( name.text() ) ){
			FXMessageBox::error( this, MBOX_OK, "Invalid Name", "A profile with this name exists already." );
			
		}else if( name.empty() ){
			FXMessageBox::error( this, MBOX_OK, "Invalid Name", "Profiles with empty name are not allowed." );
			
		}else{
			profile.SetName( name.text() );
			UpdateProfileList();
			break;
		}
	}
	return 1;
}

long deglDialogProfileList::updateBtnProfRename( FXObject *sender, FXSelector, void* ){
	return sender->tryHandle( sender, FXSEL( SEL_COMMAND, pGetSelectedProfile()
		&& ! pGetSelectedProfile()->GetGameCustom() ? ID_ENABLE : ID_DISABLE ), NULL );
}



long deglDialogProfileList::onCBModGraChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	deglGameProfile &profile = *pGetSelectedProfile()->GetEdit();
	profile.SetModuleGraphic( pSysGraphic.combobox->getText().text() );
	profile.Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysGraphic, profile.GetModuleGraphic() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnGraModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleGraphic() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysGraphic, pGetSelectedProfile()->GetEdit()->GetModuleGraphic() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModInpChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	deglGameProfile &profile = *pGetSelectedProfile()->GetEdit();
	profile.SetModuleInput( pSysInput.combobox->getText().text() );
	profile.Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysInput, profile.GetModuleInput() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnInpModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleInput() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysInput, pGetSelectedProfile()->GetEdit()->GetModuleGraphic() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModPhyChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModulePhysics( pSysPhysics.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysPhysics, pGetSelectedProfile()->GetEdit()->GetModulePhysics() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnPhyModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModulePhysics() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysPhysics, pGetSelectedProfile()->GetEdit()->GetModulePhysics() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModAmrChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModuleAnimator( pSysAnimator.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysAnimator, pGetSelectedProfile()->GetEdit()->GetModuleAnimator() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnAmrModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleAnimator() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysAnimator, pGetSelectedProfile()->GetEdit()->GetModuleAnimator() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModAIChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModuleAI( pSysAI.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysAI, pGetSelectedProfile()->GetEdit()->GetModuleAI() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnAIModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleAI() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysAI, pGetSelectedProfile()->GetEdit()->GetModuleAI() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModCRChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModuleCrashRecovery( pSysCrashRecovery.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysCrashRecovery, pGetSelectedProfile()->GetEdit()->GetModuleCrashRecovery() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnCRModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleCrashRecovery() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysCrashRecovery, pGetSelectedProfile()->GetEdit()->GetModuleCrashRecovery() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModAudChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModuleAudio( pSysAudio.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysAudio, pGetSelectedProfile()->GetEdit()->GetModuleAudio() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnAudModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleAudio() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysAudio, pGetSelectedProfile()->GetEdit()->GetModuleAudio() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModNetChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModuleNetwork( pSysNetwork.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysNetwork, pGetSelectedProfile()->GetEdit()->GetModuleNetwork() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnNetModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleNetwork() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysNetwork, pGetSelectedProfile()->GetEdit()->GetModuleNetwork() );
		UpdateProfileList();
	}
	return 1;
}

long deglDialogProfileList::onCBModSynChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	pGetSelectedProfile()->GetEdit()->SetModuleSynthesizer( pSysSynthesizer.combobox->getText().text() );
	pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
	UpdateSystem( pSysSynthesizer, pGetSelectedProfile()->GetEdit()->GetModuleSynthesizer() );
	UpdateProfileList();
	return 1;
}

long deglDialogProfileList::onBtnSynModInfo( FXObject*, FXSelector, void* ){
	const deglEngineModuleList &moduleList = pWindowMain->GetLauncher()->GetEngine()->GetModuleList();
	deglEngineModule * const module = pGetSelectedProfile() ? moduleList.GetModuleNamed(
		pGetSelectedProfile()->GetEdit()->GetModuleSynthesizer() ) : NULL;
	
	if( module ){
		deglDialogModuleProps( pWindowMain, module, this ).execute( PLACEMENT_OWNER );
		pGetSelectedProfile()->GetEdit()->Verify( *pWindowMain->GetLauncher() );
		UpdateSystem( pSysSynthesizer, pGetSelectedProfile()->GetEdit()->GetModuleSynthesizer() );
		UpdateProfileList();
	}
	return 1;
}



long deglDialogProfileList::onListMPModulesChanged( FXObject*, FXSelector, void* ){
	UpdateMPParameterList();
	return 1;
}

long deglDialogProfileList::onMPParameterValueCommand( FXObject *sender, FXSelector, void* ){
	try{
		const int count = pMPParameters.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			if( ( ( deglDialogProfileListParameter* )pMPParameters.GetAt( i ) )->ProcessSelCommand( sender ) ){
				break;
			}
		}
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
	}
	return 1;
}

long deglDialogProfileList::onMPParameterValueChanged( FXObject *sender, FXSelector, void* ){
	try{
		const int count = pMPParameters.GetCount();
		int i;
		for( i=0; i<count; i++ ){
			if( ( ( deglDialogProfileListParameter* )pMPParameters.GetAt( i ) )->ProcessSelChanged( sender ) ){
				break;
			}
		}
	}catch( const deException &e ){
		pWindowMain->DisplayException( e );
	}
	return 1;
}

long deglDialogProfileList::onMPParameterValueFocusIn( FXObject *sender, FXSelector, void* ){
	const int count = pMPParameters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deglDialogProfileListParameter &parameter = *( ( deglDialogProfileListParameter* )pMPParameters.GetAt( i ) );
		if( parameter.SenderMatchesEdit( sender ) ){
			pTextMPParameterInfo->setText( parameter.GetDescription().GetString() );
			break;
		}
	}
	return 1;
}

long deglDialogProfileList::onMPParameterLabelLMPress( FXObject *sender, FXSelector, void* ){
	const int count = pMPParameters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		const deglDialogProfileListParameter &parameter = *( ( deglDialogProfileListParameter* )pMPParameters.GetAt( i ) );
		if( parameter.SenderMatchesLabel( sender ) ){
			pTextMPParameterInfo->setText( parameter.GetDescription().GetString() );
			break;
		}
	}
	return 1;
}

long deglDialogProfileList::onMPParameterLabelLMRelease( FXObject*, FXSelector, void* ){
	return 1;
}

long deglDialogProfileList::onMPParameterLabelRMPress( FXObject *sender, FXSelector, void* data){
	const int count = pMPParameters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deglDialogProfileListParameter * const parameter = ( deglDialogProfileListParameter* )pMPParameters.GetAt( i );
		if( ! parameter->SenderMatchesLabel( sender ) ){
			continue;
		}
		
		const FXEvent &event = *( ( const FXEvent * )data );
		FXMenuPane *popup = NULL;
		int x = event.root_x;
		int y = event.root_y;
		
		pPUMPParameter = parameter;
		
		try{
			popup = new FXMenuPane( this );
			new FXMenuCommand( popup, "Reset Value", NULL, this, ID_PU_PARAM_RESET );
			
			popup->create();
			
			popup->popup( NULL, x + 1, y + 1 ); // popup-bug. do not show straight under the cursor
			pWindowMain->getApp()->runModalWhileShown( popup );
			
			delete popup;
			
		}catch( const deException &e ){
			if( popup ){
				delete popup;
			}
			pPUMPParameter = NULL;
			pWindowMain->DisplayException( e );
		}
	}
	
	return 1;
}

long deglDialogProfileList::onMPParameterLabelRMRelease( FXObject*, FXSelector, void* ){
	return 1;
}

long deglDialogProfileList::onMPParameterOptBasic( FXObject*, FXSelector, void* ){
	pMPParameterCategory = deModuleParameter::ecBasic;
	pUpdateMPParamVisiblity();
	return 1;
}

long deglDialogProfileList::updateMPParameterOptBasic( FXObject *sender, FXSelector, void* ){
	return sender->tryHandle( sender, FXSEL( SEL_COMMAND,
		pMPParameterCategory == deModuleParameter::ecBasic
			? FXRadioButton::ID_CHECK : FXRadioButton::ID_UNCHECK ), 0 );
}

long deglDialogProfileList::onMPParameterOptAdvanced( FXObject*, FXSelector, void* ){
	pMPParameterCategory = deModuleParameter::ecAdvanced;
	pUpdateMPParamVisiblity();
	return 1;
}

long deglDialogProfileList::updateMPParameterOptAdvanced( FXObject *sender, FXSelector, void* ){
	return sender->tryHandle( sender, FXSEL( SEL_COMMAND,
		pMPParameterCategory == deModuleParameter::ecAdvanced
			? FXRadioButton::ID_CHECK : FXRadioButton::ID_UNCHECK ), 0 );
}

long deglDialogProfileList::onMPParameterOptExpert( FXObject*, FXSelector, void* ){
	if( pMPParameterCategory != deModuleParameter::ecExpert && ! pAllowExpertMode ){
		if( FXMessageBox::warning( this, MBOX_YES_NO, "Enable Expert Parameters",
		"Do you really want to enable expert parameters?\n\nYou can easily break modules with them!" )
		!= MBOX_CLICKED_YES ){
			return 1;
		}
	}
	
	pAllowExpertMode = true;
	pMPParameterCategory = deModuleParameter::ecExpert;
	pUpdateMPParamVisiblity();
	return 1;
}

long deglDialogProfileList::updateMPParameterOptExpert( FXObject* sender, FXSelector, void* ){
	return sender->tryHandle( sender, FXSEL( SEL_COMMAND,
		pMPParameterCategory == deModuleParameter::ecExpert
			? FXRadioButton::ID_CHECK : FXRadioButton::ID_UNCHECK ), 0 );
}



long deglDialogProfileList::onEditRunArgsChanged( FXObject*, FXSelector, void* ){
	if( pGetSelectedProfile() ){
		pGetSelectedProfile()->GetEdit()->SetRunArguments( pEditRunArgs->getText().text() );
	}
	return 1;
}

long deglDialogProfileList::onChkReplaceRunArgsChanged( FXObject*, FXSelector, void* ){
	if( pGetSelectedProfile() ){
		pGetSelectedProfile()->GetEdit()->SetReplaceRunArguments( pChkReplaceRunArgs->getCheck() );
	}
	return 1;
}

long deglDialogProfileList::onEditWidthChanged( FXObject*, FXSelector, void* ){
	if( pGetSelectedProfile() ){
		int width = pEditWidth->getText().toInt();
		if( width < 1 ){
			pEditWidth->setText( "1" );
			width = 1;
		}
		pGetSelectedProfile()->GetEdit()->SetWidth( width );
	}
	return 1;
}

long deglDialogProfileList::onCBFullScreenResolutionsChanged( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	const int selection = pCBFullScreenResolutions->getCurrentItem();
	if( selection < 1 ){
		pGetSelectedProfile()->GetEdit()->SetFullScreen( false );
		
	}else{
		const decPoint &resolution = pWindowMain->GetLauncher()->GetEngine()->GetResolutionAt( selection - 1 );
		pGetSelectedProfile()->GetEdit()->SetWidth( resolution.x );
		pGetSelectedProfile()->GetEdit()->SetHeight( resolution.y );
		pGetSelectedProfile()->GetEdit()->SetFullScreen( true );
	}
	
	UpdateProfile();
	return 1;
}

long deglDialogProfileList::onEditHeightChanged( FXObject*, FXSelector, void* ){
	if( pGetSelectedProfile() ){
		int height = pEditHeight->getText().toInt();
		if( height < 1 ){
			pEditHeight->setText( "1" );
			height = 1;
		}
		pGetSelectedProfile()->GetEdit()->SetHeight( height );
	}
	return 1;
}

long deglDialogProfileList::onCBDisableModuleVersionModuleChanged( FXObject*, FXSelector, void* ){
	UpdateCBDisabledModuleVersionsVersion();
	return 0;
}

long deglDialogProfileList::onBtnDisableModuleVersionAdd( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	const FXString selectedModule = pCBDisableModuleVersionModule->getText();
	if( selectedModule.empty() ){
		return 1;
	}
	
	const FXString selectedVersion = pCBDisableModuleVersionVersion->getText();
	if( selectedVersion.empty() ){
		return 1;
	}
	
	deglGPDisableModuleVersionList &list = pGetSelectedProfile()->GetEdit()->GetDisableModuleVersionList();
	if( ! list.HasWith( selectedModule.text(), selectedVersion.text() ) ){
		deObjectReference version;
		try{
			version.TakeOver( new deglGPDisableModuleVersion( selectedModule.text(), selectedVersion.text() ) );
			list.Add( ( deglGPDisableModuleVersion* )( deObject* )version );
		}catch( const deException &e ){
			GetWindowMain()->DisplayException( e );
			return 1;
		}
		UpdateListDisabledModuleVersions();
	}
	return 1;
}

long deglDialogProfileList::onBtnDisableModuleVersionRemove( FXObject*, FXSelector, void* ){
	if( ! pGetSelectedProfile() ){
		return 1;
	}
	
	const int selection = pListDisableModuleVersions->getCurrentItem();
	if( selection == -1 ){
		return 1;
	}
	
	deglGPDisableModuleVersion * const version =
		( deglGPDisableModuleVersion* )pListDisableModuleVersions->getItemData( selection );
	pGetSelectedProfile()->GetEdit()->GetDisableModuleVersionList().Remove( version );
	
	UpdateListDisabledModuleVersions();
	return 1;
}



long deglDialogProfileList::onPUParamReset( FXObject*, FXSelector, void* ){
	if( pPUMPParameter ){
		try{
			pPUMPParameter->Reset();
		}catch( const deException &e ){
			pWindowMain->DisplayException( e );
		}
		pPUMPParameter = NULL;
		return 1;
	}
	return 1;
}



// Private Functions
//////////////////////

void deglDialogProfileList::pCreateSystem( sSystem &system, const char *textLabel, const char *toolText, int comboBoxSelector,
int buttonSelector, FXComposite *container ){
	deglConfiguration &configuration = *pWindowMain->GetLauncher()->GetConfiguration();
	const deglGuiBuilder &guiBuilder = *pWindowMain->GetGuiBuilder();
	
	guiBuilder.CreateLabel( container, textLabel, toolText );
	
	FXVerticalFrame * const frameRight = new FXVerticalFrame( container, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 );
	
	FXHorizontalFrame * const frameLine = new FXHorizontalFrame( frameRight, LAYOUT_SIDE_TOP | LAYOUT_FILL_X |
		LAYOUT_FILL_Y | LAYOUT_FILL_ROW | LAYOUT_FILL_COLUMN, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1 );
	system.icon = new FXLabel( frameLine, "", pWindowMain->GetIconValidSmall(), LABEL_NORMAL, 0, 0, 0, 0, 0, 0, 0, 0 );
	system.combobox = guiBuilder.CreateComboBox( frameLine, this, comboBoxSelector, toolText, true, 20, 5, false );
	system.combobox->setSortFunc( deglGuiBuilder::SortListItemByName );
	system.modinfo = guiBuilder.CreateButton( frameLine, "", pWindowMain->GetIconButtonInfo(), this, buttonSelector, "Show module informations" );
	system.modinfo->setLayoutHints( system.modinfo->getLayoutHints() | LAYOUT_FILL_Y );
	
	system.problem = new FXLabel( frameRight, "This label contains a short problem description.", NULL, LABEL_NORMAL | LAYOUT_FILL_X );
	system.problem->setJustify( JUSTIFY_LEFT | JUSTIFY_TOP );
	system.problem->setBackColor( configuration.GetBackColorProblem() );
	system.problem->setTextColor( configuration.GetTextColorProblem() );
}

void deglDialogProfileList::pUpdateMPParamVisiblity(){
	const int count = pMPParameters.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deglDialogProfileListParameter* )pMPParameters.GetAt( i ) )->UpdateVisibility( pMPParameterCategory );
	}
	pFrameMPParameters->getParent()->recalc();
}

deglDialogProfileList::cEditProfile *deglDialogProfileList::pGetSelectedProfile() const{
	if( pListProfiles->getCurrentItem() == -1 ){
		return NULL;
	}
	return ( cEditProfile* )pListProfiles->getItemData( pListProfiles->getCurrentItem() );
}

void deglDialogProfileList::pSetSelectedProfile( deglGameProfile *profile ){
	const int count = pListProfiles->getNumItems();
	int selection = -1;
	int i;
	
	for( i=0; i<count; i++ ){
		const cEditProfile &editProfile = *( ( cEditProfile* )pListProfiles->getItemData( i ) );
		if( editProfile.GetOriginal() == profile ){
			selection = i;
			break;
		}
	}
	
	if( selection == -1 && count > 0 ){
		selection = 0;
	}
	
	pListProfiles->setCurrentItem( selection, true );
}

void deglDialogProfileList::pSetSelectedProfile( cEditProfile *profile ){
	pListProfiles->setCurrentItem( pListProfiles->findItemByData( profile ) );
}

deglDialogProfileList::cEditProfile *deglDialogProfileList::pGetProfileNamed( const char *name ) const{
	const int count = pProfiles.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		cEditProfile * const profile = ( cEditProfile* )pProfiles.GetAt( i );
		if( ! profile->GetGameCustom() && profile->GetEdit()->GetName() == name ){
			return profile;
		}
	}
	return NULL;
}
