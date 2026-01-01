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

// include only once
#ifndef _MEDECALMOVE_H_
#define _MEDECALMOVE_H_

#include "meUndoDataDecal.h"
#include "../meBaseUndoMove.h"
#include "../../../world/meWorld.h"



/**
 * \brief Move Decals Undo Action.
 *
 * Undo Action for interactively moving decals.
 */
class meUDecalMove : public meBaseUndoMove{
public:
	typedef deTObjectReference<meUDecalMove> Ref;
	
private:
	meWorld::Ref pWorld;
	
	meUndoDataDecal::List pDecals;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new undo. */
	meUDecalMove(meWorld *world);
	
protected:
	/** \brief Clean up undo. */
	~meUDecalMove() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	virtual void Undo();
	virtual void Redo();
	void ProgressiveRedo() override;
	/*@}*/
};

#endif
