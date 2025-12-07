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

#ifndef _DEGLDGPPANELSYSTEM_H_
#define _DEGLDGPPANELSYSTEM_H_

#include "../../foxtoolkit.h"

class deglDialogGameProblems;



/**
 * Game Problems Dialog Systems Panel.
 * Shows problems with systems.
 */
class deglDGPPanelSystem : public FXVerticalFrame{
	FXDECLARE(deglDGPPanelSystem)
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
		ID_CB_MOD_VR,
		
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
	sSystem pSysVR;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create dialog. */
	deglDGPPanelSystem(deglDialogGameProblems *parentDialog, FXComposite *container);
	
	/** Clean up dialog. */
	virtual ~deglDGPPanelSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent dialog. */
	inline deglDialogGameProblems *GetParentDialog() const{return pParentDialog;}
	
	/** System is working. */
	inline bool GetStatusWorking() const{return pStatusWorking;}
	
	/** Update modules. */
	void UpdateSystemModuleLists();
	
	/** Update panel. */
	void UpdatePanel();
	
	/** Update a system. */
	void UpdateSystem(sSystem &system, const char *moduleName);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onCBModGraChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModInpChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModPhyChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModAmrChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModAIChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModCRChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModAudChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModNetChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModSynChanged(FXObject *sender, FXSelector selector, void *data);
	long onCBModVRChanged(FXObject *sender, FXSelector selector, void *data);
	/*@}*/
	
	
	
private:
	void pCreateSystem(sSystem &system, const char *textLabel, const char *toolText,
		int comboBoxSelector, FXComposite *container);
};

#endif
