#ifndef DSLANGUAGESUPPORT_H
#define DSLANGUAGESUPPORT_H

#include <interfaces/idocument.h>
#include <interfaces/iplugin.h>
#include <interfaces/ilanguagecheck.h>
#include <interfaces/ilanguagecheckprovider.h>
#include <language/interfaces/ilanguagesupport.h>
#include <language/duchain/topducontext.h>

using KDevelop::ICodeHighlighting;
using KDevelop::IDocument;
using KDevelop::ILanguageSupport;
using KDevelop::IndexedString;
using KDevelop::IPlugin;
using KDevelop::ParseJob;
using KDevelop::ReferencedTopDUContext;
using KDevelop::SourceFormatterItemList;
using KDevelop::ConfigPage;

namespace DragonScript {

class Highlighting;

/**
 * \brief Language support module for DragonScript language.
 */
class DSLanguageSupport : public IPlugin, public ILanguageSupport {
	Q_OBJECT
	Q_INTERFACES(KDevelop::ILanguageSupport)
	
private:
	Highlighting *pHighlighting;
	static DSLanguageSupport *pSelf;
	
	
	
public:
	/** \brief Create language support. */
	DSLanguageSupport(QObject* parent, const QVariantList& args);
	
	/** \brief Clean up language support. */
	virtual ~DSLanguageSupport();
	
	
	
	/** \brief Singleton. */
	inline static DSLanguageSupport *self(){ return pSelf; }
	
	/** \brief Name of language.*/
	virtual QString name() const override;
	
	/** \brief Create parse job used by background parser to parse \p url.*/
	virtual ParseJob *createParseJob(const IndexedString &url) override;
	
	/** \brief Code highlighting instance. */
	ICodeHighlighting *codeHighlighting() const override;
	
	/** \brief Per-project configuration pages. */
	int perProjectConfigPages() const override;
	
	/** \brief Create per-project configuration page. */
	ConfigPage* perProjectConfigPage(int number, const KDevelop::ProjectConfigOptions &options,
		QWidget *parent) override;
};

}

#endif
