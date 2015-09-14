#include <QtCore/QString>
#include <qqml.h>
#include <QtQml/QQmlEngine>

class ValpineBase
{
public:
    static QString version();
    static void registerQmlModule(QQmlEngine *engine)
    {
        Q_INIT_RESOURCE(ValpineBase);

        if (engine)
        {
            engine->addImportPath("qrc:/import");
        }
    }

};
