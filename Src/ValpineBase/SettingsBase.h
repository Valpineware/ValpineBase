//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_SettingsBase_h
#define _ValpineBase_SettingsBase_h

#include <QtCore/QObject>
#include <QtCore/QVariant>

#include <ValpineBase/ValpineBase.h>

namespace vbase {

class SettingsBase : public QObject
{
	Q_OBJECT

public:
	/**
	  * This is a hack for access in QML. This method cannot be pure-virtual
	  * because then Qt will complain when registering the type.
	  */
	Q_INVOKABLE virtual void setValue(int key,
									  const QVariant &value)
	{
		Q_UNUSED(key)
		Q_UNUSED(value);
		errorAboutCallingVirtualMethodDirectly();
	}

	/**
	  * This is a hack for access in QML. This method cannot be pure-virtual
	  * because then Qt will complain when registering the type.
	  */
	Q_INVOKABLE virtual QVariant value(int key)
	{
		Q_UNUSED(key);
		errorAboutCallingVirtualMethodDirectly();

		return QVariant();
	}


	struct MetaKeyInfo
	{
		QVariant defaultValue;
	};

protected:
	void emitValueChanged(int key, const QVariant &newValue)
	{
		emit valueChanged(key, newValue);
	}

private:
	void errorAboutCallingVirtualMethodDirectly() const
	{
		qFatal("This method must be overriden and not called in a subclass");
	}

signals:
	void valueChanged(int key, const QVariant &newValue);
};

END_NAMESPACE

#endif
