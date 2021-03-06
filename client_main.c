#include <stdlib.h>

#include "newnode.h"
#include "network.h"
#include "thread.h"
#include "log.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif


int main(int argc, char *argv[])
{
    char *port_s = "8006";

    for (;;) {
        int c = getopt(argc, argv, "p:v");
        if (c == -1) {
            break;
        }
        switch (c) {
        case 'p':
            port_s = optarg;
            break;
        case 'v':
            o_debug++;
            break;
        default:
            die("Unhandled argument: %c\n", c);
        }
    }

    port_t http_port = atoi(port_s);
    port_t socks_port = http_port + 1;
    network *n = newnode_init("client", "com.newnode.client", &http_port, &socks_port, ^(const char *url, https_complete_callback cb) {
        debug("https: %s\n", url);
        cb(true);
    });
    if (!n) {
        return 1;
    }

#ifdef __APPLE__
    newnode_thread(n);
    CFRunLoopRun();
    return 0;
#else
    return newnode_run(n);
#endif
}
