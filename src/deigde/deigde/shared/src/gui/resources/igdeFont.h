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

#ifndef _IGDEFONT_H_
#define _IGDEFONT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/font/deFont.h>

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
	bool pBold, pItalic, pUnderline, pStrikeThrough;
	deFont::Ref pEngineFont;
	
	
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
