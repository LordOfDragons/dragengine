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

#ifndef _AEWPANIMATOR_H_
#define _AEWPANIMATOR_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>


class aeAnimator;
class aeWindowProperties;
class aeWPAnimatorListener;



/**
 * \brief Animator panel.
 */
class aeWPAnimator : public igdeContainerScroll{
private:
	aeWindowProperties &pWindowProperties;
	aeWPAnimatorListener *pListener;
	aeAnimator *pAnimator;
	
	igdeEditPathReference pEditRigPath;
	igdeEditPathReference pEditAnimPath;
	
	igdeListBoxReference pListBones;
	igdeComboBoxFilterReference pCBBones;
	igdeButtonReference pBtnBoneAdd;
	igdeButtonReference pBtnBoneDel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	aeWPAnimator( aeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAnimator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Properties window. */
	inline aeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Animator. */
	inline aeAnimator *GetAnimator() const{ return pAnimator; }
	
	/** \brief Set animator. */
	void SetAnimator( aeAnimator *animator );
	
	/** \brief Update animator. */
	void UpdateAnimator();
	
	/** \brief Update rig bone list. */
	virtual void UpdateRigBoneList();
	
	/** \brief Rig bone combo box text. */
	const decString &GetCBRigBoneText() const;
	
	/** \brief Set rig bone combo box text. */
	void SetCBRigBoneText( const char *text );
	/*@}*/
};

#endif
