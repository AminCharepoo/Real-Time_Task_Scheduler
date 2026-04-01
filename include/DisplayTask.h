#ifndef DISPLAYTASK_H
#define DISPLAYTASK_H

#include "Task.h"
#include <string>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c0
#define LCD_ADDR 0x3F
#define SDA_PIN 19
#define SCL_PIN 21

class DisplayTask : public Task {
private:
    // display state
    std::string message;
    unsigned int char_index;
    
    // timing
    uint32_t interval_ms;
    struct repeating_timer timer;
    volatile bool tick_flag;
    
    // VIP preemption
    volatile bool* vip_flag;
    
    // LCD low-level functions
    void lcd_toggle_enable(uint8_t data);
    void lcd_send_byte(uint8_t val, int mode);
    void lcd_cmd(uint8_t command);
    void lcd_char(char c);
    void lcd_string(const char* str);
    void lcd_set_cursor(int line);
    void lcd_clear();
    void lcd_init();
    
    // static ISR callback
    static bool timer_callback(struct repeating_timer *t);
    
public:
    DisplayTask(
        std::string name,
        priority_level priority, 
        interrupt_behavior behavior, 
        const std::string& msg, 
        uint32_t interval_ms, 
        volatile bool* vip_flag,
        Queue& q
    );

    ~DisplayTask();
    
    void setup();
    void execute() override;

    void setMessage(const std::string& msg);
};

#endif