
#include "42-Prague-Smart-Sign.h"

#if !defined(XIAO_ESP32C3)
    static bool ft_result_check(const String address, int32_t result, int32_t old_max, int32_t old_min)
    {
        if (address == MAX_STATE)
        {
            if (result > old_max + 200 || result < old_max - 200)
                return (false);
            else
                return (true); 
        }
        if (address == MIN_STATE)
        {
            if (result > old_min + 200 || result < old_min - 200 || result >= old_max)        // during wired software update ESP32 may detect a Brown-out even when connected to a charged battery...
                return (false);                                                               // ...as a result the charged battery value may get written into the discharged battery position 
            else
                return (true); 
        }
        return (false);                                                                       // the program will never reach here. It is only to make compiler happy
    }

    void IRAM_ATTR ft_battery_state(const String address)                                     // Make a record of the lowest or highest battery reading
    {
        int32_t battery;
        int32_t result;
        int32_t old_max;
        int32_t old_min;  

        if (!rtc_g.spiffs_state)
            return;
        old_max = ft_read_spiffs_file(MAX_STATE).toInt();
        old_min = ft_read_spiffs_file(MIN_STATE).toInt();
        #ifdef DEBUG
            if (address == MAX_STATE)
            {
                DEBUG_PRINTF("\n\n======  EEPROM Test  ======\n", "");
                DEBUG_PRINTF("\n current data in the memory:\n", "");
                DEBUG_PRINTF("  - battery Max state %d\n", old_max);
                DEBUG_PRINTF("  - battery Min state %d\n", old_min);
                DEBUG_PRINTF("\n===========================\n", "");
            }
        #endif
        battery = ft_read_spiffs_file(address.c_str()).toInt();                               // if the read chunk of EEPROM memory was completely empty it returns the value of 0xFFFF
        if (address == MAX_STATE && ft_read_spiffs_file(MAX_STATE).isEmpty())
            battery = BATTERY_DEFAULT_MAX;
        if (address == MIN_STATE && ft_read_spiffs_file(MIN_STATE).isEmpty())
            battery = BATTERY_DEFAULT_MIN;
        result = ceil((adc1_get_raw(ADC1_CHANNEL_0) + battery) / 2);
        DEBUG_PRINTF("\nBattery measurement result: %d\n", result);
        if (!ft_result_check(address, result, old_max, old_min))
        {
            DEBUG_PRINTF(" -- The system consideres this result UNacceptable\n\n", "");
            return;
        }
        DEBUG_PRINTF(" -- The system consideres this result acceptable\n\n", "");
        ft_write_spiffs_file(address.c_str(), String(result));
    }

    short  ft_battery_check(void)                                                           // Find out the battery charge
    {
        short   i;
        int32_t min;
        float   coeff;

        i = 0;
        if (!rtc_g.spiffs_state)                                            // if unable to read accurate constants values, use default ones
        {
            min = BATTERY_DEFAULT_MIN;
            coeff = BATTERY_DEFAULT_COEFF;
        }
        else                                                                // if able to read, get the accurate constant values
        {
            min = ft_read_spiffs_file(MIN_STATE).toInt();
            coeff = (ft_read_spiffs_file(MAX_STATE).toInt() - min) / 100;
        }
        while (i < 10)                                                        // limit of number of battery readings, in this case 10 
        {
            g.battery += ceil((adc1_get_raw(ADC1_CHANNEL_0) - min) / coeff);
            i++;
        }
        g.battery = g.battery / i;                                            // counting average of i samples
        if (g.battery <= 0)                                                   // normalize the reading result if it turned out to be out of range
            g.battery = 0;
        if (g.battery >= 100)
            g.battery = 100;
        return (g.battery);
    }

    void  ft_battery_init(void)                                               // Configure ADC module for the future battery charge measurements
    {
        # if defined(ESP32_S2)
            adc1_config_width(ADC_WIDTH_BIT_13);
        # else // ESP32 || ESP_C3
            adc1_config_width(ADC_WIDTH_BIT_12);
        # endif
        adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_11db);
    }
#else // XIAO_ESP32C3

#endif
 
