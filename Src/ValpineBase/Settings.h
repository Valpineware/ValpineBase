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
#include <QtCore/QList>

#include <ValpineBase/SettingsBase.h>

namespace vbase {

//TODO the user should not be able to supply their own KeyEnum
template<class KeyClass, typename KeyType = typename KeyClass::KeyEnum>
class Settings : public SettingsBase
{
	//TODO add some static_assert checks

public:
	bool load(const QString &filePath)
	{
		settings = new QSettings(filePath, QSettings::IniFormat);
		settings->setValue("Sample", "100");
		settings->sync();

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

			settings->setValue(name, newValue);
			settings->sync();
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

		QVariant value = settings->value(name);

		if (!value.isValid())
		{
			return KeyClass::metaKeyInfoForKey(key).defaultValue;
		}
		else if (QString(value.typeName()) == "QString" &&
			(value.toString() == "false" || value.toString() == "true"))
			return QVariant(value.toBool());

		return value;
	}


	void enqueueValue(KeyType key, const QVariant &newValue)
	{
		settingsQueue.append(QPair<KeyType,QVariant>(key, newValue));
	}


	void applyQueuedValues()
	{
		for (const QPair<KeyType,QVariant> &pair : settingsQueue)
			setValue(pair.first, pair.second);

		settingsQueue.clear();
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
	QSettings *settings;
	QList<QPair<KeyType, QVariant>> settingsQueue;
};

END_NAMESPACE

#endif
