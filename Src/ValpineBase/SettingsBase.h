//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_SettingsBase_h
#define _ValpineBase_SettingsBase_h

#include <QtCore/QObject>
#include <QtCore/QVariant>

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
		qFatal("This method must be overriden and not called in a subclass");
	}

	/**
	  * This is a hack for access in QML. This method cannot be pure-virtual
	  * because then Qt will complain when registering the type.
	  */
	Q_INVOKABLE virtual QVariant value(int key)
	{
		qFatal("This method must be overriden and not called in a subclass");

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

signals:
	void valueChanged(int key, const QVariant &newValue);
};

}

#endif
