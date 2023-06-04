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

#ifndef _AEWPAPANELRULESTATEMANIPULATOR_H_
#define _AEWPAPANELRULESTATEMANIPULATOR_H_

#include "aeWPAPanelRule.h"

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>



/**
 * State Manipulator Rule Panel.
 */
class aeWPAPanelRuleStateManipulator : public aeWPAPanelRule{
private:
	igdeEditVectorReference pEditMinPos;
	igdeEditVectorReference pEditMaxPos;
	igdeEditVectorReference pEditMinRot;
	igdeEditVectorReference pEditMaxRot;
	igdeEditVectorReference pEditMinSize;
	igdeEditVectorReference pEditMaxSize;
	igdeTextFieldReference pEditMinVertexPositionSet;
	igdeTextFieldReference pEditMaxVertexPositionSet;
	igdeCheckBoxReference pChkEnablePosition;
	igdeCheckBoxReference pChkEnableRotation;
	igdeCheckBoxReference pChkEnableSize;
	igdeCheckBoxReference pChkEnableVertexPositionSet;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new panel. */
	aeWPAPanelRuleStateManipulator( aeWPRule &wpRule );
	
protected:
	/** Clean up panel. */
	virtual ~aeWPAPanelRuleStateManipulator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update rule. */
	virtual void UpdateRule();
	
	/** Update target list. */
	virtual void UpdateTargetList();
	/*@}*/
};

#endif
