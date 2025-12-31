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

// include only once
#ifndef _FEFONTNOTIFIER_H_
#define _FEFONTNOTIFIER_H_

// includes
#include <dragengine/deObject.h>

// predefinitions
class feFont;
class feFontImage;
class feFontGlyph;



/**
 * @brief Font Notifier.
 * Notifier for changes in a font.
 */
class feFontNotifier : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<feFontNotifier> Ref;


	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new font notifier. */
	feFontNotifier();
	/** Cleans up the font notifier. */
protected:
	virtual ~feFontNotifier();
public:
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Element or work mode changed. */
	virtual void ModeChanged(feFont *font);
	/** Changed or saved state changed. */
	virtual void StateChanged(feFont *font);
	/** Undos changed. */
	virtual void UndoChanged(feFont *font);
	
	/** Font parameters changed. */
	virtual void FontChanged(feFont *font);
	/** An image changed. */
	virtual void ImageChanged(feFont *font, feFontImage *image);
	/** Glyphs have been added or removed. */
	virtual void GlyphStructureChanged (feFont *font);
	/** A glyph changed. */
	virtual void GlyphChanged(feFont *font, feFontGlyph *glyph);
	/** Glyph selection changed. */
	virtual void GlyphSelectionChanged(feFont *font);
	/** Active glyph changed. */
	virtual void ActiveGlyphChanged(feFont *font);
	/*@}*/
};

// end of include only once
#endif
