/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEWPLINK_H_
#define _AEWPLINK_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class aeAnimator;
class aeLink;
class aeWindowProperties;
class aeWPLinkListener;
class aeEditLinkMapping;



/**
 * Link panel.
 */
class aeWPLink : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPLinkListener *pListener;
	aeAnimator *pAnimator;
	
	igdeListBoxReference pListLink;
	
	igdeTextFieldReference pEditName;
	igdeComboBoxReference pCBController;
	igdeSpinTextFieldReference pSpinRepeat;
	igdeViewCurveBezierReference pEditCurve;
	igdeComboBoxFilterReference pCBBone;
	igdeComboBoxReference pCBBoneParameter;
	igdeTextFieldReference pEditBoneMinimum;
	igdeTextFieldReference pEditBoneMaximum;
	igdeComboBoxFilterReference pCBVertexPositionSet;
	igdeTextFieldReference pEditVertexPositionSetMinimum;
	igdeTextFieldReference pEditVertexPositionSetMaximum;
	igdeCheckBoxReference pChkWrapY;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create panel. */
	aeWPLink( aeWindowProperties &windowProperties );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPLink();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** Active link. */
	aeLink *GetLink() const;
	
	/** Update link list. */
	void UpdateLinkList();
	
	/** Select active link. */
	void SelectActiveLink();
	
	/** Update link. */
	void UpdateLink();
	
	/** Update rig bone list. */
	void UpdateRigBoneList();
	
	/** Update model vertex position set list. */
	void UpdateModelVertexPositionSetList();
	
	/** Update controller list. */
	void UpdateControllerList();
	/*@}*/
};

#endif
