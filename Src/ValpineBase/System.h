//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Log_h
#define _ValpineBase_Log_h

#include <QtCore/QDebug>

namespace vbase
{
    class System
    {
    public:
        static QDebug warn(bool quote=false, bool space=false);
        static void fatal(const QString &message);
    };
}

#endif
