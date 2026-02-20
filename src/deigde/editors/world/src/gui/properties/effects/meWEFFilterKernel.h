#if 0

/* 
 * Drag[en]gine Map Editor
 *
 * Copyright (C) 2004, Plüss Roland ( roland@rptd.ch )
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

// include only once
#ifndef _MEWEFFILTERKERNEL_H_
#define _MEWEFFILTERKERNEL_H_

// includes
#include <stdio.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>

// predefinitions
class meWindowEffects;
class meWTFilterKernel;
class deEffectFilterKernel;



/**
 * Panel for editing an overlay image effect.
 */
class meWEFFilterKernel : public FXVerticalFrame{
	FXDECLARE(meWEFFilterKernel)
protected:
	meWEFFilterKernel();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_EDITROWS,
		ID_EDITCOLS,
		ID_TABLEKERNEL,
		ID_EDIT_SCALE,
		ID_CHKENABLE,
		ID_CBTEMPLATES,
		ID_BTNAPPLYTEMPLATE,
		ID_BTNSAVETEMPLATE,
		ID_LAST
	};
	
private:
	meWindowEffects *pWndEffects;
	deEffectFilterKernel *pEffectFilterKernel;
	
	meWTFilterKernel **pTemplates;
	int pTemplateCount;
	int pTemplateSize;
	
	FXTextField *pEditRows;
	FXTextField *pEditCols;
	FXTable *pTblKernel;
	FXTextField *pEditScale;
	FXCheckButton *pChkEnable;
	FXComboBox *pCBTemplates;
	
public:
	// constructor, destructor
	meWEFFilterKernel(deEffectFilterKernel *effectFilterKernel, meWindowEffects *windowEffects, FXComposite *container);
	virtual ~meWEFFilterKernel();
	
	// management
	void UpdateFilterKernel();
	
	void AddTemplate(meWTFilterKernel *aTemplate);
	
	// events
	long onEditRowsCommand(FXObject *sender, FXSelector selector, void *data);
	long onEditColsCommand(FXObject *sender, FXSelector selector, void *data);
	long onTableKernelChanged(FXObject *sender, FXSelector selector, void *data);
	long onEditScaleCommand(FXObject *sender, FXSelector selector, void *data);
	long onCheckEnableCommand(FXObject *sender, FXSelector selector, void *data);
	long onCBTemplatesChanged(FXObject *sender, FXSelector selector, void *data);
	long onBtnApplyTemplateCommand(FXObject *sender, FXSelector selector, void *data);
	long onBtnSaveTemplateCommand(FXObject *sender, FXSelector selector, void *data);
	
private:
	void pAddTemplates();
};

// end of include only once
#endif

#endif
