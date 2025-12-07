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

#ifndef _DECXMLVISITORWRITER_H_
#define _DECXMLVISITORWRITER_H_

#include "../decXmlVisitor.h"

class decXmlWriter;
class decBaseFileWriter;


/**
 * \brief Visitor writing xml document to file using decXmlWriter.
 */
class DE_DLL_EXPORT decXmlVisitorWriter : public decXmlVisitor{
private:
	bool pCompact;
	decXmlWriter *pWriter;
	bool pTagIndent;
	bool pTagNewline;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	decXmlVisitorWriter();
	
	/** \brief Clean up visitor. */
	virtual ~decXmlVisitorWriter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Writer or NULL. */
	inline decXmlWriter *GetWriter(){return pWriter;}
	
	/** \brief Write compact XML for example for network transfer. */
	inline bool GetCompact() const{return pCompact;}
	
	/** \brief Set to write compact XML for example for network transfer. */
	void SetCompact(bool compact);
	
	/** \brief Write XML document to file. */
	void WriteDocument(decBaseFileWriter *file, decXmlDocument &document);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit comment. */
	virtual void VisitComment(decXmlComment &comment);
	
	/** \brief Visit processing instructions. */
	virtual void VisitPI(decXmlPI &pi);
	
	/** \brief Visit element tag. */
	virtual void VisitElementTag(decXmlElementTag &tag);
	
	/** \brief Visit element character data. */
	virtual void VisitCharacterData(decXmlCharacterData &data);
	
	/** \brief Visit element entity reference. */
	virtual void VisitEntityReference(decXmlEntityReference &ref);
	
	/** \brief Visit character reference. */
	virtual void VisitCharReference(decXmlCharReference &ref);
	
	/** \brief Visit character data section. */
	virtual void VisitCDSect(decXmlCDSect &cdsect);
	
	/** \brief Visit attribute value. */
	virtual void VisitAttValue(decXmlAttValue &value);
	
	/** \brief Visit namespace. */
	virtual void VisitNamespace(decXmlNamespace &ns);
	/*@}*/
};

#endif
