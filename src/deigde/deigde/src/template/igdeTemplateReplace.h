/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	igdeTemplateReplace( const igdeTemplateReplace &file );
	
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
	void SetToken( const char *token );
	
	/** \brief Value to replace token with. */
	inline eValues GetValue() const{ return pValue; }
	
	/** \brief Set value to replace token with. */
	void SetValue( eValues value );
	
	/** \brief Format of replacement value. */
	inline eFormats GetFormat() const{ return pFormat; }
	
	/** \brief Set format of replacement value. */
	void SetFormat( eFormats format );
	/*@}*/
};

#endif
