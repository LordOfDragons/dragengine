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

#ifndef _DEDSXMLDOCUMENT_H_
#define _DEDSXMLDOCUMENT_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/common/xmlparser/decXmlDocument.h>


/**
 * \brief Xml document extension.
 */
class dedsXmlDocument : public decXmlDocument{
public:
	/** \brief Element types. */
	enum eElementTypes{
		/** \brief Element. */
		eetElement,
		
		/** \brief Tag. */
		eetTag,
		
		/** \brief CData. */
		eetCData,
		
		/** \brief Comment. */
		eetComment
	};
	
	
	
private:
	decString pFilename;
	decString pParseLog;
	bool pParseFailed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates xml document. */
	dedsXmlDocument(const char *filename);
	
protected:
	/** \brief Cleans up xml document. */
	virtual ~dedsXmlDocument();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Filename. */
	inline const decString GetFilename() const{ return pFilename; }
	
	/** \brief Set filename. */
	void SetFilename(const char *filename);
	
	/** \brief Parse failed. */
	inline bool GetParseFailed() const{ return pParseFailed; }
	
	/** \brief Set parse failed. */
	void SetParseFailed(bool parseFailed);
	
	/** \brief Parse log. */
	inline const decString &GetParseLog() const{ return pParseLog; }
	
	/** \brief Set parse log. */
	void SetParseLog(const char *text);
};

#endif
