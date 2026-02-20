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

#include <stdio.h>
#include <stdlib.h>

#include "deModelLodVertexPositionSet.h"
#include "deModelLodVertexPositionSetPosition.h"
#include "../../common/exceptions.h"



// Class deModelLodVertexPositionSet
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deModelLodVertexPositionSet::deModelLodVertexPositionSet() = default;

deModelLodVertexPositionSet::deModelLodVertexPositionSet(const deModelLodVertexPositionSet &set) :
pPositions(set.pPositions){
}

deModelLodVertexPositionSet &deModelLodVertexPositionSet::operator=(const deModelLodVertexPositionSet &set){
	if(this != &set){
		pPositions = set.pPositions;
	}
	return *this;
}

deModelLodVertexPositionSet::deModelLodVertexPositionSet(deModelLodVertexPositionSet &&set) noexcept :
pPositions(std::move(set.pPositions)){
}

deModelLodVertexPositionSet &deModelLodVertexPositionSet::operator=(deModelLodVertexPositionSet &&set) noexcept{
	if(this != &set){
		pPositions = std::move(set.pPositions);
	}
	return *this;
}
