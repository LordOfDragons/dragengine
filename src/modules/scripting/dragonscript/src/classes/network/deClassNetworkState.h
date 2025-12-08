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

#ifndef _DECLASSNETWORKSTATE_H_
#define _DECLASSNETWORKSTATE_H_

#include <libdscript/libdscript.h>

class deNetworkState;
class deScriptingDragonScript;


/**
 * \brief Network state native script class.
 */
class deClassNetworkState : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsNetworkStateValueType;
	dsClass *pClsNetworkStateValueFormat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new native script class. */
	deClassNetworkState(deScriptingDragonScript &ds);
	
	/** \brief Clean up native script class. */
	virtual ~deClassNetworkState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Script module. */
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Network state from object. */
	deNetworkState *GetNetworkState(dsRealObject *myself) const;
	
	/** \brief Push network state. */
	void PushNetworkState(dsRunTime *rt, deNetworkState *state);
	
	inline dsClass *GetClassNetworkStateValueType() const{ return pClsNetworkStateValueType; }
	inline dsClass *GetClassNetworkStateValueFormat() const{ return pClsNetworkStateValueFormat; }
	/*@}*/
	
	
	
	
private:
	struct sInitData{
		dsClass *clsNS, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsStr;
		dsClass *clsPt, *clsPt3, *clsVec, *clsQuat, *clsObj, *clsNSL;
		dsClass *clsDVec;
		dsClass *clsVector2;
		dsClass *clsNetworkStateValueType;
		dsClass *clsNetworkStateValueFormat;
		dsClass *clsMemoryFile;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetReadOnly);
	
	DEF_NATFUNC(nfGetValueCount);
	DEF_NATFUNC(nfAddValue);
	DEF_NATFUNC(nfGetValueFormatAt);
	DEF_NATFUNC(nfGetValueIntAt);
	DEF_NATFUNC(nfGetValueFloatAt);
	DEF_NATFUNC(nfGetValueStringAt);
	DEF_NATFUNC(nfGetValuePoint2At);
	DEF_NATFUNC(nfGetValuePoint3At);
	DEF_NATFUNC(nfGetValueVector2At);
	DEF_NATFUNC(nfGetValueVector3At);
	DEF_NATFUNC(nfGetValueDVector3At);
	DEF_NATFUNC(nfGetValueQuaternionAt);
	DEF_NATFUNC(nfSetValueIntAt);
	DEF_NATFUNC(nfSetValueFloatAt);
	DEF_NATFUNC(nfSetValueStringAt);
	DEF_NATFUNC(nfSetValuePoint2At);
	DEF_NATFUNC(nfSetValuePoint3At);
	DEF_NATFUNC(nfSetValueVector2At);
	DEF_NATFUNC(nfSetValueVector3At);
	DEF_NATFUNC(nfSetValueDVector3At);
	DEF_NATFUNC(nfSetValueQuaternionAt);
	DEF_NATFUNC(nfGetValuePrecisionAt);
	DEF_NATFUNC(nfSetValuePrecisionAt);
	DEF_NATFUNC(nfGetValueDataAt);
	DEF_NATFUNC(nfSetValueDataAt);
	
	DEF_NATFUNC(nfGetStateListener);
	DEF_NATFUNC(nfSetStateListener);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
