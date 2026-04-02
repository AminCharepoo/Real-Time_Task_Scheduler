## Real-Time Task Scheduler - Amin Charepoo, Dani Robinson, Manu Geronimo
This project is a custom real-time task scheduler on the Raspberry Pi Pico using C++.  
It manages multiple tasks with priorities and deadlines, can preempt tasks, and responds to hardware interrupts.  


## How to Build and Run
- Wire the LCD, Buttons, and LEDS to respective pins based on code definitions
- Hold BOOTSEL butotn on the Pico while conencting to USB
- Copy the compiled and generated .uf2 file to teh Pico drive 
- Restart power to the pico and it will automatically run the program

### OOP Design Summary
OOP Design Summary
Class Hierarchy
Base Class: Task

Properties: priority, name, interrupt_behavior
Provides common interface for all task types

Derived Classes:

ButtonTask

Handles GPIO interrupts - HIGH priority - Queues itself when button is pressed - takes in an action function as a constructor parameter which at the moment is turning on an LED.


TimerTask

Has a repeating_timer and self-enqueues every time the timer flags. - MEDIUM priority - takes in a function as a constructor parameter which at the moment is turning on and off an LED


DisplayTask

Manages LCD character-by-character displaying
Members: message, char_index, interval_ms, tick_flag, vip_flag - Has a timer to periodically increase the size of the substring of the message it is displaying - LOW priority


VIPButtonTask

Specialized button task for demonstrating VIP interrupts - VIP priority



Core Data Structures
Queue (Circular Buffer)

Scans queue every loop to check which task has the highest priority -> dequeues the item, executes its action, and checks if it got interrupted.


Design Patterns

Queue passed to task constructors so they can queue themselves
ISR Flag Pattern: Volatile boolean flags for interrupt signaling

### File Structure

```
Real-Time_Task_Scheduler/
│
├── .vscode/
│   └── c_cpp_properties.json
│   └── cmake-kits.json
│   └── extensions.json
│   └── launch.json
│   └── settings.json
│   └── tasks.json
├── Practice/
│   └── Interupt
│   └── SerialMonitor
│   └── TaskClass
│   └── blink
├── README.md
├── docs/
│   └── ECEE2140 - Iteration2.pdf
│   └── Class Interaction.pdf
│   └── ProjectDesignOverview.tex
│   └── System_Design_Overview.pdf
├── images/
│   └── system_diagram.png
├── include/
│   └── ButtonTask.h
│   └── DisplayTask.h
│   └── Queue.h
│   └── Task.h
│   └── TimerTask.h
│   └── types.h
│   └── utils.h
├── pseudocode/
│   └── PseudoCode.pdf
│   └── PsuedoText.tex
├── src/
│   └── BusttonTask.cpp
│   └── DisplayTask.cpp
│   └── Queue.cpp
│   └── Task.cpp
│   └── TimerTask.cpp
│   └── utils.cpp
├── .gitignore
├── CMakeLists.txt
├── README.md
├── main.cpp
├── pico_sdk_import.cmake
└── system_diagram.png
```
### Project Goals
Create a Real-Time Task Scheduler that works on a queueing system and accepts interrupts. There are three types of tasks with three different priority levels (HIGH, LOW, VIP). VIP tasks interrupt currently running tasks and take priority. Our Real-Time Task Scheduler is coded in C++, and for easy visualization and consistency, we use it with a Raspberry Pi Pico. This Raspberry Pi Pico has 2 LEDs, 1 button, and 1 LCD screen. 

## Main Functionalities
1. Priority based task scheduling 
- Circular buffer queue with priority dequeuing
- 3 Priority levels: HIGH, MEDIUM, LOW
- Special priority: VIP
2. Multiple Task Types
- Button Task: GPIO interrupt
- Timer Task: Periodic timer-based task
- Display Task: LCD Character by Character display
3. VIP Interrupts
- A VIP task being queued stops a display task
4. Hardware Interface
- GPIO button interrupts for user input
- 16x2 LCD Display 
- LED status indicators

## How to Build and Run
- Wire the LCD, Buttons, and LEDS to respective pins based on code definitions
- Hold BOOTSEL butotn on the Pico while conencting to USB
- Copy the compiled and generated .uf2 file to teh Pico drive 
- Restart power to the pico and it will automatically run the program


## Tools and Technologies
### Hardware
- Microcontroller: Raspberry Pi Pico
- 16x2 LCD Display
- GPIO Button
- LEDs

### Software 
- Language: C++ 
- SDK: Raspberry Pi Pico SDK
- Libraries 
    - hardware/gpio.h
    - hardware/timer.h
    - hardware/sync.h
    - pico_stdlib

### What has been implemented so far
- Core Scheduler System
    - Queue class with circular buffer
    - Priority scanning 
    - Interrupt aware and able to resume tasks interrupted. 
- Task System
    - Base Task class
    - 3 Children: ButtonTask for input queueing, TimerTask for periodic queueing, and DisplayTask to showcase VIP interrupts. 
- Interrupt Handling
    - Track VIP pending with a flag
    - Main loop restores interrupted tasks