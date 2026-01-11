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

#ifndef _DEDEANIMATOR_H_
#define _DEDEANIMATOR_H_

#include <dragengine/systems/modules/animator/deBaseAnimatorModule.h>



/**
 * DEAnimator animator module.
 */
class deDEAnimator : public deBaseAnimatorModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create animator module. */
	deDEAnimator(deLoadableModule &loadableModule);
	
	/** Clean up animator module. */
	~deDEAnimator() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** Initialize module. */
	bool Init() override;
	
	/** Shut down module and clean up. */
	void CleanUp() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Create peer for animator. */
	deBaseAnimatorAnimator *CreateAnimator(deAnimator *animator) override;
	
	/** Create peer for animator instance. */
	deBaseAnimatorAnimatorInstance *CreateAnimatorInstance(deAnimatorInstance *instance) override;
	
	/** Create peer for animation. */
	deBaseAnimatorAnimation *CreateAnimation(deAnimation *animation) override;
	
	/** Create peer for component. */
	deBaseAnimatorComponent *CreateComponent(deComponent *component) override;
	/*@}*/
};

#endif
