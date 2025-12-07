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

#ifndef _DEMCMODINFO_H_
#define _DEMCMODINFO_H_

#include "../modio.h"

#include <dragengine/resources/service/deServiceObject.h>

class deModioUserConfig;


/**
 * Convert mod info.
 */
class deMCModInfo{
private:
	deMCModInfo() = default;
	
public:
	/** Convert identifier list. */
	static std::vector<Modio::ModID> ModIDList(const deServiceObject &so);
	
	/** Convert ModServerSideStatus. */
	static deServiceObject::Ref ModServerSideStatus(Modio::ModServerSideStatus status);
	
	/** Convert mod info. */
	static deServiceObject::Ref ModInfo(const Modio::ModInfo &info, const deModioUserConfig &config);
	
	/** Convert mod stats. */
	static deServiceObject::Ref ModStats(const Modio::ModStats &stats);
	
	/** Convert mod stats. */
	static deServiceObject::Ref ModCollectionEntry(
		const Modio::ModCollectionEntry &status, const deModioUserConfig &config);
	
	/** Convert ModServerSideStatus. */
	static deServiceObject::Ref ModState(Modio::ModState state);
	/*@}*/
};

#endif
