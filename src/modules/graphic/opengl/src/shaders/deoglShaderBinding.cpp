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

#include "deoglShaderBinding.h"

#include <dragengine/common/exceptions.h>


// Class deoglShaderBinding::List
///////////////////////////////////

bool deoglShaderBinding::List::HasNamed(const char *name){
	return HasMatching([&](const deoglShaderBinding &binding){
		return binding.GetName() == name;
	});
}

int deoglShaderBinding::List::IndexOfNamed(const char *name) const{
	return IndexOfMatching([&](const deoglShaderBinding &binding){
		return binding.GetName() == name;
	});
}


// Class deoglShaderBinding
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglShaderBinding::deoglShaderBinding() :
pTarget(-1){
}

deoglShaderBinding::deoglShaderBinding(const char *name, int target) :
pName(name),
pTarget(target){
}

deoglShaderBinding::deoglShaderBinding(const deoglShaderBinding &binding) :
pName(binding.pName),
pTarget(binding.pTarget){
}

deoglShaderBinding deoglShaderBinding::operator=(const deoglShaderBinding &binding){
	pName = binding.pName;
	pTarget = binding.pTarget;
	return *this;
}

deoglShaderBinding::deoglShaderBinding(deoglShaderBinding &&binding) noexcept :
pName(std::move(binding.pName)),
pTarget(binding.pTarget){
}

deoglShaderBinding deoglShaderBinding::operator=(deoglShaderBinding &&binding) noexcept{
	pName = std::move(binding.pName);
	pTarget = binding.pTarget;
	return *this;
}
