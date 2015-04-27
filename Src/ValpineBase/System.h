//==================================================================================================================|
// Created 2015.04.26 by Daniel L. Watkins
//
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//==================================================================================================================|

#ifndef _ValpineBase_Log_h
#define _ValpineBase_Log_h

#include <QtCore/QDebug>

namespace ValpineBase
{
	class System
	{
	public:
		static void warning(const QString &message);
		static void fatal(const QString &message);
	};
}

#endif
