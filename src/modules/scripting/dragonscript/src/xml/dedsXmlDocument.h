/* 
 * Drag[en]gine DragonScript Script Module
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
	dedsXmlDocument( const char *filename );
	
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
	void SetFilename( const char *filename );
	
	/** \brief Parse failed. */
	inline bool GetParseFailed() const{ return pParseFailed; }
	
	/** \brief Set parse failed. */
	void SetParseFailed( bool parseFailed );
	
	/** \brief Parse log. */
	inline const decString &GetParseLog() const{ return pParseLog; }
	
	/** \brief Set parse log. */
	void SetParseLog( const char *text );
};

#endif
