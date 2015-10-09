//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Settings_h
#define _ValpineBase_Settings_h

#include <type_traits>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QIODevice>
#include <QtCore/QSettings>
#include <QtCore/QVariant>
#include <QtCore/QMetaEnum>

#include <ValpineBase/SettingsBase.h>

namespace vbase {

template<class KeyClass, typename KeyType = typename KeyClass::KeyEnum>
class Settings : public SettingsBase
{
public:
	bool load(const QString &filePath)
	{
		mSettings = new QSettings(filePath, QSettings::IniFormat);
		mSettings->setValue("Sample", "100");
		mSettings->sync();

		qDebug() << "Loaded settings filePath of " << filePath;
		qDebug() << "Settings instance is " << mSettings->fileName();

		return true;
	}


	virtual void setValue(int key, const QVariant &value) override
	{
		setValue(static_cast<KeyType>(key), value);
	}


	void setValue(KeyType key, const QVariant &newValue)
	{
		auto v = value(key);
		if (v != newValue)
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

		if (!value.isValid())
		{
			return KeyClass::metaKeyInfoForKey(key).defaultValue;
		}
		else if (QString(value.typeName()) == "QString" &&
			(value.toString() == "false" || value.toString() == "true"))
			return QVariant(value.toBool());

		return value;
	}

private:
	QString stringNameForKey(KeyType key)
	{
		const QMetaObject &mo = KeyClass().staticMetaObject;
		int index = mo.indexOfEnumerator("KeyEnum");

		if (index >= 0)
			return mo.enumerator(index).valueToKey(key);

		return "";
	}

private:
	QSettings *mSettings;
	QList<QPair<KeyType, QVariant>> mSettingsQueue;
};

END_NAMESPACE

#endif
