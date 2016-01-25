#include "server.h"

int main() {
    TcpServer s(8812);
    s.start();
    int a;
    scanf("%d", &a);
    s.stop();
    return 0;
}
