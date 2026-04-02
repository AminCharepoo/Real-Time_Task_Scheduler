## Real-Time Task Scheduler - Amin Charepoo, Dani Robinson, Manu Geronimo
This project is a custom real-time task scheduler on the Raspberry Pi Pico using C++.  
It manages multiple tasks with priorities and deadlines, can preempt tasks, and responds to hardware interrupts.  


### Run command:

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
Create a Real-Time Task Scheduler that works on a queueing system and accepts interrupts. Interrupts can be VIP tasks that run before a normal task. Our Real-Time Task Scheduler will be coded in C++, and for easy visualization and consistency, we use it with a Raspberry Pi Pico. This Raspberry Pi Pico has 2 LEDs, 1 button, and 1 LCD screen. 
