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

#ifndef _SEWPMAPPED_H_
#define _SEWPMAPPED_H_

#include "seWPMappedListener.h"
#include "../../skin/seSkin.h"

#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class seMapped;
class seWindowProperties;


/**
 * Mapped panel.
 */
class seWPMapped : public igdeContainerScroll{
private:
	seWindowProperties &pWindowProperties;
	const seWPMappedListener::Ref pListener;
	
	seSkin::Ref pSkin;
	bool pRequiresUpdate;
	
	igdeListBoxReference pListMapped;
	igdeTextFieldReference pEditName;
	
	igdeViewCurveBezierReference pEditCurve;
	igdeComboBoxReference pCBInputType;
	igdeTextFieldReference pEditInputLower;
	igdeTextFieldReference pEditInputUpper;
	igdeCheckBoxReference pChkInputClamped;
	igdeTextFieldReference pEditOutputLower;
	igdeTextFieldReference pEditOutputUpper;
	igdeTextFieldReference pEditBone;
	igdeTextFieldReference pEditRenderable;
	igdeComboBoxReference pCBRenderableComponent;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	seWPMapped( seWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~seWPMapped();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline seWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Skin. */
	inline const seSkin::Ref &GetSkin() const{ return pSkin; }
	
	/** Set skin. */
	void SetSkin( seSkin *skin );
	
	/** Active mapped. */
	seMapped *GetMapped() const;
	
	/** Update mapped list. */
	void UpdateMappedList();
	
	/** Change selection to the active mapped. */
	void SelectActiveMapped();
	
	/** Update mapped parameters. */
	void UpdateMapped();
	
	/** Fit view to curve. */
	void FitViewToCurve();
	/*@}*/
};

#endif
