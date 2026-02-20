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

#ifndef _DELBASEXML_H_
#define _DELBASEXML_H_

#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>

class decXmlWriter;
class decXmlElementTag;



/**
 * \brief Basic XML Load/Save Class.
 */
class DE_DLL_EXPORT delBaseXML{
private:
	deLogger::Ref pLogger;
	decString pLoggerSource;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create base xml read/save. */
	delBaseXML(deLogger *logger, const char *loggerSource);
	
	/** \brief Clean up base xml read/save. */
	virtual ~delBaseXML();
	/*@}*/
	
	
	
protected:
	/** \name Management */
	/*@{*/
	/** \brief Logger. */
	inline const deLogger::Ref &GetLogger() const{ return pLogger; }
	
	/** \brief Logger source. */
	inline const decString &GetLoggerSource() const{ return pLoggerSource; }
	
	/** \brief Named attribute is present. */
	bool HasAttribute(const decXmlElementTag &tag, const char *name) const;
	
	/**
	 * \brief Named attribute value.
	 * \throws EInvalidParam Named attribute is absent.
	 */
	const decString &GetAttributeString(const decXmlElementTag &tag, const char *name) const;
	
	/**
	 * \brief Named attribute value.
	 * \throws EInvalidParam Named attribute is absent.
	 */
	int GetAttributeInt(const decXmlElementTag &tag, const char *name) const;
	
	/**
	 * \brief Named attribute value.
	 * \throws EInvalidParam Named attribute is absent.
	 */
	float GetAttributeFloat(const decXmlElementTag &tag, const char *name) const;
	
	/** \brief First CDATA value or 0 if absent. */
	int GetCDataInt(const decXmlElementTag &tag) const;
	
	/** \brief First CDATA value or 0 if absent. */
	float GetCDataFloat(const decXmlElementTag &tag) const;
	
	/** \brief First CDATA value or empty string if absent. */
	const decString &GetCDataString(const decXmlElementTag &tag) const;
	
	/**
	 * \brief First CDATA value.
	 * \throws EInvalidParam CDATA absent.
	 */
	bool GetCDataBool(const decXmlElementTag &tag) const;
	
	/** \brief Log error unknown tag. */
	void ErrorUnknownTag(const decXmlElementTag &root, const decXmlElementTag &tag);
	
	/** \brief Log error unknown value. */
	void ErrorUnknownValue(const decXmlElementTag &tag, const char *value);
	
	/** \brief Log error missing tag. */
	void ErrorMissingTag(const decXmlElementTag &root, const char *tagName);
	/*@}*/
};

#endif
