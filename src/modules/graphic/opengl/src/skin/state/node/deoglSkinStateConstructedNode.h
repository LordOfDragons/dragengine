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
