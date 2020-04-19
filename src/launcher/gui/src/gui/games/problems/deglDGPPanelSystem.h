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

#ifndef _DEGLDGPPANELSYSTEM_H_
#define _DEGLDGPPANELSYSTEM_H_

#include "../../foxtoolkit.h"

class deglDialogGameProblems;



/**
 * @brief Game Problems Dialog Systems Panel.
 * Shows problems with systems.
 */
class deglDGPPanelSystem : public FXVerticalFrame{
	FXDECLARE( deglDGPPanelSystem )
protected:
	deglDGPPanelSystem();
	
public:
	enum eFoxIDs{
		ID_SELF = FXVerticalFrame::ID_LAST,
		
		ID_CB_MOD_GRA,
		ID_CB_MOD_INP,
		ID_CB_MOD_PHY,
		ID_CB_MOD_AMR,
		ID_CB_MOD_AI,
		ID_CB_MOD_CR,
		ID_CB_MOD_AUD,
		ID_CB_MOD_NET,
		ID_CB_MOD_SYN,
		
		ID_LAST
	};
	
private:
	struct sSystem{
		FXVerticalFrame *box;
		FXLabel *label;
		FXComboBox *combobox;
		FXLabel *problem;
		int type;
	};
	
	deglDialogGameProblems *pParentDialog;
	bool pStatusWorking;
	
	sSystem pSysGraphic;
	sSystem pSysInput;
	sSystem pSysPhysics;
	sSystem pSysAnimator;
	sSystem pSysAI;
	sSystem pSysCrashRecovery;
	sSystem pSysAudio;
	sSystem pSysSynthesizer;
	sSystem pSysNetwork;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new dialog. */
	deglDGPPanelSystem( deglDialogGameProblems *parentDialog, FXComposite *container );
	/** Cleans up the dialog. */
	virtual ~deglDGPPanelSystem();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the parent dialog. */
	inline deglDialogGameProblems *GetParentDialog() const{ return pParentDialog; }
	/** Determines if the system is working. */
	inline bool GetStatusWorking() const{ return pStatusWorking; }
	
	/** Update modules. */
	void UpdateSystemModuleLists();
	/** Update panel. */
	void UpdatePanel();
	/** Update a system. */
	void UpdateSystem( sSystem &system, const char *moduleName );
	/*@}*/
	
	/** @name Events */
	/*@{*/
	long onCBModGraChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModInpChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModPhyChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModAmrChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModAIChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModCRChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModAudChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModNetChanged( FXObject *sender, FXSelector selector, void *data );
	long onCBModSynChanged( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
	
private:
	void pCreateSystem( sSystem &system, const char *textLabel, const char *toolText, int comboBoxSelector, FXComposite *container );
};

// end of include only once
#endif
