//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include "System.h"

namespace vbase {

QDebug System::warn(bool quote, bool space)
{
	QDebug d = qDebug();

	if (!quote)
		d.noquote();

	if (!space)
		d.nospace();

	return d;
}


void System::fatal(const QString &message)
{
	qFatal("%s", message.toStdString().c_str());
}

END_NAMESPACE
