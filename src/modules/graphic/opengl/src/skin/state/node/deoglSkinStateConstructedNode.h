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

#ifndef _DEOGLSKINSTATECONSTRUCTEDNODE_H_
#define _DEOGLSKINSTATECONSTRUCTEDNODE_H_

#include "../../../target/deoglRenderTarget.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>

class deoglRenderThread;
class deoglSkinState;
class deoglRenderCanvasContext;


/**
 * Skin state constructed node.
 */
class deoglSkinStateConstructedNode : public deObject{
public:
	typedef deTObjectReference<deoglSkinStateConstructedNode> Ref;
	
	enum eType{
		etGroup,
		etImage,
		etShape,
		etText
	};
	
	
	
protected:
	const eType pType;
	
	decPoint3 pPosition;
	decPoint3 pSize;
	float pRotation;
	float pShear;
	float pBrightness;
	float pContrast;
	float pGamma;
	decColor pColorize;
	float pTransparency;
	const deSkinPropertyNode::eCombineModes pCombineMode;
	Ref pMask;
	int pMapped[ deSkinPropertyNode::MappedCount ];
	
	decTexMatrix2 pTransform;
	bool pDirtyTransform;
	
	decColorMatrix pColorTransform;
	bool pDirtyColorTransform;
	
	deoglRenderTarget::Ref pMaskRenderTarget;
	
	
	
protected:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed node. */
	deoglSkinStateConstructedNode( deSkinPropertyNode &node, eType type );
	
	/** Create skin state constructed node. */
	deoglSkinStateConstructedNode( const deoglSkinStateConstructedNode &node );
	
	/** Clean up skin state mapped. */
	virtual ~deoglSkinStateConstructedNode() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Type. */
	inline eType GetType() const{ return pType; }
	
	/** Position. */
	inline const decPoint3 &GetPosition() const{ return pPosition; }
	
	/** Size. */
	inline const decPoint3 &GetSize() const{ return pSize; }
	
	/** Rotation. */
	inline float GetRotation() const{ return pRotation; }
	
	/** Shear. */
	inline float GetShear() const{ return pShear; }
	
	/** Brightness. */
	inline float GetBrightness() const{ return pBrightness; }
	
	/** Contrast. */
	inline float GetContrast() const{ return pContrast; }
	
	/** Gamma. */
	inline float GetGamma() const{ return pGamma; }
	
	/** Colorize. */
	inline const decColor &GetColorize() const{ return pColorize; }
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Mask. */
	inline const Ref &GetMask() const{ return pMask; }
	
	/** Combine mode. */
	inline deSkinPropertyNode::eCombineModes GetCombineMode() const{ return pCombineMode; }
	
	/** Mapped for type. */
	int GetMappedFor( deSkinPropertyNode::eMapped type ) const;
	
	
	
	/** Transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Color transformation matrix. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	
	
	/** Mask render target or nullptr. */
	inline const deoglRenderTarget::Ref &GetMaskRenderTarget() const{ return pMaskRenderTarget; }
	
	
	
	/**
	 * Update.
	 * \warning Called from main thread.
	 */
	virtual void Update( deoglSkinState &state );
	
	/** Prepare for render. */
	virtual void PrepareForRender( deoglSkinState &state );
	
	/** Render. */
	virtual void Render( deoglSkinState &state, const deoglRenderCanvasContext &context );
	
	/** Create copy. */
	virtual Ref Copy() const = 0;
	
	
	
	/** Create node. */
	static Ref CreateNode( deSkinPropertyNode &node );
	/*@}*/
	
	
	
protected:
	void pUpdateTransform();
	void pUpdateColorTransform();
};

#endif
