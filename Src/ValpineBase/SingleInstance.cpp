//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include "SingleInstance.h"

namespace vbase {

SingleInstance::SingleInstance(QObject *parent) : QObject(parent)
{
	connect(&_server, SIGNAL(newConnection()),
			this, SLOT(newConnection()));
}


SingleInstance::~SingleInstance()
{
}


void SingleInstance::listen(const QString &name)
{
	_server.removeServer(name);
	if (!_server.listen(name))
		qDebug() << "Unable to listen";

	qDebug() << "Listening for " << name;
	qDebug() << _server.errorString();
}


bool SingleInstance::hasPrevious(const QString &name, const QStringList &args)
{
	qDebug() << "Checking for previous instance...";

	QLocalSocket socket;
	socket.connectToServer(name, QLocalSocket::ReadWrite);

	if (socket.waitForConnected())
	{
		qDebug() << "Connection found!";
		qDebug() << "Forwarding argument to existing instance...";

		QByteArray buffer;
		for (auto item : args)
		{
			buffer.append(item+"\n");
		}

		qDebug() << "Forwading buffer=" << buffer;

		socket.write(buffer);

		return true;
	}

	qDebug() << socket.errorString();
	qDebug() << "No connection found";

	return false;
}


void SingleInstance::newConnection()
{
	emit newInstance();

	_socket = _server.nextPendingConnection();
	connect(_socket, SIGNAL(readyRead()),
			this, SLOT(readyRead()));
}


void SingleInstance::readyRead()
{
	QString argsString = _socket->readAll();

	QVariantList argsList;
	for (auto line : argsString.split("\n"))
	{
		if (line != "")
			argsList.append(line);
	}

	qDebug() << "Received args:";

	for (auto line : argsList)
		qDebug() << "  " << line;

	emit receivedArguments(argsList);

	_socket->deleteLater();
}

END_NAMESPACE
