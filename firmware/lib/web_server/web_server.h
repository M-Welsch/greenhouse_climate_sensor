#ifndef _web_server_h_
#define _web_server_h_

#include <ESP8266WebServer.h>
#include "status.h"

class WebServer {
        const char* ssid = "NETGEAR_Repeater";
        const char* password = "XL12ABZXYGKIDO";
        char *_ip_address;
    public:
        void handle_clients(void);
        void setup(Status *main_status);
        void connect(void);
        bool connected(void);
        void ip_address(char *buffer);
};

/* HTML Content */
const char HEAD[] = R"=====(
<!DOCTYPE html>
<title>Backofen &Uuml;berwachung</title>
<style>
    body {
        font-family:Arial, Helvetica, sans-serif;
        font-size: 14px;
    }
    table {
	    background-color:beige;
    }
</style>
<html>
<body>
)=====";

const char FOOT[] = R"=====(

</body>
</html>
)=====";

#endif