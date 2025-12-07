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

#ifndef _DEFONT_H_
#define _DEFONT_H_

#include "deFontSize.h"
#include "deFontGlyph.h"
#include "../deFileResource.h"
#include "../image/deImage.h"
#include "../../common/collection/decObjectList.h"
#include "../../common/collection/decIntList.h"
#include "../../common/math/decMath.h"
#include "../../threading/deMutex.h"

class deFontManager;
class deBaseGraphicFont;
class decUnicodeString;


/**
 * \brief Font for use by text output.
 * 
 * Fonts are image based and can be colored or not colored in which case they can be
 * colorized. Colored fonts can not be colorized.
 */
class DE_DLL_EXPORT deFont : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deFont> Ref;
	
	
private:
	deFontGlyph pUndefinedGlyph, *pGlyphs;
	int pGlyphCount;
	unsigned short *pGlyphGroups;
	int pGlyphGroupCount;
	unsigned short *pGlyphMap;
	int pGlyphMapCount;
	int pFontWidth, pLineHeight, pBaseLine;
	bool pColorFont, pScalable;
	
	decString pImagePath;
	deImage::Ref pImage;
	
	decIntList pFixedSizes;
	decObjectList pSizes;
	deMutex pMutex;
	
	deBaseGraphicFont *pPeerGraphic;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font resource. */
	deFont(deFontManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime);
	
protected:
	/**
	 * \brief Clean up font.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	~deFont() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Height of line of text in pixels. */
	inline int GetLineHeight() const{return pLineHeight;}
	
	/** \brief Set height of line of text in pixels. */
	void SetLineHeight(int lineHeight);
	
	/** \brief Font is colored and can not be tinted. */
	inline bool GetIsColorFont() const{return pColorFont;}
	
	/** \brief Set if font is colored and can not be tinted. */
	void SetIsColorFont(bool colorFont);
	
	/** \brief Maximum width of all glyphs in pixels. */
	inline int GetFontWidth() const{return pFontWidth;}
	
	/** \brief Font is scalable. */
	inline bool GetScalable() const{return pScalable;}
	
	/** \brief Set if font is scalable. */
	void SetScalable(bool scalable);
	
	/** \brief Base line position in pixels from top. */
	inline int GetBaseLine() const{return pBaseLine;}
	
	/** \brief Set base line position in pixels from top. */
	void SetBaseLine(int baseLine);
	
	
	/** \brief Undefined glyph. */
	inline deFontGlyph &GetUndefinedGlyph(){return pUndefinedGlyph;}
	inline const deFontGlyph &GetUndefinedGlyph() const{return pUndefinedGlyph;}
	
	/** \brief Number of glyphs. */
	inline int GetGlyphCount() const{return pGlyphCount;}
	
	/** \brief Sets number of glyphs resetting all to default values. */
	void SetGlyphCount(int count);
	
	/** \brief Glyph for unicode is defined. */
	bool HasGlyph(int unicode) const;
	
	/** \brief Glyph at index. */
	deFontGlyph &GetGlyphAt(int index);
	const deFontGlyph &GetGlyphAt(int index) const;
	
	/**
	 * \brief Glyph for Unicode.
	 * 
	 * If glyph is not defined the undefined glyph is returned.
	 */
	const deFontGlyph &GetGlyph(int unicode) const;
	
	/**
	 * \brief Glyph for Unicode.
	 * 
	 * If glyph is not defined the undefined glyph is returned. Size can be nullptr in which
	 * case this function behaves the same as GetGlyph(int).
	 */
	const deFontGlyph &GetGlyph(int unicode, const deFontSize *size) const;
	
	/**
	 * \brief Glyph index for Unicode.
	 * 
	 * If glyph is not defined -1 is returned.
	 */
	int GetGlyphIndex(int unicode) const;
	
	
	/** \brief Update glyphs. */
	void UpdateGlyphs();
	
	/** \brief Verify settings of font are all valid. */
	bool Verify();
	
	
	/** \brief Path to image resource or empty string is not set. */
	inline const decString &GetImagePath() const{return pImagePath;}
	
	/** \brief Set path to image if existing. */
	void SetImagePath(const char *path);
	
	/** \brief Image or nullptr. */
	inline const deImage::Ref &GetImage() const{return pImage;}
	
	/** \brief Set image or nullptr. */
	void SetImage(deImage *image);
	
	
	/** \brief List of fixed font sizes (line heights). */
	inline decIntList &GetFixedSizes(){return pFixedSizes;}
	inline const decIntList &GetFixedSizes() const{return pFixedSizes;}
	
	/**
	 * \brief Fixed size best matching line height.
	 * 
	 * If font has no fixed sizes 0 is returned.
	 */
	int BestFixedSizeFor(int lineHeight) const;
	
	
	/**
	 * \brief Font size count.
	 * \note Mutex protected access to sizes.
	 */
	int GetSizeCount();
	
	/**
	 * \brief Font size.
	 * \note Mutex protected access to sizes.
	 */
	deFontSize &GetSizeAt(int index);
	
	/**
	 * \brief Font size with line height or null.
	 * \warning Make sure to check if font size has pending load task. While loading you must not
	 *          access any data except GetTaskLoad().
	 * \note Mutex protected access to sizes.
	 */
	deFontSize *GetSizeWith(int lineHeight);
	
	/**
	 * \brief Add font size if absent.
	 * 
	 * Returns the added font size if absent or the already existing size.
	 * 
	 * \note Mutex protected access to sizes.
	 * \warning For use by deFontManager only.
	 */
	deFontSize *AddSize(int lineHeight, const deFontSize::Ref &size);
	
	
	/**
	 * \brief Prepare font size if required.
	 * 
	 * Call this during loading time to allow loading font size ahead of use. It is safe to not
	 * call this function but it helps to improve loading times. If a matching font size is found
	 * it is returned otherwise nullptr.
	 * 
	 * Also call this before using the font for rendering. In this case wait for the load task
	 * to finish if present.
	 * 
	 * \note Mutex protected access to sizes.
	 */
	deFontSize *PrepareSize(int lineHeight);
	
	/**
	 * \brief Ensure font size is prepared.
	 * 
	 * Call this before using the font for rendering. If a matching font size is found it is
	 * returned otherwise nullptr.
	 * 
	 * Similar to PrepareSize() but waits for the load task to finish if present.
	 * 
	 * \note Mutex protected access to sizes.
	 */
	deFontSize *EnsureSizePrepared(int lineHeight);
	
	
	/** \brief Measure size of text. */
	decPoint TextSize(const decUnicodeString &text, const deFontSize *size = nullptr) const;
	
	/** \brief Measure size of text. */
	decPoint TextSize(const char *text, const deFontSize *size = nullptr) const;
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer. */
	inline deBaseGraphicFont *GetPeerGraphic() const{return pPeerGraphic;}
	
	/** \brief Set graphic system peer. */
	void SetPeerGraphic(deBaseGraphicFont *peer);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pFreeGlyphMap();
	void pCreateGlyphMap();
	deFontSize *pGetSizeWith(int lineHeight);
};

#endif
