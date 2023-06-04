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

#ifndef _IGDEFONT_H_
#define _IGDEFONT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/font/deFontReference.h>

class igdeEnvironment;



/**
 * \brief IGDE UI Font.
 * 
 * Fonts are immutable.
 */
class DE_DLL_EXPORT igdeFont : public deObject{
public:
	/** \brief Font configuration. */
	struct DE_DLL_EXPORT sConfiguration{
		decString name; //< Name of font
		float size; //< Font size in points
		bool bold; //< Bold font
		bool italic; //< Italic font
		bool underline; //< Underline font
		bool strikeThrough; //< Strike through font
		
		sConfiguration();
		sConfiguration( const sConfiguration &config );
		
		sConfiguration &operator=( const sConfiguration &config );
		bool operator==( const sConfiguration &config ) const;
	};
	
	
	
private:
	igdeEnvironment &pEnvironment;
	void *pNativeFont;
	decString pName;
	float pSize;
	bool pBold;
	bool pItalic;
	bool pUnderline;
	bool pStrikeThrough;
	deFontReference pEngineFont;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font. */
	igdeFont( igdeEnvironment &environment, const sConfiguration &config );
	
	
	
protected:
	/**
	 * \brief Clean up font.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeFont();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Environment. */
	inline igdeEnvironment &GetEnvironment() const{ return pEnvironment; }
	
	/** \brief Font name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Font size in points. */
	inline float GetSize() const{ return pSize; }
	
	/** \brief Bold font. */
	inline bool GetBold() const{ return pBold; }
	
	/** \brief Italic font. */
	inline bool GetItalic() const{ return pItalic; }
	
	/** \brief Underline font. */
	inline bool GetUnderline() const{ return pUnderline; }
	
	/** \brief Strike through font. */
	inline bool GetStrikeThrough() const{ return pStrikeThrough; }
	
	/** \brief Font configuration. */
	void GetConfig( sConfiguration &config ) const;
	
	
	
	/** \brief Text size. */
	decPoint TextSize( const char *text ) const;
	
	
	
	/**
	 * \brief Get engine font.
	 * 
	 * Engine font is created the first time it is requested.
	 */
	deFont *GetEngineFont();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Get native font pointer.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void *GetNativeFont() const{ return pNativeFont; }
	/*@}*/
};

#endif
