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

#ifndef _DECLASSLAYERMASK_H_
#define _DECLASSLAYERMASK_H_

#include <libdscript/libdscript.h>

class decLayerMask;
class deScriptingDragonScript;



/**
 * \brief Layer Mask Script Class.
 */
class deClassLayerMask : public dsClass{
private:
	deScriptingDragonScript *pDS;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new class. */
	deClassLayerMask(deScriptingDragonScript *ds);
	/** \brief Cleans up the class. */
	virtual ~deClassLayerMask();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the module. */
	inline deScriptingDragonScript *GetDS() const{return pDS;}
	
	/** \brief Creates the class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Retrieves the layer stored in the given real object. */
	const decLayerMask &GetLayerMask(dsRealObject *myself) const;
	/** \brief Pushes the given layermask onto the stack. */
	void PushLayerMask(dsRunTime *rt, const decLayerMask &layerMask);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsLyM, *clsVoid, *clsInt, *clsStr, *clsObj, *clsBool;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNew2);
	DEF_NATFUNC(nfNewWith);
	DEF_NATFUNC(nfNewWith2);
	DEF_NATFUNC(nfNewWith3);
	DEF_NATFUNC(nfNewWith4);
	DEF_NATFUNC(nfNewAll);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfClearMask);
	DEF_NATFUNC(nfSetBit);
	DEF_NATFUNC(nfClearBit);
	DEF_NATFUNC(nfIsBitSet);
	DEF_NATFUNC(nfIsBitCleared);
	DEF_NATFUNC(nfIsEmpty);
	
	DEF_NATFUNC(nfToBitString);
	DEF_NATFUNC(nfSetFromBitString);
	DEF_NATFUNC(nfToHexString);
	DEF_NATFUNC(nfSetFromHexString);
	
	DEF_NATFUNC(nfMatches);
	DEF_NATFUNC(nfMatchesNot);
	
	DEF_NATFUNC(nfOpInverse);
	DEF_NATFUNC(nfOpAnd);
	DEF_NATFUNC(nfOpOr);
	DEF_NATFUNC(nfOpXor);
	DEF_NATFUNC(nfOpAndAssign);
	DEF_NATFUNC(nfOpOrAssign);
	DEF_NATFUNC(nfOpXorAssign);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfToString);
#undef DEF_NATFUNC
};

#endif
