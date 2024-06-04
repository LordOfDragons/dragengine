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

#ifndef _DEBASEANIMATORMODULE_H_
#define _DEBASEANIMATORMODULE_H_

#include "../deBaseModule.h"

class deAnimation;
class deAnimator;
class deAnimatorInstance;
class deBaseAnimatorAnimation;
class deBaseAnimatorAnimator;
class deBaseAnimatorAnimatorInstance;
class deBaseAnimatorComponent;
class deComponent;



/**
 * \brief Base animator module providing support for producing animations.
 */
class DE_DLL_EXPORT deBaseAnimatorModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseAnimatorModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseAnimatorModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down module and clean up. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for animator. */
	virtual deBaseAnimatorAnimator *CreateAnimator( deAnimator *animator ) = 0;
	
	/** \brief Create peer for animator instance. */
	virtual deBaseAnimatorAnimatorInstance *CreateAnimatorInstance( deAnimatorInstance *instance ) = 0;
	
	/** \brief Create peer for animation. */
	virtual deBaseAnimatorAnimation *CreateAnimation( deAnimation *animation ) = 0;
	
	/** \brief Create peer for component. */
	virtual deBaseAnimatorComponent *CreateComponent( deComponent *component ) = 0;
	/*@}*/
};

#endif
