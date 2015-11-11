//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef _vbase_SingleInstance_h
#define _vbase_SingleInstance_h

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtNetwork/QLocalSocket>
#include <QtNetwork/QLocalServer>

#include <ValpineBase/ValpineBase.h>

namespace vbase {

class SingleInstance : public QObject
{
	Q_OBJECT

public:
	SingleInstance(QObject *parent=nullptr);
	~SingleInstance();

	void listen(const QString &name);
	bool hasPrevious(const QString &name, const QStringList &args);

signals:
	void newInstance();
	void receivedArguments(const QVariantList &arguments);

public slots:
	void newConnection();
	void readyRead();

private:
	QLocalSocket *_socket = nullptr;
	QLocalServer _server;
};

END_NAMESPACE

#endif

