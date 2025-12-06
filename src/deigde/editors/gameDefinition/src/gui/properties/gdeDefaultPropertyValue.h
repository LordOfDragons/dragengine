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

#ifndef _GDEDEFAULTPROPERTYVALUE_H_
#define _GDEDEFAULTPROPERTYVALUE_H_

#include "../../gamedef/property/gdeProperty.h"

#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gui/composed/igdeEditPropertyValue.h>
#include <deigde/gamedefinition/property/igdeGDProperty.h>

class gdeProperty;

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
		igdeEditPropertyValue::Ref &widget, igdeEditPropertyValueListener *listener );
	
	static void CreateAndAdd( igdeContainer &form, igdeUIHelper &helper, const char *label,
		const char *description, igdeEditPropertyValue::Ref &widget,
		igdeEditPropertyValueListener *listener );
	
	/** \brief Set value. */
	void SetValue( const decString &value, const gdeProperty &property );
	/*@}*/
};

#endif
