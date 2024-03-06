
#include "42-Prague-Smart-Sign.h"

void IRAM_ATTR  ft_eeprom_init(void)
{
    RTC_DATA_ATTR static short i = 1;
    
    if (!EEPROM.begin(EEPROM_SIZE))
    {
        if (i <= 2)
        {
            i++;
            DEBUG_PRINTF("\nFailed to initialise EEPROM. Restarting %d...\n", i);
            ft_delay(500);
            ESP.restart();
        }
    }
    else
    {
        i = 1;
        DEBUG_PRINTF("\nEEPROM is successfully initialised.\n", "");
        rtc_g.eeprom_state = true;
        return;
    }
    i = 1;
    DEBUG_PRINTF("\nEEPROM was not initialised. Reading and Writing data is unavailable this session.\n", "");
    rtc_g.eeprom_state = false;
}

static bool ft_result_chech(const int address, uint16_t result, uint16_t old_max, uint16_t old_min)
{
    if (address == MAX_STATE_ADDR)
    {
        if (result > old_max + 200 || result < old_max - 200)
            return (false);
        else
            return (true); 
    }
    if (address == MIN_STATE_ADDR)
    {
        if (result > old_min + 200 || result < old_min - 200)
            return (false);
        else
            return (true); 
    }
}

void IRAM_ATTR ft_battery_state(const int address)
{
    uint16_t battery;
    uint16_t result;
    uint16_t old_max;
    uint16_t old_min;  

    if (!rtc_g.eeprom_state)
        return;
    old_max = EEPROM.readShort(MAX_STATE_ADDR);
    old_min = EEPROM.readShort(MIN_STATE_ADDR);
    #ifdef DEBUG
        if (address == MAX_STATE_ADDR)
        {
            DEBUG_PRINTF("\n======  EEPROM Test  ======\n", "");
            DEBUG_PRINTF("\n current data in the memory:\n", "");
            DEBUG_PRINTF("  - battery Max state %d\n", old_max);
            DEBUG_PRINTF("  - battery Min state %d\n", old_min);
            DEBUG_PRINTF("\n===========================\n", "");
        }
    #endif
    battery = EEPROM.readShort(address);
    result = ceil((adc1_get_raw(ADC1_CHANNEL_0) + battery) / 2);
    if (!ft_result_chech(address, result, old_max, old_min))
        return;
    EEPROM.writeShort(address, result);
    EEPROM.commit();
}

short  ft_battery_check(void)
{
    short i;
    short min;
    float coeff;

    i = 0;
    if (!rtc_g.eeprom_state)
    {
        min = BATTERY_DEFAULT_MIN;
        coeff = BATTERY_DEFAULT_COEFF;
    }
    else
    {
        min = EEPROM.readShort(MIN_STATE_ADDR);
        coeff = (EEPROM.readShort(MAX_STATE_ADDR) - min) / 100;
    }
    while (i < 10)
    {
        g.battery += ceil((adc1_get_raw(ADC1_CHANNEL_0) - min) / coeff);
        i++;
    }
    g.battery = g.battery / i;                                            // counting average of i samples
    if (g.battery <= 0)
        g.battery = 0;
    if (g.battery >= 100)
        g.battery = 100;
    return (g.battery);
}

void IRAM_ATTR ft_battery_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_13);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
}
 
