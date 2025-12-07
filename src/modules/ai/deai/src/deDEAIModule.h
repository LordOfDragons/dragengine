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

#ifndef _DEDEAI_H_
#define _DEDEAI_H_

#include "dragengine/systems/modules/ai/deBaseAIModule.h"


class dedaiDeveloperMode;
class dedaiCommandExecuter;



/**
 * \brief DEAI AI module.
 */
class deDEAIModule : public deBaseAIModule{
private:
	dedaiDeveloperMode *pDeveloperMode;
	dedaiCommandExecuter *pCommandExecuter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new ai module. */
	deDEAIModule(deLoadableModule &loadableModule);
	/** \brief Cleans up the ai module. */
	virtual ~deDEAIModule();
	/*@}*/
	
	/** \name Module Management */
	/*@{*/
	/** \brief Initializes the ai module. */
	virtual bool Init();
	/** \brief Shuts down the ai module and cleans up. */
	virtual void CleanUp();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Create peer for a world object. */
	virtual deBaseAIWorld *CreateWorld(deWorld *world);
	
	/** \brief Create peer for a navigation space object. */
	virtual deBaseAINavigationSpace *CreateNavigationSpace(deNavigationSpace *navspace);
	
	/** \brief Create peer for a navigation blocker object. */
	virtual deBaseAINavigationBlocker *CreateNavigationBlocker(deNavigationBlocker *blocker);
	
	/** \brief Create peer for a navigator object. */
	virtual deBaseAINavigator *CreateNavigator(deNavigator *navigator);
	
	/** \brief Create peer for height terrain. */
	virtual deBaseAIHeightTerrain *CreateHeightTerrain(deHeightTerrain &heightTerrain);
	
	/** \brief Retrieves the developer mode. */
	inline dedaiDeveloperMode &GetDeveloperMode() const{ return *pDeveloperMode; }
	/*@}*/
	
	/** \name Debugging */
	/*@{*/
	/**
	 * \brief Sends a command to the module and retrieves an answer from it.
	 * \details At least the 'help' command has to be understood answering a
	 *          list of possible commands. The command is provided in the 'command'
	 *          parameter and the answer has to be written into 'answer'. The default
	 *          implementation simply answers only to help with itself.
	 * \param[in] command The command to send to the module as a list of arguments.
	 * \param[out] answer Unicode string to hold the answer returned from the module.
	 */
	virtual void SendCommand(const decUnicodeArgumentList &command, decUnicodeString &answer);
	/*@}*/
};

#endif
