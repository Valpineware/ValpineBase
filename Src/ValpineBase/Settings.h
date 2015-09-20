//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _ValpineBase_Settings_h
#define _ValpineBase_Settings_h

#include <QtCore/QDebug>
#include <QtCore/QIODevice>
#include <QtCore/QSettings>
#include <QtCore/QVariant>

namespace vbase {

class Settings
{
public:
	bool load(const QString &filepath)
	{
		mSettings = new QSettings(filepath, QSettings::IniFormat);

		return true;
	}


	void setValue(const QString &key, const QVariant &value)
	{
		mSettings->setValue(key, value);
		mSettings->sync();
	}


	QVariant value(const QString &key) const
	{
		return mSettings->value(key);
	}


private:
	QSettings *mSettings = nullptr;

};
}

#endif
