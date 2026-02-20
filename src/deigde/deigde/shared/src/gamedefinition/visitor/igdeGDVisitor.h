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

#ifndef _IGDEGDVISITOR_H_
#define _IGDEGDVISITOR_H_

#include <dragengine/dragengine_export.h>

class igdeGDClass;
class igdeGDSkin;
class igdeGDSky;



/**
 * \brief Game Definition Visitor.
 */
class DE_DLL_EXPORT igdeGDVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	igdeGDVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~igdeGDVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit object class. */
	virtual void VisitObjectClass(igdeGDClass *gdclass);
	
	/** \brief Visit skin. */
	virtual void VisitSkin(igdeGDSkin *gdskin);
	
	/** \brief Visit sky. */
	virtual void VisitSky(igdeGDSky *gdsky);
	/*@}*/
};

#endif
