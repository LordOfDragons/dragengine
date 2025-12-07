/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _MEUOBJECTRANDOMROTATION_H_
#define _MEUOBJECTRANDOMROTATION_H_

#include "../../gui/object/meUndoDataObject.h"
#include "../../../world/meWorld.h"

#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/collection/decObjectOrderedSet.h>


/**
 * Undo action applying random rotating to objects.
 */
class meUObjectRandomRotation : public igdeUndo{
private:
	class ObjectData : public meUndoDataObject{
	public:
		typedef deTObjectReference<ObjectData> Ref;
	
	
	private:
		decVector pNewRotation;
		
	public:
		ObjectData(meObject *object, bool randomizeX, bool randomizeY, bool randomizeZ) :
		meUndoDataObject(object)
		{
			pNewRotation = GetOldOrientation();
			if(randomizeX){
				pNewRotation.x = decMath::random(-180.0f, 180.0f);
			}
			if(randomizeY){
				pNewRotation.y = decMath::random(-180.0f, 180.0f);
			}
			if(randomizeZ){
				pNewRotation.z = decMath::random(-180.0f, 180.0f);
			}
		}
		
		inline const decVector &GetNewOrientation() const{ return pNewRotation; }
	};
	
	
	const meWorld::Ref pWorld;
	decObjectOrderedSet pObjects;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo action. */
	meUObjectRandomRotation(meWorld *world, const meObjectList &objects,
		bool randomizeX, bool randomizeY, bool randomizeZ);
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
