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
	FXDECLARE(meWEFColorMatrix)
	
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
	meWEFColorMatrix(deEffectColorMatrix *effectColorMatrix, meWindowEffects *windowEffects, FXComposite *container);
	virtual ~meWEFColorMatrix();
	
	// management
	void UpdateColorMatrix();
	
	void AddTemplate(meWTColorMatrix *aTemplate);
	
	// events
	long onTableMatrixChanged(FXObject *sender, FXSelector selector, void *data);
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
