#include "ValpineBase.h"

static void initResources()
{
	Q_INIT_RESOURCE(ValpineBase);
}


namespace vbase
{
	QString ValpineBase::version()
	{
		static const QString versionStr = "5.6.2";

		return versionStr;
	}


	void ValpineBase::registerQmlModule(QQmlEngine*engine)
	{
		initResources();

		if (engine)
		{
			engine->addImportPath("qrc:/import");
		}
	}
}
