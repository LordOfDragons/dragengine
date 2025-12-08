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

#ifndef _MEUROTATEOBJECT_H_
#define _MEUROTATEOBJECT_H_

#include "../meBaseUndoRotate.h"
#include <dragengine/common/collection/decObjectOrderedSet.h>

class meObjectList;
class meWorld;



/**
 * Undo action for rotating a set of objects.
 */
class meURotateObject : public meBaseUndoRotate{
private:
	meWorld::Ref pWorld;
	decObjectOrderedSet pObjects;
	
public:
	// constructor, destructor
	meURotateObject(meWorld::Ref world, const meObjectList &objects);
	virtual ~meURotateObject();
	
	// undo and redo operations
	virtual void Undo();
	virtual void Redo();
	void ProgressiveRedo() override;
	
private:
	void pCleanUp();
};

// end of include only once
#endif
