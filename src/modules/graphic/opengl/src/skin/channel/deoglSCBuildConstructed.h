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

#ifndef _DEOGLSCBUILDCONSTRUCTED_H_
#define _DEOGLSCBUILDCONSTRUCTED_H_

#include "../../deoglGL.h"
#include "../../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>
#include <dragengine/common/math/decMath.h>

class deoglSkinChannel;

class deImage;
class deSkinPropertyConstructed;



/**
 * Build channel from constructed property.
 */
class deoglSCBuildConstructed : public deSkinPropertyNodeVisitor{
private:
	struct sTarget;
	struct sContext;
	
	/** Target. */
	struct sTarget{
		int targetRed;
		int targetGreen;
		int targetBlue;
		int targetAlpha;
		
		int pixBufComponentCount;
		GLfloat *pixBufDataFloat;
		GLubyte *pixBufDataByte;
		int pixBufWidth;
		int pixBufHeight;
		int pixBufDepth;
		
		bool tileX;
		bool tileY;
		decPoint3 textureSize;
		
		deoglPixelBuffer::Ref maskBuffer;
		
		sTarget();
	};
	
	/** Context. */
	struct sContext{
		sContext *parent;
		sContext *child;
		sContext *root;
		sTarget *mask;
		decPoint clamp;
		decTexMatrix2 transformInverse;
		decTexMatrix2 transformScreen;
		decColorMatrix transformColor;
		decPoint3 localClipFrom;
		decPoint3 localClipTo;
		decPoint3 clipFrom;
		decPoint3 clipTo;
		float transparency;
		float gamma;
		
		sContext();
		sContext( const deSkinPropertyNode &node, sContext *pparent );
		decColor applyGamma( const decColor &color ) const;
	};
	
	deoglSkinChannel &pChannel;
	sContext *pContext;
	sTarget *pTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglSCBuildConstructed( deoglSkinChannel &channel );
	
	/** Clean up visitor. */
	virtual ~deoglSCBuildConstructed();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Build from constructed property. */
	bool BuildFromProperty( const deSkinPropertyConstructed &property, int targetRed,
		int targetGreen, int targetBlue, int targetAlpha );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node );
	
	/** Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node );
	
	/** Visit shape node. */
	virtual void VisitShape( deSkinPropertyNodeShape &node );
	
	/** Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node );
	/*@}*/
	
	
	
private:
	bool pInitPixelBuffer( sTarget &target );
	void pDrawMaskIfPresent( const deSkinPropertyNode &node, sTarget &target );
	void pFillBackground( const decColor &color );
	void pTileNode( const decPoint &offset, const decTexMatrix2 &transformScreen,
		const decTexMatrix2 &transformInverse, deSkinPropertyNode &childNode );
	void pDrawImage(const sContext &context, const deImage &image,
		const decVector2 &tcScale, const decVector2 &tcOffset, int layer);
	void pWritePixel( const sContext &context, int offset, const decColor &color );
	void pWritePixelSet( int offset, const decColor &color );
	void pWritePixelBlend( int offset, const decColor &color, float alpha );
};

#endif

