//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef ValpineBase_h
#define ValpineBase_h

#include <QtCore/QString>
#include <QtQml/QQmlEngine>

#include <ValpineBase/ValpineBase.h>

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE }

namespace vbase {

class ValpineBase
{
	public:
	static QString version();
	static void registerQmlModule(QQmlEngine *engine);
};

END_NAMESPACE

#endif
