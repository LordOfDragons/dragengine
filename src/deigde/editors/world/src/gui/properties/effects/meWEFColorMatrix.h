/* 
 * Drag[en]gine IGDE World Editor
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

#if 0

// include only once
#ifndef _MEWEFCOLORMATRIX_H_
#define _MEWEFCOLORMATRIX_H_

// includes
#include <stdio.h>
#include <stdlib.h>

#include <deigde/gui/foxtoolkit.h>

// predefinitions
class meWindowEffects;
class meWTColorMatrix;
class deEffectColorMatrix;



/**
 * Panel for editing a color matrix effect.
 */
class meWEFColorMatrix : public FXVerticalFrame{
	FXDECLARE( meWEFColorMatrix )
	
protected:
	meWEFColorMatrix();
	
public:
	enum eFoxIDs{
		ID_CANVAS = FXVerticalFrame::ID_LAST,
		ID_TABLEMATRIX,
		ID_CHKENABLE,
		ID_CBTEMPLATES,
		ID_BTNAPPLYTEMPLATE,
		ID_BTNSAVETEMPLATE,
		ID_LAST
	};
	
private:
	meWindowEffects *pWndEffects;
	deEffectColorMatrix *pEffectColorMatrix;
	
	meWTColorMatrix **pTemplates;
	int pTemplateCount;
	int pTemplateSize;
	
	FXTable *pTblMatrix;
	FXCheckButton *pChkEnable;
	FXComboBox *pCBTemplates;
	
public:
	// constructor, destructor
	meWEFColorMatrix( deEffectColorMatrix *effectColorMatrix, meWindowEffects *windowEffects, FXComposite *container );
	virtual ~meWEFColorMatrix();
	
	// management
	void UpdateColorMatrix();
	
	void AddTemplate( meWTColorMatrix *aTemplate );
	
	// events
	long onTableMatrixChanged( FXObject *sender, FXSelector selector, void *data );
	long onCheckEnableCommand( FXObject *sender, FXSelector selector, void *data );
	long onCBTemplatesChanged( FXObject *sender, FXSelector selector, void *data );
	long onBtnApplyTemplateCommand( FXObject *sender, FXSelector selector, void *data );
	long onBtnSaveTemplateCommand( FXObject *sender, FXSelector selector, void *data );
	
private:
	void pAddTemplates();
};

// end of include only once
#endif


#endif
