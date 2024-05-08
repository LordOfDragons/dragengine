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

#ifndef _DECLASSENGINE_H_
#define _DECLASSENGINE_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/utils/decTimer.h>

class deScriptingDragonScript;


/**
 * Game engine.
 */
class deClassEngine : public dsClass{
private:
	deScriptingDragonScript &pDS;
	decTimer pDebugTimer;
	bool pDefaultEnableGI;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create script class. */
	deClassEngine( deScriptingDragonScript &ds );
	
	/** Clean up script class. */
	virtual ~deClassEngine();
	
	
	
	/** \name Management */
	/*@{*/
	/** Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Debug timer. */
	inline decTimer &GetDebugTimer(){ return pDebugTimer; }
	
	/** Default enable GI. */
	inline bool GetDefaultEnableGI() const{ return pDefaultEnableGI; }
	
	/** Set default enable GI. */
	void SetDefaultEnableGI( bool enable );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsEngine;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsBoolean;
		dsClass *clsSet;
		dsClass *clsDictionary;
		
		dsClass *clsWindow;
		dsClass *clsGame;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	// engine
	DEF_NATFUNC( nfGetElapsedTime );
	DEF_NATFUNC( nfResetTimers );
	DEF_NATFUNC( nfGetGame );
	DEF_NATFUNC( nfGetFPSRate );
	DEF_NATFUNC( nfQuit );
	DEF_NATFUNC( nfGetEditMode );
	DEF_NATFUNC( nfLoadingResourceCount );
	DEF_NATFUNC( nfLog );
	DEF_NATFUNC( nfGetScriptModuleStats );
	DEF_NATFUNC( nfGetAppActive );
	DEF_NATFUNC( nfUpdateResourceLoading );
	
	DEF_NATFUNC( nfGetCompatibleVersion );
	DEF_NATFUNC( nfGetCompatibleVersionMajor );
	DEF_NATFUNC( nfGetCompatibleVersionMinor );
	DEF_NATFUNC( nfGetCompatibleVersionPatch );
	DEF_NATFUNC( nfIsCompatibleVersionOlder );
	DEF_NATFUNC( nfIsCompatibleVersionNewer );
	
	DEF_NATFUNC( nfGetModuleVersion );
	DEF_NATFUNC( nfGetModuleVersionMajor );
	DEF_NATFUNC( nfGetModuleVersionMinor );
	DEF_NATFUNC( nfGetModuleVersionPatch );
	DEF_NATFUNC( nfIsModuleVersionOlder );
	DEF_NATFUNC( nfIsModuleVersionNewer );
	
	DEF_NATFUNC( nfGetDefaultEnableGI );
	DEF_NATFUNC( nfSetDefaultEnableGI );
	
	DEF_NATFUNC( nfGetUserLocaleLanguage );
	DEF_NATFUNC( nfGetUserLocaleTerritory );
	
	DEF_NATFUNC( nfGetSupportedServices );
#undef DEF_NATFUNC
};

#endif
