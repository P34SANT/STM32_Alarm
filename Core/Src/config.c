#include "config.h"

#define CONFIG_MAGIC            0x43464731UL   // "CFG1"
#define CONFIG_VERSION          1U
#define PHONE_DIGITS            10U
#define PHONE_STR_LEN           (PHONE_DIGITS + 1U)   // + '\0'
#define MAX_OWNER_NUMBERS       5U


uint16_t debounce_time_ms = 200;
uint16_t poll_interval_ms = 1000;


uint8_t f_auto_shutdown      = 1;
uint8_t f_siren_enabled      = 1;
uint8_t auto_shutdown_second = 5;

//config struct for later on to write into flash. rn we use normal vars
typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t length;

    uint8_t  arm_state;
    uint8_t  sensor1_enabled;
    uint8_t  sensor2_enabled;
    uint8_t  sensor3_enabled;
    uint8_t  siren_enabled;

    uint8_t  boot_grace_s;
    uint16_t sensor_grace_ms;
    uint16_t sensor_check_ms;
    uint16_t reserved0;

    char     call_number[PHONE_STR_LEN];
    char     owner_numbers[MAX_OWNER_NUMBERS][PHONE_STR_LEN];

    uint32_t crc32;
    
} Config_t;
