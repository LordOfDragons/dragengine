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

#ifndef _DERLTASKREADSKINPROPERTYNODE_H_
#define _DERLTASKREADSKINPROPERTYNODE_H_

#include "../../skin/property/node/deSkinPropertyNodeVisitor.h"
#include "../../../common/string/decString.h"

class deResourceLoader;
class deRLTaskReadSkinInternal;
class deEngine;
class deVirtualFileSystem;


/**
 * \brief Skin constructed property node visitor loading resources in nodes.
 * 
 * For use by deResourceLoader for loading resource inside skins loading using
 * asynchronous skin loading.
 * 
 * If a resource is already present loading is skiped. This can be used by
 * deSkinBuilder users to use already loaded resources.
 */
class DE_DLL_EXPORT deRLTaskReadSkinPropertyNode : public deSkinPropertyNodeVisitor{
private:
	deResourceLoader &pResourceLoader;
	deRLTaskReadSkinInternal &pTask;
	deEngine &pEngine;
	deVirtualFileSystem * const pVirtualFileSystem;
	const decString pBasePath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deRLTaskReadSkinPropertyNode(deResourceLoader &resourceLoader, deRLTaskReadSkinInternal &task,
		deEngine &engine, deVirtualFileSystem *vfs, const char *basePath);
	
	/** \brief Clean up visitor. */
	~deRLTaskReadSkinPropertyNode() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit node. */
	void VisitNode(deSkinPropertyNode &node) override;
	
	/** \brief Visit group node. */
	void VisitGroup(deSkinPropertyNodeGroup &node) override;
	
	/** \brief Visit image node. */
	void VisitImage(deSkinPropertyNodeImage &node) override;
	
	/** \brief Visit text node. */
	void VisitText(deSkinPropertyNodeText &node) override;
	/*@}*/
};

#endif
