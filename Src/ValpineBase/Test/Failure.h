//=============================================================================|
// Copyright (C) 2015 Valpineware
// This file is licensed under the MIT License.
//=============================================================================|

#ifndef vbase_test_Message_h
#define vbase_test_Message_h

#include <exception>
#include <memory>

#include <QString>
#include <QStringList>
#include <QMetaMethod>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>

#include <ValpineBase/ValpineBase.h>

namespace vbase { namespace test {

class Failure
{
public:
	QStringList details;
	QString filePath;
	int lineNumber = -1;

	enum class Type
	{
		Warn = 0,
		Error = 1
	} type = Type::Error;

	QString typeName() const
	{
		switch (type)
		{
		case Type::Warn:
			return "warning";
		case Type::Error:
			return "error";
		}

		return "error";
	}


	QJsonObject toJsonObject() const
	{
		QJsonObject jsonObject;
		jsonObject.insert("filePath", filePath);
		jsonObject.insert("lineNumber", lineNumber);
		jsonObject.insert("type", static_cast<int>(type));

		QJsonArray messageArray;
		for (const auto &message : details)
			messageArray.append(message);

		jsonObject.insert("details", messageArray);

		return jsonObject;
	}
};


/**
 * Indicates a test failure due to an assert being triggered.
 */
class TestAssertException {};

END_NAMESPACE
END_NAMESPACE

#endif
