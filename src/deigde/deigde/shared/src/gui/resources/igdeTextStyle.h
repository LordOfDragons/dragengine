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

#ifndef _IGDETEXTSTYLE_H_
#define _IGDETEXTSTYLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>



/**
 * \brief IGDE UI Text Style.
 */
class DE_DLL_EXPORT igdeTextStyle : public deObject{
private:
	decString pName;
	decColor pColor;
	decColor pBgColor;
	bool pHasColor;
	bool pHasBgColor;
	bool pBold;
	bool pItalic;
	bool pUnderline;
	bool pStrikeThrough;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create text style. */
	igdeTextStyle( const char *name );
	
	/** \brief Create copy of text style. */
	igdeTextStyle( const char *name, const igdeTextStyle &textStyle );
	
	
	
protected:
	/**
	 * \brief Clean up text style.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeTextStyle();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Text color is set. */
	inline bool GetHasColor() const{ return pHasColor; }
	
	/** \brief Set text color. */
	void SetColor( const decColor &color );
	
	/** \brief Clear text color. */
	void ClearColor();
	
	
	
	/** \brief Background color. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Background color is set. */
	inline bool GetHasBgColor() const{ return pHasBgColor; }
	
	/** \brief Set background color. */
	void SetBgColor( const decColor &color );
	
	/** \brief Clear background color. */
	void ClearBgColor();
	
	
	
	/** \brief Bold. */
	inline bool GetBold() const{ return pBold; }
	
	/** \brief Set bold. */
	void SetBold( bool bold );
	
	/** \brief Italic. */
	inline bool GetItalic() const{ return pItalic; }
	
	/** \brief Set italic. */
	void SetItalic( bool italic );
	
	/** \brief Underline. */
	inline bool GetUnderline() const{ return pUnderline; }
	
	/** \brief Set underline. */
	void SetUnderline( bool underline );
	
	/** \brief Strike through. */
	inline bool GetStrikeThrough() const{ return pStrikeThrough; }
	
	/** \brief Set strike through. */
	void SetStrikeThrough( bool strikeThrough );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set text style. */
	igdeTextStyle &operator=( const igdeTextStyle &textStyle );
	/*@}*/
};

#endif
