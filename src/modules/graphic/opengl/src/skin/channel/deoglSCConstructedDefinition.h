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

#ifndef _DEOGLSCCONSTRUCTEDDEFINITION_H_
#define _DEOGLSCCONSTRUCTEDDEFINITION_H_

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>
#include <dragengine/common/math/decMath.h>

class deSkinPropertyConstructed;
#include <dragengine/common/file/decMemoryFileWriter.h>
class decMemoryFile;
class deEngine;


/**
 * Build constructed definition string.
 */
class deoglSCConstructedDefinition : public deSkinPropertyNodeVisitor{
private:
	const deEngine &pEngine;
	decMemoryFileWriter::Ref pDefinition;
	decMemoryFileWriter::Ref pVerify;
	bool pCacheValid;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglSCConstructedDefinition(const deEngine &engine, decMemoryFile *definition,
		decMemoryFile *verify, const deSkinPropertyConstructed &property);
	
	/** Clean up visitor. */
	virtual ~deoglSCConstructedDefinition();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Definitiojn is cache valid. */
	inline bool GetCacheValid() const{ return pCacheValid; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit group node. */
	virtual void VisitGroup(deSkinPropertyNodeGroup &node);
	
	/** Visit image node. */
	virtual void VisitImage(deSkinPropertyNodeImage &node);
	
	/** Visit shape node. */
	virtual void VisitShape(deSkinPropertyNodeShape &node);
	
	/** Visit text node. */
	virtual void VisitText(deSkinPropertyNodeText &node);
	/*@}*/
	
	
	
private:
	void pWriteBasicProperties(const deSkinPropertyNode &node);
	void pWriteFloat(float value, float rounding);
	void pWriteColor(const decColor &color);
};

#endif

