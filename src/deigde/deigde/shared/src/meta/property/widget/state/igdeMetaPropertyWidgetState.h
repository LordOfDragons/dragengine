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

#ifndef _IGDEMETAPROPERTYWIDGETSTATE_H_
#define _IGDEMETAPROPERTYWIDGETSTATE_H_

#include <dragengine/deObject.h>

class igdeBaseXML;
class decXmlElementTag;
class decXmlWriter;


/**
 * \brief Meta property widget state.
 */
class DE_DLL_EXPORT igdeMetaPropertyWidgetState : public deObject{
public:
	/** \brief Reference type. */
	using Ref = deTObjectReference<igdeMetaPropertyWidgetState>;
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create meta property widget state for property. */
	igdeMetaPropertyWidgetState() = default;
	
	/** \brief Clean up widget state. */
	virtual ~igdeMetaPropertyWidgetState() = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Load state from file. */
	virtual void LoadFromFile(igdeBaseXML &baseXml, const decXmlElementTag &root) = 0;
	
	/** \brief Save state to file. */
	virtual void SaveToFile(igdeBaseXML &baseXml, decXmlWriter &writer) const = 0;
	/*@}*/
};

#endif
