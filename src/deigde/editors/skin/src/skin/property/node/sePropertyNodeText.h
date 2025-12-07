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

#ifndef _SEPROPERTYNODETEXT_H_
#define _SEPROPERTYNODETEXT_H_

#include "sePropertyNode.h"

#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontSize.h>


/**
 * \brief Texture property text node.
 */
class sePropertyNodeText : public sePropertyNode{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<sePropertyNodeText> Ref;
	
	
	enum eTextMapped{
		etmFontSize = MappedCount, //<!Font size
		etmColorRed, //<!Color red component
		etmColorGreen, //<!Color green component
		etmColorBlue //<!Color blue component
	};
	
	static const int TextMappedCount = etmColorBlue + 1;
	
	
	
private:
	decString pPath;
	deFont::Ref pFont;
	deFontSize::Ref pFontSize;
	float pTextSize;
	decString pText;
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	sePropertyNodeText(deEngine &engine);
	
	/** \brief Create copy of node. */
	sePropertyNodeText(const sePropertyNodeText &node);
	
	/** \brief Clean up node. */
	virtual ~sePropertyNodeText();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font path or empty string if not set. */
	inline const decString &GetPath() const{ return pPath; }
	
	/** \brief Set font path or empty path if not set. */
	void SetPath(const char *path);
	
	/** \brief Font or nullptr. */
	inline const deFont::Ref &GetFont() const{ return pFont; }
	
	/** \brief Font size or nullptr. */
	inline const deFontSize::Ref &GetFontSize() const{ return pFontSize; }
	
	/** \brief Text size in canvas units. */
	inline float GetTextSize() const{ return pTextSize; }
	
	/**
	 * \brief Set text size in canvas units.
	 * \details Size is clamped to 0 or larger.
	 */
	void SetTextSize(float size);
	
	/** \brief Update font. */
	void UpdateFont();
	
	/** \brief Update font size. */
	void UpdateFontSize();
	
	/** \brief Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** \brief Set text. */
	void SetText(const char *text);
	
	/** \brief Text color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** \brief Set text color. */
	void SetColor(const decColor &color);
	
	
	
	/** \brief Create copy of node. */
	virtual sePropertyNode *Copy() const;
	
	/** \brief Update resources. */
	virtual void UpdateResources();
	/*@}*/
};

#endif
