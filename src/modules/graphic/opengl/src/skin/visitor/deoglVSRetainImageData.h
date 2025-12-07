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

#ifndef _DEOGLVSRETAINIMAGEDATA_H_
#define _DEOGLVSRETAINIMAGEDATA_H_

#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/resources/skin/property/deSkinPropertyVisitor.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>

class deSkin;
class deSkinProperty;
class deImage;



/**
 * Visitor retaining or releasing image data.
 */
class deoglVSRetainImageData : public deSkinPropertyVisitor, deSkinPropertyNodeVisitor {
public:
	decPointerList pImages;
	bool pForceLock;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglVSRetainImageData();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Retain images in property.
	 * \param[in] forceLock Force locking even if not necessary. Required to deal with
	 *                      combined images until they are removed
	 */
	void RetainPropertyImages(deSkinProperty &property, bool forceLock);
	
	/** Release all retained images. */
	void ReleaseImages();
	
	/** Process image. */
	void ProcessImage(deImage &image, bool forceLock);
	/*@}*/
	
	
	
	/** \name Visiting Property */
	/*@{*/
	/** Visit property. */
	virtual void VisitProperty(deSkinProperty &property);
	
	/** Visit single value property. */
	virtual void VisitValue(deSkinPropertyValue &property);
	
	/** Visit uniform color property. */
	virtual void VisitColor(deSkinPropertyColor &property);
	
	/** Visit image property. */
	virtual void VisitImage(deSkinPropertyImage &property);
	
	/** Visit video property. */
	virtual void VisitVideo(deSkinPropertyVideo &property);
	
	/** Visit complex property. */
	virtual void VisitConstructed(deSkinPropertyConstructed &property);
	/*@}*/
	
	
	
	/** \name Visiting Node */
	/*@{*/
	/** Visit node. */
	virtual void VisitNode(deSkinPropertyNode &node);
	
	/** Visit group node. */
	virtual void VisitGroup(deSkinPropertyNodeGroup &node);
	
	/** Visit image node. */
	virtual void VisitImage(deSkinPropertyNodeImage &node);
	
	/** Visit shape node. */
	virtual void VisitShape(deSkinPropertyNodeShape &node);
	
	/** Visit text node. */
	virtual void VisitText(deSkinPropertyNodeText &node);
	/*@}*/
};

#endif
 
