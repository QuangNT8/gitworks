#ifndef CONFIG_H
#define CONFIG_H

/*server info*/
#define SERVER_IP               "192.168.2.90"
//#define KIT_IP                  "127.0.0.1"
//#define SERVER_IP               "192.168.2.90" // machine raspberry
//#define KIT_IP                  "192.168.2.190" // machine raspberry test-kit
#define KIT_IP                  "192.168.2.174" // machine test-kit
#define SERVER_PORT             5678
#define EVENT_CACHE_SIZE        1000
#define DEFAULT_EVENT_COUNT     50
#define TANK_COUNT 8

#define MAX_FILE_SIZE           1024 * 1024 * 1024 //in Bytes
#define TEMPORARY_PATH          "files"
#define FILE_SCHEME             "file://"

#define TIMER_CYCLE             20  //ms

#define LINE_LENGTH             (((740 + 50) * 2 + 420 * 3) * 1024)
#define MIN_DISTANCE_TWO_ROBOT  (400*1024)
#define MAX_POSITION_ROBOT      (((740 + 50) + 420 * 3) * 1024)
#endif // CONFIG_H
