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

#ifndef _DEMODIOUSERCONFIG_H_
#define _DEMODIOUSERCONFIG_H_

#include "../modio.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decStringSet.h>
#include <dragengine/common/collection/decIntDictionary.h>

class deModio;
class decBaseFileReader;
class decBaseFileWriter;


/**
 * User configuration.
 */
class deModioUserConfig : public deObject{
public:
	typedef deTObjectReference<deModioUserConfig> Ref;
	
	
private:
	deModio &pModule;
	decString pId;
	decStringSet pDisabledMods;
	decIntDictionary pUserRatings;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create mod config. */
	deModioUserConfig(deModio &module, const decString &id);
	
	/** Load mod config. */
	deModioUserConfig(deModio &module, decBaseFileReader &reader);
	
protected:
	/** Delete mod config. */
	~deModioUserConfig() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** User id. */
	inline const decString &GetId() const{ return pId; }
	
	
	
	/** Disabled modifications. */
	inline const decStringSet &GetDisabledMods() const{ return pDisabledMods; }
	
	/** Disabled modifications. Saving on changing has to be done manually. */
	inline decStringSet &GetDisabledMods(){ return pDisabledMods; }
	
	/** Set disabled modifications. */
	void SetDisabledMods(const decStringSet &mods);
	
	/** Mod disabled. */
	bool GetModDisabled(const decString &id) const;
	
	/** Set mod disabled. */
	void SetModDisabled(const decString &id, bool disabled);
	
	
	
	/** User ratings. */
	inline const decIntDictionary &GetUserRatings() const{ return pUserRatings; }
	
	/** User rating for mod. */
	Modio::Rating GetUserRating(const decString &id) const;
	
	/** Set user rating for mod. */
	void SetUserRating(const decString &id, Modio::Rating rating);
	
	
	
	/** Write config to file. */
	void WriteToFile(decBaseFileWriter &writer);
	/*@}*/
	
	
	
private:
	void pReadFromFileV0(decBaseFileReader &reader);
};

#endif
