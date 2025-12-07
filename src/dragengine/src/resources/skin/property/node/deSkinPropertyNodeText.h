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

#ifndef _DESKINPROPERTYNODETEXT_H_
#define _DESKINPROPERTYNODETEXT_H_

#include "deSkinPropertyNode.h"
#include "../../../font/deFont.h"
#include "../../../../common/string/decString.h"


/**
 * \brief Skin property text node for constructed texture property.
 */
class DE_DLL_EXPORT deSkinPropertyNodeText : public deSkinPropertyNode{
public:
	/** \brief Mapped. */
	enum eTextMapped{
		etmFontSize, //!< Font size
		etmColorRed, //!< Color red component
		etmColorGreen, //!< Color green component
		etmColorBlue //!< Color blue component
	};
	
	static const int TextMappedCount = etmColorBlue + 1;
	
	
	
private:
	decString pPath;
	deFont::Ref pFont;
	float pFontSize;
	decString pText;
	decColor pColor;
	
	int pTextMapped[TextMappedCount];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property node. */
	deSkinPropertyNodeText();
	
	/** \brief Clean up constructed property node. */
	virtual ~deSkinPropertyNodeText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set font path or empty path if not set. */
	void SetPath(const char *path);
	
	/** \brief Font or nullptr if not set. */
	inline const deFont::Ref &GetFont() const{ return pFont; }
	
	/** \brief Set font or nullptr to unset. */
	void SetFont(deFont *font);
	
	/** \brief Font size in canvas units. */
	inline float GetFontSize() const{ return pFontSize; }
	
	/**
	 * \brief Set font size in canvas units.
	 * 
	 * Size is clamped to 0 or larger.
	 */
	void SetFontSize(float size);
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const char *text);
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor(const decColor &color);
	
	
	
	/** \brief Index of mapped value or -1 to use static value. */
	int GetTextMappedFor(eTextMapped mapped) const;
	
	/** \brief Set index of mapped value or -1 to use static value. */
	void SetTextMappedFor(eTextMapped mapped, int index);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits node. */
	virtual void Visit(deSkinPropertyNodeVisitor &visitor);
	/*@}*/
};

#endif
