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

#ifndef _SEUMAPPEDREMOVE_H_
#define _SEUMAPPEDREMOVE_H_

#include "../../skin/seSkin.h"
#include "../../skin/mapped/seMapped.h"
#include "../../skin/property/seProperty.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/collection/decObjectList.h>


/**
 * Undo action remove mapped.
 */
class seUMappedRemove : public igdeUndo{
public:
	typedef deTObjectReference<seUMappedRemove> Ref;
	
	
private:
	class sDependency : public deObject{
	public:
		typedef deTObjectReference<sDependency> Ref;
	
	
		const seProperty::Ref property;
		const int mappedComponent;
		
		sDependency( seProperty *aproperty, int amappedComponent ) :
		property( aproperty ),
		mappedComponent( amappedComponent ){ };
	};
	
	
	const seMapped::Ref pMapped;
	const seSkin::Ref pSkin;
	
	decObjectList pDependencies;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	seUMappedRemove( seMapped *mapped );
	
protected:
	/** Clean up undo. */
	virtual ~seUMappedRemove();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Count of dependencies. */
	int GetDependencyCount() const;
	
	/** Undo action. */
	virtual void Undo();
	
	/** Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
