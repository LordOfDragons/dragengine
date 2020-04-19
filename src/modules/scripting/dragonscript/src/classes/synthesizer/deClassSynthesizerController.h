/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DEDSCLASSSYNTHESIZERCONTROLLER_H_
#define _DEDSCLASSSYNTHESIZERCONTROLLER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSynthesizer;
class deSynthesizerInstance;



/**
 * \brief Synthesizer controller script class.
 */
class deClassSynthesizerController : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSynthesizerController( deScriptingDragonScript &ds );
	
	/** \brief Clean up class. */
	virtual ~deClassSynthesizerController();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** \brief Get parameters from object. */
	void GetController( dsRealObject *myself, deSynthesizer *&synthesizer,
		deSynthesizerInstance *&instance, int &index ) const;
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deSynthesizer *synthesizer, int index );
	
	/** \brief Push controller. */
	void PushController( dsRunTime *rt, deSynthesizerInstance *instance, int index );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSynthesizerCtrl;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsCurveBezier;
		dsClass *clsSynthesizer;
		dsClass *clsSynthesizerInst;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetSynthesizer );
	DEF_NATFUNC( nfGetSynthesizerInstance );
	DEF_NATFUNC( nfGetControllerIndex );
	
	DEF_NATFUNC( nfGetName );
	DEF_NATFUNC( nfSetName );
	DEF_NATFUNC( nfGetLower );
	DEF_NATFUNC( nfGetUpper );
	DEF_NATFUNC( nfSetRange );
	DEF_NATFUNC( nfGetCurve );
	DEF_NATFUNC( nfSetCurve );
	DEF_NATFUNC( nfSetCurveConstant );
	DEF_NATFUNC( nfSetCurveTime );
	DEF_NATFUNC( nfGetClamp );
	DEF_NATFUNC( nfSetClamp );
	DEF_NATFUNC( nfSetCurveFrom );
	DEF_NATFUNC( nfSetAllFrom );
	
	DEF_NATFUNC( nfEquals );
#undef DEF_NATFUNC
};

#endif
