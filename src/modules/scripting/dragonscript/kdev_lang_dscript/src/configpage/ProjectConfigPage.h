#ifndef DSPROJECTCONFIGPAGE_H
#define DSPROJECTCONFIGPAGE_H

#include <QStringListModel>
#include <KConfigGroup>
#include <project/projectconfigpage.h>

#include "configpageexport.h"

using KDevelop::ConfigPage;
using KDevelop::IProject;
using KDevelop::IPlugin;

class Ui_ProjectConfig;

namespace DragonScript {

class KDEVDSCONFIGPAGE_EXPORT ProjectConfigPage : public ConfigPage{
	Q_OBJECT
	
private:
	KConfigGroup pConfigGroup;
	Ui_ProjectConfig *pUI;
	IProject *pProject;
	QStringListModel *pModelPathInclude;
	
public:
	ProjectConfigPage( IPlugin *self, const KDevelop::ProjectConfigOptions &options, QWidget *parent );
	~ProjectConfigPage();
	
	QString name() const override;
	
public Q_SLOTS:
	void apply() override;
	void defaults() override;
	void reset() override;
	
	void editPathDSIChanged();
	
	void btnPathIncludeSelect();
	void btnPathIncludeAdd();
	void btnPathIncludeRemove();
};

}

#endif
