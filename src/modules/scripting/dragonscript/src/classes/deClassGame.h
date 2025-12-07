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

#ifndef _DECLASSGAME_H_
#define _DECLASSGAME_H_

#include <libdscript/libdscript.h>

class deEngine;
class deScriptingDragonScript;



/**
 * \brief Game script class.
 */
class deClassGame : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassGame(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	virtual ~deClassGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsGame;
		
		dsClass *clsInteger;
		dsClass *clsVoid;
		dsClass *clsString;
		dsClass *clsInputEvent;
	};
	
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfInputEvent);
	DEF_NATFUNC(nfInitGame);
	DEF_NATFUNC(nfCleanUp);
	DEF_NATFUNC(nfOnFrameUpdate);
	DEF_NATFUNC(nfOnResizeRenderWindow);
	DEF_NATFUNC(nfOnAppActivate);
	
	DEF_NATFUNC(nfGetArgumentCount);
	DEF_NATFUNC(nfGetArgumentAt);
	DEF_NATFUNC(nfGetRestartInfo);
#undef DEF_NATFUNC
};

#endif
