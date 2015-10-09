//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Log_h
#define _ValpineBase_Log_h

#include <QtCore/QDebug>

#include <ValpineBase/ValpineBase.h>

namespace vbase {

class System
{
public:
	static QDebug warn(bool quote=false, bool space=false);
	static void fatal(const QString &message);
};

END_NAMESPACE

#endif
