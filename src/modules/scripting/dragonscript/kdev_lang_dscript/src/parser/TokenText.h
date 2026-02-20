#ifndef _TOKENTEXT_H_
#define _TOKENTEXT_H_

#include <QtCore/QString>

// this is only required because KDevelop-PG-Qt creates a total train-wreck of a
// dsp_tokentext.h which defines fuctions in headers! this causes multiple-definition
// problems. so we limit now the text to one inclusion unit only. stupid but necessary
namespace DragonScript{
	QString TokenText(int token);
}

#endif
