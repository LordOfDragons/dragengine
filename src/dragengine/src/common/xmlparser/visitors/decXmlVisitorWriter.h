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
	~decXmlVisitorWriter() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Writer or NULL. */
	inline decXmlWriter *GetWriter(){ return pWriter; }
	
	/** \brief Write compact XML for example for network transfer. */
	inline bool GetCompact() const{ return pCompact; }
	
	/** \brief Set to write compact XML for example for network transfer. */
	void SetCompact(bool compact);
	
	/** \brief Write XML document to file. */
	void WriteDocument(decBaseFileWriter *file, decXmlDocument &document);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit comment. */
	void VisitComment(decXmlComment &comment) override;
	
	/** \brief Visit processing instructions. */
	void VisitPI(decXmlPI &pi) override;
	
	/** \brief Visit element tag. */
	void VisitElementTag(decXmlElementTag &tag) override;
	
	/** \brief Visit element character data. */
	void VisitCharacterData(decXmlCharacterData &data) override;
	
	/** \brief Visit element entity reference. */
	void VisitEntityReference(decXmlEntityReference &ref) override;
	
	/** \brief Visit character reference. */
	void VisitCharReference(decXmlCharReference &ref) override;
	
	/** \brief Visit character data section. */
	void VisitCDSect(decXmlCDSect &cdsect) override;
	
	/** \brief Visit attribute value. */
	void VisitAttValue(decXmlAttValue &value) override;
	
	/** \brief Visit namespace. */
	void VisitNamespace(decXmlNamespace &ns) override;
	/*@}*/
};

#endif
