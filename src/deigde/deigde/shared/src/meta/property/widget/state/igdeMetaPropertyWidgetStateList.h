/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEMETAPROPERTYWIDGETSTATELIST_H_
#define _IGDEMETAPROPERTYWIDGETSTATELIST_H_

#include "igdeMetaPropertyWidgetState.h"


/**
 * \brief Meta property widget state for list and set type widgets.
 */
class DE_DLL_EXPORT igdeMetaPropertyWidgetStateList : public igdeMetaPropertyWidgetState{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyWidgetStateList>;
	
	
	int rows;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create meta property widget state for property. */
	igdeMetaPropertyWidgetStateList();
	
protected:
	/** \brief Clean up widget state. */
	~igdeMetaPropertyWidgetStateList() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Load state from file. */
	void LoadFromFile(igdeBaseXML &baseXml, const decXmlElementTag &root) override;
	
	/** \brief Save state to file. */
	void SaveToFile(igdeBaseXML &baseXml, decXmlWriter &writer) const override;
	/*@}*/
};

#endif
