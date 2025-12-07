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

#ifndef _FEFONT_H_
#define _FEFONT_H_

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectSet.h>

#include "glyph/feFontGlyphList.h"
#include "glyph/feFontGlyphSelection.h"
#include <dragengine/src/resources/font/deFont.h>
#include "feFontImage.h"

class igdeGameDefinition;
class igdeEnvironment;
class feFontNotifier;
class feUndoSystem;
class feFontGlyph;

class deEngine;
class deLogger;



/**
 */
class feFont : public igdeEditableEntity{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<feFont> Ref;
	
	
	/** Element modes. */
	enum eElementModes{
		/** Bone mode. */
		eemBone,
		/** Shape mode. */
		eemShape,
		/** Constraint mode. */
		eemConstraint,
		/** Push mode. */
		eemPush,
	};
	
	/** Work modes. */
	enum eWorkModes{
		/** Select elements mode. */
		ewmSelect,
		/** Move elements mode. */
		ewmMove,
		/** Scale elements mode. */
		ewmScale,
		/** Rotate elements mode. */
		ewmRotate,
		/** Place 3D cursor mode. */
		ewm3DCursor,
	};
	
	
	
private:
	deFont::Ref pEngFont;
	
	feFontImage::Ref pFontImage;
	int pLineHeight, pBaseLine;
	bool pColorFont;
	feFontGlyphList pGlyphs;
	feFontGlyphSelection *pGlyphSelection;
	
	decString pBasePath;
	decString pFilename;
	bool pChanged;
	bool pSaved;
	
	int pElementMode;
	int pWorkMode;
	
	bool pDirtyFont;
	
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creat font. */
	feFont(igdeEnvironment *environment);
	
	/** \brief Clean up font. */
	virtual ~feFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine font. */
	inline const deFont::Ref &GetEngineFont() const{ return pEngFont; }
	
	/** \brief Height of a line of text. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** \brief Set height of a line of text. */
	void SetLineHeight(int lineHeight);
	
	/** \brief Font is a color font. */
	inline bool GetColorFont() const{ return pColorFont; }
	
	/** \brief Set if font is a color font. */
	void SetColorFont(bool colorFont);
	
	/** \brief Base line. */
	inline int GetBaseLine() const{ return pBaseLine; }
	
	/** \brief Set base line. */
	void SetBaseLine(int baseLine);
	
	/** \brief Font image. */
	inline const feFontImage::Ref &GetFontImage() const{ return pFontImage; }
	
	
	
	/** \brief Element mode indicating which type of element to edit. */
	inline int GetElementMode() const{ return pElementMode; }
	
	/** \brief Set element mode indicating which type of element to edit. */
	void SetElementMode(int mode);
	
	/** \brief Work mode. */
	inline int GetWorkMode() const{ return pWorkMode; }
	
	/** \brief Set work mode. */
	void SetWorkMode(int mode);
	
	
	
	/** \brief Dispose of all resources. */
	void Dispose();
	
	/** \brief Update font. */
	void Update(float elapsed);
	
	/** \brief Reset font. */
	void Reset();
	
	/** \brief Invalidate font. */
	void Invalidate();
	
	/** \brief Rebuild font. */
	void Rebuild();
	/*@}*/
	
	
	
	/** \name Glyphs */
	/*@{*/
	/** \brief Number of glyphs. */
	int GetGlyphCount() const;
	
	/** \brief Glyph at the given position. */
	feFontGlyph *GetGlyphAt(int index) const;
	
	/** \brief Glyph with the given code or NULL if not found. */
	feFontGlyph *GetGlyphWithCode(int code) const;
	
	/** \brief Index of the given glyph or -1 if not found. */
	int IndexOfGlyph(feFontGlyph *glyph) const;
	
	/** \brief Index of the glyph with the given code or -1 if not found. */
	int IndexOfGlyphWithCode(int code) const;
	
	/** \brief Glyph exists. */
	bool HasGlyph(feFontGlyph *glyph) const;
	
	/** \brief Glyph with the given code exists. */
	bool HasGlyphWithCode(int code) const;
	
	/** \brief Add glyph. */
	void AddGlyph(feFontGlyph *glyph);
	
	/** \brief Remove glyph. */
	void RemoveGlyph(feFontGlyph *glyph);
	
	/** \brief Remove glyph with the given code if existing. */
	void RemoveGlyphWithCode(int code);
	
	/** \brief Remove all glyphs. */
	void RemoveAllGlyphs();
	
	/** \brief Glyph selection. */
	inline feFontGlyphSelection &GetGlyphSelection(){ return *pGlyphSelection; }
	inline const feFontGlyphSelection &GetGlyphSelection() const{ return *pGlyphSelection; }
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add notifier. */
	void AddNotifier(feFontNotifier *notifier);
	
	/** \brief Remove notifier. */
	void RemoveNotifier(feFontNotifier *notifier);
	
	
	
	/** \brief Notify listeners the changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notify all that the element or work mode changed. */
	void NotifyModeChanged();
	
	/** \brief Notify all that an image changed. */
    void NotifyImageChanged(feFontImage *image);
	
	/** \brief Notify all that a font parameter changed. */
	void NotifyFontChanged();
	
	/** \brief Notify all that glyphs have been added or removed. */
	void NotifyGlyphStructureChanged();
	
	/** \brief Notify all that a glyph changed. */
	void NotifyGlyphChanged(feFontGlyph *glyph);
	
	/** Glyph selection changed. */
	void NotifyGlyphSelectionChanged();
	
	/** Active glyph changed. */
	void NotifyActiveGlyphChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
