/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINSTATECNTEXT_H_
#define _DEOGLSKINSTATECNTEXT_H_

#include "deoglSkinStateConstructedNode.h"
#include "../../../font/deoglRFont.h"

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>


/**
 * Skin state constructed node.
 */
class deoglSkinStateCNText : public deoglSkinStateConstructedNode{
public:
	typedef deTObjectReference<deoglSkinStateCNText> Ref;
	
	
	
private:
	deoglRFont::Ref pFont;
	float pFontSize;
	const decString pText;
	decColor pColor;
	
	int pTextMapped[ deSkinPropertyNodeText::TextMappedCount ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed node. */
	deoglSkinStateCNText( deSkinPropertyNodeText &node );
	
	/** Create skin state constructed node. */
	deoglSkinStateCNText( const deoglSkinStateCNText &node );
	
protected:
	/** Clean up skin state mapped. */
	virtual ~deoglSkinStateCNText() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Font. */
	inline const deoglRFont::Ref &GetFont() const{ return pFont; }
	
	/** Font size. */
	inline float GetFontSize() const{ return pFontSize; }
	
	/** Text. */
	inline const decString &GetText() const{ return pText; }
	
	/** Color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Mapped for type. */
	int GetTextMappedFor( deSkinPropertyNodeText::eTextMapped type ) const;
	
	
	
	/**
	 * Update.
	 * \warning Called from main thread.
	 */
	virtual void Update( deoglSkinState &state ) override;
	
	/** Render. */
	virtual void Render( deoglSkinState &state, const deoglRenderCanvasContext &context ) override;
	
	/** Create copy. */
	virtual deoglSkinStateConstructedNode::Ref Copy() const override;
	/*@}*/
};

#endif
