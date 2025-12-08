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

#ifndef _GDEWPSOCNAVIGATIONSPACE_H_
#define _GDEWPSOCNAVIGATIONSPACE_H_

#include "../../../gamedef/objectClass/navspace/gdeOCNavigationSpace.h"

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditVector.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCNavigationSpace;
class gdeWindowProperties;
class gdeWPSOCNavigationSpaceListener;



/**
 * \brief Object class navigation space property panel.
 */
class gdeWPSOCNavigationSpace : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCNavigationSpaceListener::Ref pListener;
	
	gdeGameDefinition::Ref pGameDefinition;
	
	igdeEditPath::Ref pEditPath;
	igdeEditVector::Ref pEditPosition;
	igdeEditVector::Ref pEditRotation;
	igdeTextField::Ref pEditBoneName;
	igdeTextField::Ref pEditLayer;
	igdeTextField::Ref pEditSnapDistance;
	igdeComboBox::Ref pCBType;
	igdeTextField::Ref pEditSnapAngle;
	igdeTextField::Ref pEditBlockingPriority;
	igdeTextField::Ref pEditBlockerShape;
	
	igdeComboBox::Ref pCBPropertyNames;
	igdeComboBoxFilter::Ref pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCNavigationSpace(gdeWindowProperties &windowMain);
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCNavigationSpace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition(gdeGameDefinition::Ref gameDefinition);
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class navigation space or \em NULL if not set. */
	gdeOCNavigationSpace *GetNavigationSpace() const;
	
	/** \brief Selected property name. */
	const gdeOCNavigationSpace::eProperties GetPropertyName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update navigation space. */
	void UpdateNavigationSpace();
	
	/** \brief Update navigation space property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
