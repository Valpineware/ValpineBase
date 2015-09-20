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

class Settings : public QObject
{
	Q_OBJECT

public:
	//TODO consider allowing people to inject their own QSettings
	//then this class could be used inline with existing applications
	//using QSettings
	bool load(const QString &filepath)
	{
		mSettings = new QSettings(filepath, QSettings::IniFormat);

		return true;
	}


	void setValue(const QString &key, const QVariant &value)
	{
		mSettings->setValue(key, value);
		mSettings->sync();

		emit valueChanged(key, value);
	}


	QVariant value(const QString &key) const
	{
		return mSettings->value(key);
	}

signals:
	void valueChanged(const QString &key, const QVariant &value);


private:
	QSettings *mSettings = nullptr;

};
}

#endif
