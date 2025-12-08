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

#ifndef _DECLASSBILLBOARD_H_
#define _DECLASSBILLBOARD_H_

#include <libdscript/libdscript.h>

class deEngine;
class deBillboard;
class deScriptingDragonScript;



/**
 * \brief Billboard Script Class.
 */
class deClassBillboard : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassBillboard(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassBillboard();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Billboard from object. */
	deBillboard *GetBillboard(dsRealObject *myself) const;
	
	/** \brief Pushe billboard. */
	void PushBillboard(dsRunTime *rt, deBillboard::Ref billboard);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsBillboard;
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsBool;
		dsClass *clsObject;
		
		dsClass *clsVector;
		dsClass *clsVector2;
		dsClass *clsDVector;
		dsClass *clsSkin;
		dsClass *clsDynamicSkin;
		dsClass *clsLayerMask;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetAxis);
	DEF_NATFUNC(nfSetAxis);
	DEF_NATFUNC(nfGetSize);
	DEF_NATFUNC(nfSetSize);
	DEF_NATFUNC(nfGetOffset);
	DEF_NATFUNC(nfSetOffset);
	DEF_NATFUNC(nfGetSkin);
	DEF_NATFUNC(nfSetSkin);
	DEF_NATFUNC(nfGetDynamicSkin);
	DEF_NATFUNC(nfSetDynamicSkin);
	DEF_NATFUNC(nfGetLocked);
	DEF_NATFUNC(nfSetLocked);
	DEF_NATFUNC(nfGetSpherical);
	DEF_NATFUNC(nfSetSpherical);
	DEF_NATFUNC(nfGetSizeFixedToScreen);
	DEF_NATFUNC(nfSetSizeFixedToScreen);
	DEF_NATFUNC(nfGetVisible);
	DEF_NATFUNC(nfSetVisible);
	DEF_NATFUNC(nfGetLayerMask);
	DEF_NATFUNC(nfSetLayerMask);
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
