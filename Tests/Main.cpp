#include <QProcess>

#include <ValpineBase/Test.h>
#include "Main.h"

#include "Test_Loadable.h"
#include "Test_Property.h"
#include "Test_Property_Operators.h"

int main(int argc, char *argv[])
{
    _private_Test::run(argc>0 ? 1 : 0, argv, true);

    return 0;
}
