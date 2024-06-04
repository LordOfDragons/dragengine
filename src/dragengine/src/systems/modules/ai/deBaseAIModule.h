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

#ifndef _DEBASEAIMODULE_H_
#define _DEBASEAIMODULE_H_

#include "../deBaseModule.h"

class deHeightTerrain;
class deBaseAIHeightTerrain;
class deBaseAIAI;
class deBaseAINavigationBlocker;
class deBaseAINavigationSpace;
class deBaseAINavigator;
class deBaseAIWorld;
class deNavigationBlocker;
class deNavigationSpace;
class deNavigator;
class deWorld;



/**
 * \brief Base AI module interface.
 */
class DE_DLL_EXPORT deBaseAIModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new AI module. */
	deBaseAIModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up AI module. */
	virtual ~deBaseAIModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initialize ai module. */
	virtual bool Init() = 0;
	
	/** \brief Shut down ai module and clean up. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for a world object. */
	virtual deBaseAIWorld *CreateWorld( deWorld *world ) = 0;
	
	/** \brief Create peer for a navigation space object. */
	virtual deBaseAINavigationSpace *CreateNavigationSpace( deNavigationSpace *navspace ) = 0;
	
	/** \brief Create peer for a navigation blocker object. */
	virtual deBaseAINavigationBlocker *CreateNavigationBlocker( deNavigationBlocker *blocker ) = 0;
	
	/** \brief Create peer for a navigator object. */
	virtual deBaseAINavigator *CreateNavigator( deNavigator *navigator ) = 0;
	
	/** \brief Create peer for height terrain. */
	virtual deBaseAIHeightTerrain *CreateHeightTerrain( deHeightTerrain &heightTerrain ) = 0;
	/*@}*/
};

#endif
