#include <QtCore/QString>
#include <qqml.h>
#include <QtQml/QQmlEngine>

namespace vbase
{
	class ValpineBase
	{
	public:
		static QString version();
		static void registerQmlModule(QQmlEngine *engine);
	};
}
