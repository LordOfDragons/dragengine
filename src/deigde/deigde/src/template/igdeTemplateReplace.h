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

#ifndef _IGDETEMPLATEREPLACE_H_
#define _IGDETEMPLATEREPLACE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief Project template replace.
 */
class igdeTemplateReplace : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTemplateReplace> Ref;


	/** \brief Replacement value. */
	enum eValues{
		/** \brief Absolute project directory path. */
		evProjectPath,
		
		/** \brief Last component of project directory path. */
		evProjectPathDirectory,
		
		/** \brief Absolute project data directory path. */
		evDataPath,
		
		/** \brief Project data directory relative to project root. */
		evDataDirectory,
		
		/** \brief Project name. */
		evProjectName,
		
		/** \brief Project description. */
		evProjectDescription,
		
		/** \brief Game identifier derived from project name. */
		evGameId,
		
		/** \brief Game alias identifier derived from project name. */
		evGameAliasId
	};
	
	/** \brief Format of replacement value. */
	enum eFormats{
		/** \brief Plain text as is. */
		efText,
		
		/**
		 * \brief C-like string literal escaped.
		 * 
		 * Escapes single and double quotes with a backslash.
		 */
		efStringC,
		
		/**
		 * \brief XML Attribute escaped.
		 * 
		 * Escapes values according to XML standard for attributes. Works also for CDATA
		 * since attributes is more strict.
		 */
		efXml,
		
		/**
		 * \brief Multiline XML Attribute escaped.
		 * 
		 * Escapes values according to XML standard for attributes. Works also for CDATA
		 * since attributes is more strict. Outputs &gt;br/&lt; tags instead of '\n'.
		 */
		efXmlMultiline
	};
	
	
	
private:
	decString pToken;
	eValues pValue;
	eFormats pFormat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create template. */
	igdeTemplateReplace();
	
	/** \brief Create template. */
	igdeTemplateReplace(const igdeTemplateReplace &file);
	
protected:
	/** \brief Clean up template. */
	virtual ~igdeTemplateReplace();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Token string to replace. */
	inline const decString &GetToken() const{ return pToken; }
	
	/** \brief Set token string to replace. */
	void SetToken(const char *token);
	
	/** \brief Value to replace token with. */
	inline eValues GetValue() const{ return pValue; }
	
	/** \brief Set value to replace token with. */
	void SetValue(eValues value);
	
	/** \brief Format of replacement value. */
	inline eFormats GetFormat() const{ return pFormat; }
	
	/** \brief Set format of replacement value. */
	void SetFormat(eFormats format);
	/*@}*/
};

#endif
