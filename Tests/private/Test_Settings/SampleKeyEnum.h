#ifndef _private_Test_Settings_SampleKeyEnum_h
#define _private_Test_Settings_SampleKeyEnum_h

#include <QtCore/QObject>

class SampleKeyClass : public QObject
{
	Q_OBJECT

public:
	enum KeyEnum
	{
		GraphicsWindowWidth				= 1000,
		GraphicsWindowHeight			= 1001,
		GraphicsWindowIsFullscreen		= 1002,
		GraphicsWindowBackgroundColor	= 1003
	};

	Q_ENUMS(SampleKeyEnum)
};

#endif
