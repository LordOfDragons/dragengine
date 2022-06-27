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

#ifndef _AEWPAPANELRULESUBANIMATOR_H_
#define _AEWPAPANELRULESUBANIMATOR_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/composed/igdeEditPathReference.h>


class aeController;


/**
 * \brief Sub Animator Rule Panel.
 */
class aeWPAPanelRuleSubAnimator : public aeWPAPanelRule{
private:
	igdeEditPathReference pEditPathAnimator;
	igdeComboBoxReference pCBConnection;
	igdeComboBoxReference pCBConnectionController;
	igdeCheckBoxReference pChkEnablePosition;
	igdeCheckBoxReference pChkEnableRotation;
	igdeCheckBoxReference pChkEnableSize;
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	aeWPAPanelRuleSubAnimator( aeWPRule &wpRule );
	
protected:
	/** \brief Clean up panel. */
	virtual ~aeWPAPanelRuleSubAnimator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Update connection target list. */
	void UpdateConnectionList();
	
	/** \brief Animator path changed. */
	virtual void OnAnimatorPathChanged();
	
	/** \brief Update controller list. */
	virtual void UpdateControllerList();
	
	/** \brief Update rule. */
	virtual void UpdateRule();
	
	/** \brief Update connection. */
	virtual void UpdateConnection();
	
	/** \brief Selected combobox connection. */
	int GetCBConnection() const;
	
	/** \brief Selected combobox connection controller. */
	aeController *GetCBConnectionController() const;
	/*@}*/
};

#endif
