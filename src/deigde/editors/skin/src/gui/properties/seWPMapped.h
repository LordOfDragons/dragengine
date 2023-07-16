/* 
 * Drag[en]gine IGDE Skin Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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
