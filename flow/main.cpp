
#include <Arduino.h>
#include <Flow.h>

#include <unistd.h>

int main(int argc, const char *argv[])
{
#if YYDEBUG
    extern int yydebug;
    yydebug = 0;
#endif
    Flow.begin();

    for (;;) {
        Flow.loop();

        usleep(10000); // 100 Hz
    }
}

