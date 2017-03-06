

#include <CppUnitTest.h>

int Assert::count;
int Assert::failCount;

int main(int argc, const char *argv[])
{
    if (Assert::failCount > 0) {
        printf("%d %s out of %d asserts\n",
            Assert::failCount, Assert::failCount==1?"failure":"failures", Assert::count);
        return 1;
    }
    else {
        printf("OK!\n");
        return 0;
    }
}
