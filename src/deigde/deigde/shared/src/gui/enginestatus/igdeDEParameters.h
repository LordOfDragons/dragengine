/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEDEPARAMETERS_H_
#define _IGDEDEPARAMETERS_H_

#include "../igdeButtonReference.h"
#include "../igdeComboBoxReference.h"
#include "../igdeTextAreaReference.h"
#include "../igdeTextFieldReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/systems/modules/deModuleParameter.h>


class igdeDialogEngine;
class deModuleParameter;
class deLoadableModule;


/**
 * \brief Panel showing the parameters of modules allowing the user to also change them.
 */
class DE_DLL_EXPORT igdeDEParameters : public igdeContainerFlow{
private:
	igdeDialogEngine &pDialogEngine;
	deModuleParameter pParameterInfo;
	
	igdeComboBoxReference pCBModule;
	igdeComboBoxReference pCBParameter;
	igdeTextAreaReference pEditDescription;
	igdeTextFieldReference pEditType;
	igdeTextAreaReference pEditAllowedValues;
	igdeTextFieldReference pEditValue;
	
	igdeButtonReference pBtnSet;
	igdeButtonReference pBtnReset;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeDEParameters( igdeDialogEngine &dialogEngine );
	
	
	
protected:
	/** \brief Cleans up panel. */
	virtual ~igdeDEParameters();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	void UpdateParameter();
	void UpdateParametersList();
	void UpdateModulesList();
	
	/** \brief Selected module. */
	deLoadableModule *GetSelectedModule() const;
	
	/** \brief Selected parameter. */
	const decString &GetSelectedParameter() const;
	
	/** \brief Set parameter value. */
	void ParameterSetValue();
	
	/** \brief Reset parameter value. */
	void ParameterResetValue();
	/*@}*/
};

#endif
