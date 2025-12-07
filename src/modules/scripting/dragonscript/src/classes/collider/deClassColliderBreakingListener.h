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

#ifndef _DECLASSCOLLIDERBREAKINGLISTENER_H_
#define _DECLASSCOLLIDERBREAKINGLISTENER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;



/**
 * \brief Collider breaking listener script class.
 */
class deClassColliderBreakingListener : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassColliderBreakingListener(deScriptingDragonScript *ds);
	/** \brief Cleans up the class. */
	virtual ~deClassColliderBreakingListener();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{return pDS;}
	
	/** \brief Creates the class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Function index colliderConstraintBroke(). */
	inline int GetFuncIndexColliderConstraintBroke() const{return pFuncIndexColliderConstraintBroke;}
	
	/** \brief Function index rigConstraintBroke(). */
	inline int GetFuncIndexRigConstraintBroke() const{return pFuncIndexRigConstraintBroke;}
	/*@}*/
	
private:
	int pFuncIndexColliderConstraintBroke;
	int pFuncIndexRigConstraintBroke;
	
	struct sInitData{
		dsClass *clsCBL, *clsVoid, *clsBool, *clsFlt, *clsInt, *clsCol;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfColliderConstraintBroke);
	DEF_NATFUNC(nfRigConstraintBroke);
#undef DEF_NATFUNC
};

#endif
