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

#ifndef _DERLTASKREADSKINPROPERTY_H_
#define _DERLTASKREADSKINPROPERTY_H_

#include "deRLTaskReadSkinPropertyNode.h"
#include "../../skin/property/deSkinPropertyVisitor.h"
#include "../../../common/string/decString.h"

class deResourceLoader;
class deRLTaskReadSkinInternal;
class deEngine;
class deVirtualFileSystem;


/**
 * \brief Skin property visitor loading resources in properties.
 * 
 * For use by deResourceLoader for loading resource inside skins loadinged using
 * asynchronous skin loading.
 * 
 * If a resource is already present loading is skiped. This can be used by
 * deSkinBuilder users to use already loaded resources.
 */
class DE_DLL_EXPORT deRLTaskReadSkinProperty : public deSkinPropertyVisitor{
private:
	deResourceLoader &pResourceLoader;
	deRLTaskReadSkinInternal &pTask;
	deEngine &pEngine;
	deVirtualFileSystem * const pVirtualFileSystem;
	const decString pBasePath;
	deRLTaskReadSkinPropertyNode pLoadNode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deRLTaskReadSkinProperty(deResourceLoader &resourceLoader, deRLTaskReadSkinInternal &task,
		deEngine &engine, deVirtualFileSystem *vfs, const char *basePath);
	
	/** \brief Clean up visitor. */
	~deRLTaskReadSkinProperty() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit image property. */
	void VisitImage(deSkinPropertyImage &property) override;
	
	/** \brief Visit video property. */
	void VisitVideo(deSkinPropertyVideo &property) override;
	
	/** \brief Visit constructed property. */
	void VisitConstructed(deSkinPropertyConstructed &property) override;
	/*@}*/
};

#endif
