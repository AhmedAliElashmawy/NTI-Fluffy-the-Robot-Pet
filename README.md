# Project 4 — Fluffy the Robot Pet

**Course:** NTI Microcontroller-Based Systems (Plain C Practice)  
**Language/Standard:** C99 (`-std=c99 -Wall -Wextra -pedantic`)

---

## Overview

**Fluffy the Robot Pet** simulates a virtual digital companion with finite state machine dynamics and bitwise mood management. Users care for Fluffy by feeding, playing, managing sleep schedules, and tracking hourly lifecycle decay across critical survival metrics (Food, Fun, and Energy).

---

## Build & Run Instructions

> [!WARNING]
> The provided `Makefile` and clean commands are designed for **Linux** environments (using `rm -f app`). On Windows, compile directly with GCC or adapt the Makefile clean command (e.g. `del app.exe`).

### Prerequisites
- GCC or any C99-compliant C compiler (Linux environment recommended).

### Compilation
Compile with strict warning flags:
```bash
gcc -std=c99 -Wall -Wextra -pedantic -o app main.c
```
Or simply use `make` (Linux only):
```bash
make
```

### Execution
```bash
./app
```

### Cleaning
```bash
make clean
```

---

## Functions Implemented

All functions are implemented as `static` functions under 40 lines:

| Function | Description |
|---|---|
| `hatchPet()` | Spawns a new pet instance, initializes metrics to 10/10, clears mood bits, and zeroes the age counter. |
| `drawFace()` | Renders an ASCII expression box matching Fluffy's priority psychological state (Asleep, Sick, Sad/Hungry, or Happy). |
| `drawStat()` | Renders a normalized 10-segment visual progress bar for Food, Fun, or Energy metrics. |
| `updateMood()` | Updates status bits (`BIT_HUNGRY`, `BIT_SAD`, `BIT_ASLEEP`) using explicit symmetric `if`/`else` rules. |
| `feed()` | Nourishes the pet (+3 Food). Overfeeding when food is full (10) induces sickness (`BIT_SICK`). Refused while sleeping. |
| `play()` | Engages the pet in play (+3 Fun, -2 Energy). Refused while sleeping. |
| `sleepPet()` | Toggles sleep state. Waking up from sleep cures any active sickness. |
| `hourPasses()` | Simulates 1 hour of time: decrements food and fun; regenerates energy by +2 if asleep or consumes -1 if awake; increments age. |
| `isHappy()` | Evaluates whether all stats exceed the threshold (`LOW_WARNING = 3`) and no adverse mood flags are active. |
| `petReport()` | Displays the comprehensive care card with ASCII face, stat meters, binary mood byte, and actionable care diagnosis. |

---

## Robustness & Design Highlights

1. **Single-File Architecture:**
   - The entire implementation is contained in a single file (`main.c`) strictly adhering to project constraints (plain C only, `#include <stdio.h>`, `<stdint.h>`, `<string.h>` only).
2. **Bit Manipulation & Symmetric Invariants:**
   - Mood states are stored in a single byte and manipulated exclusively via bitwise macros (`SET_BIT`, `CLR_BIT`, `TOGGLE_BIT`, `READ_BIT`).
   - Every conditional that sets a transient mood bit contains an explicit `else` branch that clears it, preventing state lockups.
3. **Underflow & Overflow Protection:**
   - All stat decrements clamp cleanly at 0 without wrapping past unsigned bounds (no rollover to 255).
4. **Input Validation (`scanf` handling):**
   - All `scanf` calls are verified and invalid inputs are discarded safely via `clearInputBuffer()`.
5. **Terminal Flushing & Screen Refresh:**
   - Uses ANSI escape codes (`\033[H\033[J`) and immediate buffer flushes (`fflush(stdout)`) for responsive terminal interaction.
