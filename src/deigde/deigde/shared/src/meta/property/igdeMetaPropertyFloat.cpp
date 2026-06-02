/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "igdeMetaPropertyFloat.h"
#include "widget/igdeMetaPropertyFloatWidget.h"


// Class igdeMetaPropertyFloat
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeMetaPropertyFloat::igdeMetaPropertyFloat(const char *name, const char *description) :
igdeMetaProperty(name, description),
pPrecision(3),
pDefaultValue(0.0f),
pLowerLimit(0.0f),
pUpperLimit(1.0f),
pTickSpacing(0.1f),
pEnableLowerLimit(false),
pEnableUpperLimit(false){
}

igdeMetaPropertyFloat::~igdeMetaPropertyFloat() = default;


// Management
///////////////

void igdeMetaPropertyFloat::SetPrecision(int precision){
	pPrecision = decMath::max(precision, 0);
}

void igdeMetaPropertyFloat::SetDefaultValue(float value){
	pDefaultValue = value;
}


void igdeMetaPropertyFloat::SetLowerLimit(float value){
	pLowerLimit = value;
}

void igdeMetaPropertyFloat::SetUpperLimit(float value){
	pUpperLimit = value;
}

void igdeMetaPropertyFloat::SetTickSpacing(float value){
	pTickSpacing = decMath::max(value, 0.0f);
}

void igdeMetaPropertyFloat::SetEnableLowerLimit(bool enable){
	pEnableLowerLimit = enable;
}

void igdeMetaPropertyFloat::SetEnableUpperLimit(bool enable){
	pEnableUpperLimit = enable;
}


igdeMetaPropertyWidget::Ref igdeMetaPropertyFloat::CreateWidget(const igdeMetaContext::Ref &context){
	return igdeMetaPropertyFloatWidget::Ref::New(*this, context);
}
