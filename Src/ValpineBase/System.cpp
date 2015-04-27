//==================================================================================================================|
// Created 2015.04.26 by Daniel L. Watkins
//
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//==================================================================================================================|

#include "System.h"

namespace ValpineBase
{
	void System::warning(const QString &message)
	{
		qWarning("%s", message.toStdString().c_str());
	}


	void System::fatal(const QString &message)
	{
		qFatal("%s", message.toStdString().c_str());
	}
}
