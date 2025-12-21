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


#ifndef _GDEUOCREMOVEOBJECTCLASS_H_
#define _GDEUOCREMOVEOBJECTCLASS_H_

#include <deigde/undo/igdeUndo.h>

#include "../../gamedef/gdeGameDefinition.h"
#include "../../gamedef/objectClass/gdeObjectClass.h"



/**
 * \brief Undo action remove object class.
 */
class gdeURemoveObjectClass : public igdeUndo{
public:
	typedef deTObjectReference<gdeURemoveObjectClass> Ref;
	
	
private:
	gdeGameDefinition::Ref pGameDefinition;
	gdeObjectClass::Ref pObjectClass;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeURemoveObjectClass(gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass);
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeURemoveObjectClass();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
