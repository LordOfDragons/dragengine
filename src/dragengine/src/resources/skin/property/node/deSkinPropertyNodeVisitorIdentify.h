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

#ifndef _DESKINPROPERTYNODEVISITORIDENTIFY_H_
#define _DESKINPROPERTYNODEVISITORIDENTIFY_H_

#include "deSkinPropertyNodeVisitor.h"


/**
 * \brief Skin constructed property node identify visitor.
 */
class DE_DLL_EXPORT deSkinPropertyNodeVisitorIdentify : public deSkinPropertyNodeVisitor{
public:
	/** \brief Node types. */
	enum eNodeTypes{
		/** \brief Unknown node. */
		entUnknown,
		
		/** \brief Group node. */
		entGroup,
		
		/** \brief Image node. */
		entImage,
		
		/** \brief Shape node. */
		entShape,
		
		/** \brief Text node. */
		entText
	};
	
	
	
private:
	deSkinPropertyNode *pNode;
	eNodeTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin constructed property node identify visitor. */
	deSkinPropertyNodeVisitorIdentify();
	
	/** \brief Clean up skin constructed property node identify visitor. */
	~deSkinPropertyNodeVisitorIdentify() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Node type. */
	inline eNodeTypes GetType() const{ return pType; }
	
	
	
	/** \brief Node is of type unknown. */
	inline bool IsUnknown() const{ return pType == entUnknown; }
	
	/** \brief Node is of type group. */
	inline bool IsGroup() const{ return pType == entGroup; }
	
	/** \brief Node is of type image. */
	inline bool IsImage() const{ return pType == entImage; }
	
	/** \brief Node is of type shape. */
	inline bool IsShape() const{ return pType == entShape; }
	
	/** \brief Node is of type text. */
	inline bool IsText() const{ return pType == entText; }
	
	
	
	/** \brief Cast to group node. */
	deSkinPropertyNodeGroup &CastToGroup() const;
	
	/** \brief Cast to image node. */
	deSkinPropertyNodeImage &CastToImage() const;
	
	/** \brief Cast to shape node. */
	deSkinPropertyNodeShape &CastToShape() const;
	
	/** \brief Cast to text node. */
	deSkinPropertyNodeText &CastToText() const;
	
	
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit node. */
	void VisitNode(deSkinPropertyNode &node) override;
	
	/** \brief Visit group node. */
	void VisitGroup(deSkinPropertyNodeGroup &node) override;
	
	/** \brief Visit image node. */
	void VisitImage(deSkinPropertyNodeImage &node) override;
	
	/** \brief Visit shape node. */
	void VisitShape(deSkinPropertyNodeShape &node) override;
	
	/** \brief Visit text node. */
	void VisitText(deSkinPropertyNodeText &node) override;
	/*@}*/
};

#endif
