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

#ifndef _DEBPDEVELOPERMODE_H_
#define _DEBPDEVELOPERMODE_H_

#include <dragengine/common/utils/decLayerMask.h>

class dePhysicsBullet;

class debpWorld;

class decUnicodeArgumentList;
class decUnicodeString;



/**
 * Developer Mode.
 *
 * Provides access to the developer mode. This is not required for games
 * nor editing tools and is used only by the module developers for testing
 * and trouble shooting.
 * 
 * The Take-Snapshot is intended to store a COLLADE snapshot of the next
 * collision detection run requested by the host application. After the
 * snapshot is done the flag is reset.
 */
class debpDeveloperMode{
private:
	dePhysicsBullet &pBullet;
	bool pEnabled;
	
	bool pTakeSnapshot;
	decLayerMask pShowCategory;
	int pHighlightResponseType;
	bool pHighlightDeactivation;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create developer mode. */
	debpDeveloperMode(dePhysicsBullet &bullet);
	
	/** Clean up developer mode. */
	~debpDeveloperMode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Executes a command.
	 * \details If the command is recognized true is returned otherwise false.
	 */
	bool ExecuteCommand(const decUnicodeArgumentList &command, decUnicodeString &answer);
	
	/** Developer mode is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** Show category layer mask. */
	inline const decLayerMask &GetShowCategory() const{ return pShowCategory; }
	
	/** Highlight response type or -1 if disabled. */
	inline int GetHighlightResponseType() const{ return pHighlightResponseType; }
	
	/** Highlight deactivation state. */
	inline bool GetHighlightDeactivation() const{ return pHighlightDeactivation; }
	
	/**
	 * Take snapshot of world if required.
	 * 
	 * By default this does nothing unless a snapshot has been requested beforehand in which
	 * case the given world is written to a file using the collada exporter provided in BULLET.
	 */
	void TakeSnapshot(debpWorld *world);
	/*@}*/
	
	
	
private:
	void pCmdHelp(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdEnable(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdTakeSnapshot(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdShowCategory(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdHighlightResponseType(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdHighlightDeactivation(const decUnicodeArgumentList &command, decUnicodeString &answer);
	void pCmdDebugEnable(const decUnicodeArgumentList &command, decUnicodeString &answer);
};

#endif
