//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#include "SingleInstance.h"

namespace vbase
{
	SingleInstance::SingleInstance(QObject *parent) : QObject(parent)
    {
		connect(&mServer, SIGNAL(newConnection()),
				this, SLOT(newConnection()));
    }


    SingleInstance::~SingleInstance()
    {
    }


    void SingleInstance::listen(const QString &name)
    {
		mServer.removeServer(name);
		mServer.listen(name);

		qDebug() << "Listening for " << name;
		qDebug() << mServer.errorString();
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
			buffer.append(item);

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

		mSocket = mServer.nextPendingConnection();
		connect(mSocket, SIGNAL(readyRead()),
				this, SLOT(readyRead()));
    }


    void SingleInstance::readyRead()
    {
		qDebug() << "args=" << mSocket->readAll();

		mSocket->deleteLater();
    }
};
