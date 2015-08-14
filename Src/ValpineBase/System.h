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
        static void warning(const QString &message)
        {
            qWarning("%s", message.toStdString().c_str());
        }

        static void fatal(const QString &message)
        {
            qFatal("%s", message.toStdString().c_str());
        }
    };
}

#endif
