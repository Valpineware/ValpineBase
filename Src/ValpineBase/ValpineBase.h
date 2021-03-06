//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef ValpineBase_h
#define ValpineBase_h

#include <memory>

#include <QtCore/QString>
#include <QtQml/QQmlEngine>

#include <ValpineBase/ValpineBase.h>

#define BEGIN_NAMESPACE(ns) namespace ns {
#define END_NAMESPACE }

namespace vbase {

template<typename T>
using Shared = std::shared_ptr<T>;

template<typename T, typename ...Args>
Shared<T> makeShared(Args&& ...args)
{
	return Shared<T>(new T(std::forward<Args>(args)...));
}

template<typename T>
using Weak = std::weak_ptr<T>;

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename ...Args>
Unique<T> makeUnique(Args&& ...args)
{
	return Unique<T>(new T(std::forward<Args>(args)...));
}

class ValpineBase
{
	public:
	static QString version();
	static void registerQmlModule(QQmlEngine *engine);
};

END_NAMESPACE

#endif
