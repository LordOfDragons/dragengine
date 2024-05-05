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

#ifndef _DEOGLSCCONSTRUCTEDDYNAMIC_H_
#define _DEOGLSCCONSTRUCTEDDYNAMIC_H_

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>


/**
 * Determine if constructed property is dynamic.
 */
class deoglSCConstructedDynamic : public deSkinPropertyNodeVisitor{
private:
	bool pDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglSCConstructedDynamic();
	
	/** Clean up visitor. */
	virtual ~deoglSCConstructedDynamic() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Is dynamic. */
	static bool IsDynamic( deSkinPropertyNode &node );
	
	/** Dynamic. */
	inline bool GetDynamic() const{ return pDynamic; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit node. */
	virtual void VisitNode( deSkinPropertyNode &node ) override;
	
	/** Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node ) override;
	
	/** Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node ) override;
	
	/** Visit shape node. */
	virtual void VisitShape( deSkinPropertyNodeShape &node ) override;
	
	/** Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node ) override;
	/*@}*/
};

#endif

