/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEFONTSIZE_H_
#define _DEFONTSIZE_H_

#include "deFontGlyph.h"
#include "../image/deImage.h"
#include "../../deObject.h"
#include "../../parallel/deParallelTask.h"


/**
 * \brief Font with specific size for use by text output.
 * \version 1.29
 */
class DE_DLL_EXPORT deFontSize : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deFontSize> Ref;
	
	
private:
	deFontGlyph pUndefinedGlyph, *pGlyphs;
	const int pLineHeight;
	int pGlyphCount, pFontWidth, pBaseLine;
	decString pImagePath;
	deImage::Ref pImage;
	deParallelTask::Ref pTaskLoad;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font with specific size. */
	deFontSize(int lineHeight, int glyphCount);
	
protected:
	/**
	 * \brief Clean up font with specific size.
	 */
	~deFontSize() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height of line of text in pixels. */
	inline int GetLineHeight() const{return pLineHeight;}
	
	/** \brief Maximum width of all glyphs in pixels. */
	inline int GetFontWidth() const{return pFontWidth;}
	
	/** \brief Base line position in pixels from top. */
	inline int GetBaseLine() const{return pBaseLine;}
	
	/** \brief Set base line position in pixels from top. */
	void SetBaseLine(int baseLine);
	
	
	/** \brief Undefined glyph. */
	inline deFontGlyph &GetUndefinedGlyph(){return pUndefinedGlyph;}
	inline const deFontGlyph &GetUndefinedGlyph() const{return pUndefinedGlyph;}
	
	/** \brief Number of glyphs. */
	inline int GetGlyphCount() const{return pGlyphCount;}
	
	/** \brief Glyph at index. */
	deFontGlyph &GetGlyphAt(int index);
	const deFontGlyph &GetGlyphAt(int index) const;
	
	/** \brief Update glyphs. */
	void UpdateGlyphs();
	
	
	/** \brief Path to image resource or empty string is not set. */
	inline const decString &GetImagePath() const{return pImagePath;}
	
	/** \brief Set path to image if existing. */
	void SetImagePath(const char *path);
	
	/** \brief Image or nullptr. */
	inline const deImage::Ref &GetImage() const{return pImage;}
	
	/** \brief Set image or nullptr. */
	void SetImage(deImage *image);
	
	
	/** \brief Load task or nullptr if finished. */
	inline deParallelTask *GetTaskLoad() const{return pTaskLoad;}
	
	/** \brief Set load task or nullptr if finished. */
	void SetTaskLoad(deParallelTask *task);
	/*@}*/
};

#endif
