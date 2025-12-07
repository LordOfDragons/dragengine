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

#ifndef _DEALBASEXML_H_
#define _DEALBASEXML_H_

#include "common/string/decString.h"

class decXmlWriter;
class decXmlElementTag;
class deLogger;



/**
 * @brief Basic XML Load/Save Class.
 * Class used as base class for xml load/save classes. Provides a set of helper
 * function required by all xml load/save classes.
 */
class dealBaseXML{
private:
	deLogger *pLogger;
	decString pLoggerSource;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new base xml read/save. */
	dealBaseXML(deLogger *logger, const char *loggerSource);
	/** Cleans up the base xml read/save. */
	virtual ~dealBaseXML();
	/*@}*/
	
protected:
	/** @name Management */
	/*@{*/
	/** Retrieves the logger. */
	inline deLogger *GetLogger() const{return pLogger;}
	/** Retrieves the logger source. */
	inline const decString &GetLoggerSource() const{return pLoggerSource;}
	
	bool pHasAttribute(const decXmlElementTag &tag, const char *name) const;
	const char *pGetAttributeString(const decXmlElementTag &tag, const char *name) const;
	int pGetAttributeInt(const decXmlElementTag &tag, const char *name) const;
	float pGetAttributeFloat(const decXmlElementTag &tag, const char *name) const;
	int pGetCDataInt(const decXmlElementTag &tag) const;
	float pGetCDataFloat(const decXmlElementTag &tag) const;
	const char *pGetCDataString(const decXmlElementTag &tag) const;
	
	void pErrorUnknownTag(const decXmlElementTag &root, const decXmlElementTag &tag);
	void pErrorUnknownValue(const decXmlElementTag &tag, const char *value);
	void pErrorMissingTag(const decXmlElementTag &root, const char *tagName);
	/*@}*/
};

#endif // _DEGLGAME_H_
