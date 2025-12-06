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

#ifndef _DEOGLSKINSTATECNGROUP_H_
#define _DEOGLSKINSTATECNGROUP_H_

#include <dragengine/common/collection/decObjectList.h>

#include "deoglSkinStateConstructedNode.h"

class deSkinPropertyNodeGroup;


/**
 * Skin state constructed node.
 */
class deoglSkinStateCNGroup : public deoglSkinStateConstructedNode{
public:
	typedef deTObjectReference<deoglSkinStateCNGroup> Ref;
	
	
private:
	decObjectList pNodes;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state constructed node. */
	deoglSkinStateCNGroup( deSkinPropertyNodeGroup &node );
	
	/** Create skin state constructed node. */
	deoglSkinStateCNGroup( const deoglSkinStateCNGroup &node );
	
protected:
	/** Clean up skin state mapped. */
	virtual ~deoglSkinStateCNGroup() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Count of child nodes. */
	int GetNodeCount() const;
	
	/** Child node at index. */
	deoglSkinStateConstructedNode *GetNodeAt( int index ) const;
	
	
	
	/**
	 * Update.
	 * \warning Called from main thread.
	 */
	virtual void Update( deoglSkinState &state ) override;
	
	/** Prepare for render. */
	virtual void PrepareForRender( deoglSkinState &state ) override;
	
	/** Render. */
	virtual void Render( deoglSkinState &state, const deoglRenderCanvasContext &context ) override;
	
	/** Create copy. */
	virtual deoglSkinStateConstructedNode::Ref Copy() const override;
	/*@}*/
};

#endif
