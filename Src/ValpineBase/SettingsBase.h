//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include <QtCore/QObject>

namespace vbase {

class SettingsBase : public QObject
{
	Q_OBJECT

protected:
	void emitValueChanged(int key, const QVariant &newValue)
	{
		emit valueChanged(key, newValue);
	}

signals:
	void valueChanged(int key, const QVariant &newValue);
};

}
