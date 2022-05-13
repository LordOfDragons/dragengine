/* 
 * Drag[en]gine GUI Launcher
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

#ifndef _DEGLDIALOGPROFILELISTPARAMETER_H_
#define _DEGLDIALOGPROFILELISTPARAMETER_H_

#include "../foxtoolkit.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/systems/modules/deModuleParameter.h>

class delEMParameter;
class delGameProfile;


/**
 * Profiles panel module parameter.
 */
class deglDialogProfileListParameter : public deObject{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglDialogProfileListParameter> Ref;
	
	
	
private:
	delEMParameter &pParameter;
	delGameProfile &pProfile;
	decString pModuleName;
	
	decString pDescription;
	
	FXLabel *pLabel;
	FXComboBox *pComboBox;
	FXHorizontalFrame *pFrameSlider;
	FXRealSlider *pSlider;
	FXTextField *pSliderText;
	FXCheckButton *pCheckBox;
	FXTextField *pTextField;
	
	bool pCustomized;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter. */
	deglDialogProfileListParameter( delEMParameter &parameter, delGameProfile &profile,
		const char *moduleName, FXMatrix *container, FXObject *target, 
		int labelSelector, int valueSelector );
	
protected:
	/** Clean up parameter. */
	virtual ~deglDialogProfileListParameter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Description. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** Sender matches label. */
	bool SenderMatchesLabel( FXObject *sender ) const;
	
	/** Sender matches edit widget. */
	bool SenderMatchesEdit( FXObject *sender ) const;
	
	/**
	 * Process command message if sender is edit widget of this parameter.
	 * \returns true if processed or false if not destined for this parameter.
	 */
	bool ProcessSelCommand( FXObject *sender );
	
	/**
	 * Process changed message if sender is edit widget of this parameter.
	 * \returns true if processed or false if not destined for this parameter.
	 */
	bool ProcessSelChanged( FXObject *sender );
	
	/** Set parameter value. */
	void SetParameterValue( const char *value );
	
	/** Set parameter value. */
	void SetParameterValue( FXdouble value );
	
	/** Update edit widget value from parameter value. */
	void Update();
	
	/** Reset value to default. */
	void Reset();
	
	/** Update visibility. */
	void UpdateVisibility( deModuleParameter::eCategory category );
	/*@}*/
};

#endif
