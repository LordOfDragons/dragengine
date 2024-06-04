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
