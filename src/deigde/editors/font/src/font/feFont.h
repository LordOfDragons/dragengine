/* 
 * Drag[en]gine IGDE Font Editor
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

#ifndef _FEFONT_H_
#define _FEFONT_H_

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectSet.h>

#include "glyph/feFontGlyphList.h"
#include "glyph/feFontGlyphSelection.h"

class igdeGameDefinition;
class igdeEnvironment;
class feFontNotifier;
class feUndoSystem;
class feFontImage;
class feFontGlyph;

class deEngine;
class deFont;
class deLogger;



/**
 */
class feFont : public igdeEditableEntity{
public:
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
	deFont *pEngFont;
	
	feFontImage *pFontImage;
	int pLineHeight;
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
	feFont( igdeEnvironment *environment );
	
	/** \brief Clean up font. */
	virtual ~feFont();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Engine font. */
	inline deFont *GetEngineFont() const{ return pEngFont; }
	
	/** \brief Height of a line of text. */
	inline int GetLineHeight() const{ return pLineHeight; }
	
	/** \brief Set height of a line of text. */
	void SetLineHeight( int lineHeight );
	
	/** \brief Font is a color font. */
	inline bool GetColorFont() const{ return pColorFont; }
	
	/** \brief Set if font is a color font. */
	void SetColorFont( bool colorFont );
	
	/** \brief Font image. */
	inline feFontImage *GetFontImage() const{ return pFontImage; }
	
	
	
	/** \brief Element mode indicating which type of element to edit. */
	inline int GetElementMode() const{ return pElementMode; }
	
	/** \brief Set element mode indicating which type of element to edit. */
	void SetElementMode( int mode );
	
	/** \brief Work mode. */
	inline int GetWorkMode() const{ return pWorkMode; }
	
	/** \brief Set work mode. */
	void SetWorkMode( int mode );
	
	
	
	/** \brief Dispose of all resources. */
	void Dispose();
	
	/** \brief Update font. */
	void Update( float elapsed );
	
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
	feFontGlyph *GetGlyphAt( int index ) const;
	
	/** \brief Glyph with the given code or NULL if not found. */
	feFontGlyph *GetGlyphWithCode( int code ) const;
	
	/** \brief Index of the given glyph or -1 if not found. */
	int IndexOfGlyph( feFontGlyph *glyph ) const;
	
	/** \brief Index of the glyph with the given code or -1 if not found. */
	int IndexOfGlyphWithCode( int code ) const;
	
	/** \brief Glyph exists. */
	bool HasGlyph( feFontGlyph *glyph ) const;
	
	/** \brief Glyph with the given code exists. */
	bool HasGlyphWithCode( int code ) const;
	
	/** \brief Add glyph. */
	void AddGlyph( feFontGlyph *glyph );
	
	/** \brief Remove glyph. */
	void RemoveGlyph( feFontGlyph *glyph );
	
	/** \brief Remove glyph with the given code if existing. */
	void RemoveGlyphWithCode( int code );
	
	/** \brief Remove all glyphs. */
	void RemoveAllGlyphs();
	
	/** \brief Glyph selection. */
	inline feFontGlyphSelection &GetGlyphSelection(){ return *pGlyphSelection; }
	inline const feFontGlyphSelection &GetGlyphSelection() const{ return *pGlyphSelection; }
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add notifier. */
	void AddNotifier( feFontNotifier *notifier );
	
	/** \brief Remove notifier. */
	void RemoveNotifier( feFontNotifier *notifier );
	
	
	
	/** \brief Notify listeners the changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notify all that the element or work mode changed. */
	void NotifyModeChanged();
	
	/** \brief Notify all that an image changed. */
    void NotifyImageChanged( feFontImage *image );
	
	/** \brief Notify all that a font parameter changed. */
	void NotifyFontChanged();
	
	/** \brief Notify all that glyphs have been added or removed. */
	void NotifyGlyphStructureChanged();
	
	/** \brief Notify all that a glyph changed. */
	void NotifyGlyphChanged( feFontGlyph *glyph );
	
	/** Glyph selection changed. */
	void NotifyGlyphSelectionChanged();
	
	/** Active glyph changed. */
	void NotifyActiveGlyphChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
