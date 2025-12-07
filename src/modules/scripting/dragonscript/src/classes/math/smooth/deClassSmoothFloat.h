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

#ifndef _DECLASSSMOOTHFLOAT_H_
#define _DECLASSSMOOTHFLOAT_H_

#include <libdscript/libdscript.h>

#include <dragengine/common/math/decMath.h>

class decSmoothFloat;
class deScriptingDragonScript;



/**
 * \brief Smooth float script class.
 */
class deClassSmoothFloat : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
public:
	/** \brief Create a new class. */
	deClassSmoothFloat(deScriptingDragonScript &ds);
	
	/** \brief Clean up the class. */
	virtual ~deClassSmoothFloat();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief DragonScript module. */
	inline deScriptingDragonScript &GetDS(){ return pDS; }
	inline const deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Smooth float from object. */
	const decSmoothFloat &GetSmoothFloat(dsRealObject *myself) const;
	
	/** \brief Push smooth float. */
	void PushSmoothFloat(dsRunTime *rt, const decSmoothFloat &smoothFloat);
	/*@}*/
	
private:
	struct sInitData{
		dsClass *clsSmFloat, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt;
		dsClass *clsFileReader;
		dsClass *clsFileWriter;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNewCopy);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetValue);
	DEF_NATFUNC(nfSetValue);
	DEF_NATFUNC(nfGetGoal);
	DEF_NATFUNC(nfSetGoal);
	DEF_NATFUNC(nfGetAdjustTime);
	DEF_NATFUNC(nfSetAdjustTime);
	DEF_NATFUNC(nfGetAdjustRange);
	DEF_NATFUNC(nfSetAdjustRange);
	DEF_NATFUNC(nfGetChangeSpeed);
	DEF_NATFUNC(nfSetChangeSpeed);
	
	DEF_NATFUNC(nfReset);
	DEF_NATFUNC(nfUpdate);
	
	DEF_NATFUNC(nfReadFromFile);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfToString);
#undef DEF_NATFUNC
};

#endif
