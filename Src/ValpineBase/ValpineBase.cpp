//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtQml>

#include "ValpineBase.h"
#include "SettingsBase.h"

static void initResources()
{
	Q_INIT_RESOURCE(ValpineBase);
}


namespace vbase {

QString ValpineBase::version()
{
	static const QString versionStr = "0.1.0";

	return versionStr;
}


void ValpineBase::registerQmlModule(QQmlEngine*engine)
{
	initResources();

	if (engine)
	{
		engine->addImportPath("qrc:/import");

		qmlRegisterType<vbase::SettingsBase>("ValpineBase", 1, 0, "Settings");
	}
}

END_NAMESPACE
