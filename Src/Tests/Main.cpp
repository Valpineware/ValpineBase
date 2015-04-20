#include <ValpineBase/Test/Test.h>

bool ValpineBase::Test::Assert::mFatalOnAssert = true;

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
