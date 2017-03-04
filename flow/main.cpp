
#include <Arduino.h>
#include <Flow.h>

#include <unistd.h>

int main(int argc, const char *argv[])
{
    Flow.begin();

    for (;;) {
        Flow.loop();

        usleep(10000); // 100 Hz
    }
}

