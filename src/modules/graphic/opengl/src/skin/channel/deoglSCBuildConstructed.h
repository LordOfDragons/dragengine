/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSCBUILDCONSTRUCTED_H_
#define _DEOGLSCBUILDCONSTRUCTED_H_

#include "../../deoglGL.h"

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>
#include <dragengine/common/math/decMath.h>

class deoglSkinChannel;
class deoglPixelBuffer;

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
		
		deoglPixelBuffer *maskBuffer;
		
		sTarget();
		~sTarget();
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
	void pDrawImage( const sContext &context, const deImage &image,
		const decVector2 &tcScale, const decVector2 &tcOffset );
	void pWritePixel( const sContext &context, int offset, const decColor &color );
	void pWritePixelSet( int offset, const decColor &color );
	void pWritePixelBlend( int offset, const decColor &color, float alpha );
};

#endif

