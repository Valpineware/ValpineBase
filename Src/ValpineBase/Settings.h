//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Settings_h
#define _ValpineBase_Settings_h

#include <type_traits>

#include <QtCore/QDebug>
#include <QtCore/QIODevice>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

#include <ValpineBase/SettingsBase.h>

namespace vbase {

template<class KeyClass, typename KeyType = typename KeyClass::KeyEnum>
class Settings : public SettingsBase
{
public:
	bool load(const QString &fileName)
	{
		mSettings = new QSettings(fileName, QSettings::IniFormat);

		return true;
	}


	virtual void setValue(int key, const QVariant &value) override
	{
		setValue(static_cast<KeyType>(key), value);
	}


	void setValue(KeyType key, const QVariant &newValue)
	{


		if (value(key) != newValue)
		{
			QString name = stringNameForKey(key);
			qDebug() << "Settings value changed for key=" << name
					 << " value=" << newValue.toString();

			mSettings->setValue(name, newValue);
			mSettings->sync();
			SettingsBase::emitValueChanged(static_cast<int>(key), newValue);
		}
	}


	virtual QVariant value(int key) override
	{
		return value(static_cast<KeyType>(key));
	}


	QVariant value(KeyType key)
	{
		const auto &name = stringNameForKey(key);

		QVariant value = mSettings->value(name);

		if (QString(value.typeName()) == "QString" &&
			(value.toString() == "false" || value.toString() == "true"))
			return QVariant(value.toBool());

		return value;
	}

private:
	QString stringNameForKey(KeyType key)
	{
		const QMetaObject &mo = KeyClass().staticMetaObject;
		QMetaEnum me = mo.enumerator(mo.indexOfEnumerator("Key"));
		return me.valueToKey(key);
	}

private:
	QSettings *mSettings;
	QList<QPair<KeyType, QVariant>> mSettingsQueue;
};

}

#endif
