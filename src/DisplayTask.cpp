#include "DisplayTask.h"

DisplayTask::DisplayTask(
        std::string name,
        priority_level priority, 
        interrupt_behavior behavior, 
        const std::string& msg, 
        uint32_t interval_ms, 
        volatile bool* vip_flag,
        Queue& q
    ) : Task(name, priority, behavior, q),
      message(msg),
      char_index(0),
      interval_ms(interval_ms),
      tick_flag(false),
      vip_flag(vip_flag) {
}


DisplayTask::~DisplayTask() {
    cancel_repeating_timer(&timer);
}

// Toggles the enable pin of the LCD to latch data/commands.
void DisplayTask::lcd_toggle_enable(uint8_t data) {
    uint8_t buf;
    buf = data | 0x04;
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &buf, 1, false);
    sleep_us(1);
    buf = data & ~0x04;
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &buf, 1, false);
    sleep_us(50);
}

// Sends a byte to the LCD in 4-bit mode by splitting it into two nibbles.
void DisplayTask::lcd_send_byte(uint8_t val, int mode) {
    uint8_t high = (val & 0xF0) | mode | 0x08;
    uint8_t low  = ((val << 4) & 0xF0) | mode | 0x08;
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &high, 1, false);
    lcd_toggle_enable(high);
    i2c_write_blocking(I2C_PORT, LCD_ADDR, &low, 1, false);
    lcd_toggle_enable(low);
}


// Sends a command byte to the LCD
void DisplayTask::lcd_cmd(uint8_t command) {
    lcd_send_byte(command, 0);
}


// Sends a single character to the LCD for display.
void DisplayTask::lcd_char(char c) {
    lcd_send_byte(c, 1);
}

// Displays a string on the LCD by sending each character.
void DisplayTask::lcd_string(const char* str) {
    while(*str) {
        lcd_char(*str++);
    }
}

// Sets the LCD cursor to the beginning of line 0 (top) or line 1 (bottom).
void DisplayTask::lcd_set_cursor(int line) {
    if (line == 0) {
        lcd_cmd(0x80);
    } else {
        lcd_cmd(0xC0);
    }
}

// Clears the LCD display and resets the cursor to home position.
void DisplayTask::lcd_clear() {
    lcd_cmd(0x01);
    sleep_ms(2);
}

// Initializes the LCD in 4-bit mode with 2 lines, cursor off, and backlight on.
void DisplayTask::lcd_init() {
    lcd_cmd(0x33);
    lcd_cmd(0x32);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_clear();
}

// Initializes I2C communication, configures GPIO pins with pull-ups, initializes the LCD, and starts the repeating timer for character scrolling.
void DisplayTask::setup() {
    // Initialize I2C
    i2c_init(I2C_PORT, 100000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    sleep_ms(50);
    
    // Initialize LCD
    lcd_init();
    
    // Start timer
    add_repeating_timer_ms(interval_ms, timer_callback, this, &timer);
}

// Timer ISR that sets tick_flag to signal the main loop that it's time to display the next character. Returns true to keep the timer repeating.
bool DisplayTask::timer_callback(struct repeating_timer *t) {
    DisplayTask* task = static_cast<DisplayTask*>(t->user_data);
    task->tick_flag = true;
    return true;
}

void DisplayTask::execute() {
    // Check VIP preemption
    if (*vip_flag) {
        return;
    }
    
    // Check if it's time to update
    if (!tick_flag) {
        return;
    }
    tick_flag = false;
    
    // increment char index
    char_index++;
    if (char_index > message.length()) {
        char_index = 0;
    }
    
    // Get substring (0 to char_index)
    std::string display_str = message.substr(0, char_index);
    
    // Display substring
    lcd_clear();
    lcd_set_cursor(0);
    lcd_string(display_str.c_str());
}

DisplayTask::setMessage(const std::string& msg) {
    message = msg;
    char_index = 0; // reset index to start of new message
}