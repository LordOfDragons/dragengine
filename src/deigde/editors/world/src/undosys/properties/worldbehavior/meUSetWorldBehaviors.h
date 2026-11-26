/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

<<<<<<<< HEAD:src/modules/graphic/opengl/src/parameters/vr/deoglPVRSubmitDepth.h
#ifndef _DEOGLPVRSUBMITDEPTH_H_
#define _DEOGLPVRSUBMITDEPTH_H_

#include "../deoglParameterBool.h"


/**
 * VR submit depth parameter.
 */
class deoglPVRSubmitDepth : public deoglParameterBool{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create parameter. */
	explicit deoglPVRSubmitDepth(deGraphicOpenGl &ogl);
	
	/** Clean up parameter. */
	~deoglPVRSubmitDepth() override;
========
#ifndef _MEUSETWORLDBEHAVIORS_H_
#define _MEUSETWORLDBEHAVIORS_H_

#include <deigde/undo/igdeUndo.h>

#include "../../../world/meWorld.h"


/**
 * Undo set world behaviors.
 */
class meUSetWorldBehaviors : public igdeUndo{
public:
	using Ref = deTObjectReference<meUSetWorldBehaviors>;
	
	
private:
	meWorld *pWorld;
	meWorldBehavior::List pOldValue, pNewValue;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	meUSetWorldBehaviors(meWorld *world, const meWorldBehavior::List &newValue);
	
protected:
	/** Clean up undo. */
	~meUSetWorldBehaviors() override;
>>>>>>>> origin/master:src/deigde/editors/world/src/undosys/properties/worldbehavior/meUSetWorldBehaviors.h
	/*@}*/
	
	
public:
<<<<<<<< HEAD:src/modules/graphic/opengl/src/parameters/vr/deoglPVRSubmitDepth.h
	/** \name Parameter Value */
	/*@{*/
	/** Current value. */
	bool GetParameterBool() override;
	
	/** Set current value. */
	void SetParameterBool(bool value) override;
========
	/** \name Management */
	/*@{*/
	/** Undo. */
	void Undo() override;
	
	/** Redo. */
	void Redo() override;
>>>>>>>> origin/master:src/deigde/editors/world/src/undosys/properties/worldbehavior/meUSetWorldBehaviors.h
	/*@}*/
};

#endif
