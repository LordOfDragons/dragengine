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
