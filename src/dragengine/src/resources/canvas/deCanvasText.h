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

#ifndef _DECANVASTEXT_H_
#define _DECANVASTEXT_H_

#include "deCanvas.h"
#include "../font/deFont.h"
#include "../../common/string/decString.h"


/**
 * \brief Canvas rendering a text.
 */
class DE_DLL_EXPORT deCanvasText : public deCanvas{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCanvasText> Ref;
	
	
private:
	deFont::Ref pFont;
	float pFontSize;
	decString pText;
	decColor pColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	deCanvasText(deCanvasManager *manager);
	
protected:
	/**
	 * \brief Clean up canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCanvasText();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Font or NULL if not set. */
	inline deFont *GetFont() const{ return pFont; }
	
	/** \brief Set font or NULL to unset. */
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
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void Visit(deCanvasVisitor &visitor);
	/*@}*/
};

#endif
