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


#ifndef _GDEUOCTPCFPSETEXTENSION_H_
#define _GDEUOCTPCFPSETEXTENSION_H_

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/property/gdeProperty.h"
#include "../../../gamedef/filepattern/gdeFilePattern.h"



/**
 * \brief Undo action object class texture property custom file pattern set pattern.
 */
class gdeUOCTPCFPSetExtension : public igdeUndo{
public:
	typedef deTObjectReference<gdeUOCTPCFPSetExtension> Ref;
	
	
private:
	gdeObjectClass::Ref pObjectClass;
	gdeProperty::Ref pProperty;
	gdeFilePattern::Ref pFilePattern;
	
	decString pOldValue;
	decString pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCTPCFPSetExtension(gdeObjectClass *objectClass, gdeProperty *property,
		gdeFilePattern *filePattern, const char *newValue);
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCTPCFPSetExtension();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
