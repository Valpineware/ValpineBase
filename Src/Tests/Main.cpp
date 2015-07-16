#include <QProcess>

#include <ValpineBase/Test/Test.h>
#include "Main.h"

#include "Test_Loadable.h"
#include "Test_Property.h"
#include "Test_Property_Operators.h"

int main(int argc, char *argv[])
{
    auto resultFilepath = _private_Test::run(argc, argv);

    QProcess::startDetached("QtTestReviewGUI", QStringList() << resultFilepath);

    return 0;
}
