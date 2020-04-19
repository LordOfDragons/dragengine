/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEDEFAULTPROPERTYVALUE_H_
#define _GDEDEFAULTPROPERTYVALUE_H_

#include "../../gamedef/property/gdeProperty.h"

#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>


class gdeProperty;
class igdeEditPropertyValueReference;


/**
 * \brief Default property value widget.
 * 
 * Modified igdeEditPropertyValue to use a temporary igdeGDProperty filled with values
 * from a gdeProperty.
 */
class gdeDefaultPropertyValue : public igdeEditPropertyValue{
private:
	igdeGDProperty *pGDProperty;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create widget. */
	gdeDefaultPropertyValue( igdeUIHelper &helper );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeDefaultPropertyValue();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Create and add. */
	static void CreateAndAdd( igdeContainer &parent, igdeUIHelper &helper,
		igdeEditPropertyValueReference &widget, igdeEditPropertyValueListener *listener );
	
	static void CreateAndAdd( igdeContainer &form, igdeUIHelper &helper, const char *label,
		const char *description, igdeEditPropertyValueReference &widget,
		igdeEditPropertyValueListener *listener );
	
	/** \brief Set value. */
	void SetValue( const decString &value, const gdeProperty &property );
	/*@}*/
};

#endif
