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

#ifndef _AEWPAPANELRULEANIMATIONSELECT_H_
#define _AEWPAPANELRULEANIMATIONSELECT_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/igdeListBoxReference.h>


/**
 * \brief Animation select rule panel.
 */
class aeWPAPanelRuleAnimationSelect : public aeWPAPanelRule{
private:
	igdeComboBoxFilterReference pCBMoves;
	igdeListBoxReference pListMoves;
	igdeButtonReference pBtnMoveAdd;
	igdeCheckBoxReference pChkEnablePosition;
	igdeCheckBoxReference pChkEnableRotation;
	igdeCheckBoxReference pChkEnableSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new panel. */
	aeWPAPanelRuleAnimationSelect( aeWPRule &wpRule );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAPanelRuleAnimationSelect();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update animation move list. */
	virtual void UpdateAnimMoveList();
	
	/** \brief Update rule. */
	virtual void UpdateRule();
	
	/** \brief Update target list. */
	virtual void UpdateTargetList();
	
	/** \brief Combo box move text. */
	const decString &GetCBMovesText() const;
	
	/** \brief List box move selection. */
	int GetListMovesSelection() const;
	/*@}*/
};

#endif
