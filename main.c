#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define STAT_MAX      10U
#define LOW_WARNING    3U
#define NAME_LEN      12U

#define BIT_ASLEEP     0U
#define BIT_HUNGRY     1U
#define BIT_SAD        2U
#define BIT_SICK       3U

#define SET_BIT(reg, n)    ((reg) |=  (uint8_t)(1U << (n)))
#define CLR_BIT(reg, n)    ((reg) &= (uint8_t)~(1U << (n)))
#define TOGGLE_BIT(reg, n) ((reg) ^=  (uint8_t)(1U << (n)))
#define READ_BIT(reg, n)   ((uint8_t)(((reg) >> (n)) & 1U))

typedef struct {
    char     name[NAME_LEN];
    uint8_t  food;
    uint8_t  fun;
    uint8_t  energy;
    uint8_t  mood;
    uint16_t hours;
} Pet_t;

static Pet_t fluffy;

static void    clearInputBuffer(void);
static void    clearScreen(void);
static void    pauseScreen(void);
static void    hatchPet(void);
static void    drawFace(void);
static void    drawStat(const char *label, uint8_t value);
static void    feed(void);
static void    play(void);
static void    sleepPet(void);
static void    hourPasses(void);
static void    updateMood(void);
static uint8_t isHappy(void);
static void    petReport(void);

static void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Discard */
    }
}

static void clearScreen(void) {
    printf("\033[H\033[J");
    fflush(stdout);
}

static void pauseScreen(void) {
    printf("\nPress Enter to return to menu...");
    fflush(stdout);
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* Discard */
    }
}

static void hatchPet(void) {
    strncpy(fluffy.name, "Fluffy", NAME_LEN - 1U);
    fluffy.name[NAME_LEN - 1U] = '\0';
    fluffy.food = STAT_MAX;
    fluffy.fun = STAT_MAX;
    fluffy.energy = STAT_MAX;
    fluffy.mood = 0U;
    fluffy.hours = 0U;
    printf("\n[HATCHED] %s has hatched into the world! Full of energy!\n", fluffy.name);
    fflush(stdout);
}

static uint8_t isHappy(void) {
    if (fluffy.food > LOW_WARNING &&
        fluffy.fun > LOW_WARNING &&
        fluffy.energy > LOW_WARNING &&
        fluffy.mood == 0U) {
        return 1U;
    }
    return 0U;
}

static void drawFace(void) {
    printf("\n+-----------+\n");
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("|   - . -   |  (Sleeping Zzz)\n");
    } else if (READ_BIT(fluffy.mood, BIT_SICK)) {
        printf("|   x _ x   |  (Sick / Needs Sleep)\n");
    } else if (READ_BIT(fluffy.mood, BIT_SAD) || READ_BIT(fluffy.mood, BIT_HUNGRY)) {
        printf("|   . _ .   |  (Sad / Hungry)\n");
    } else if (isHappy()) {
        printf("|   ^ . ^   |  (Happy & Thriving!)\n");
    } else {
        printf("|   o . o   |  (Neutral / Alert)\n");
    }
    printf("+-----------+\n");
    fflush(stdout);
}

static void drawStat(const char *label, uint8_t value) {
    printf(" %-8s : [", label);
    for (uint8_t i = 0U; i < STAT_MAX; ++i) {
        putchar((i < value) ? '#' : '-');
    }
    printf("] %2u/%u\n", value, STAT_MAX);
    fflush(stdout);
}

static void updateMood(void) {
    if (fluffy.food <= LOW_WARNING) {
        SET_BIT(fluffy.mood, BIT_HUNGRY);
    } else {
        CLR_BIT(fluffy.mood, BIT_HUNGRY);
    }

    if (fluffy.fun <= LOW_WARNING) {
        SET_BIT(fluffy.mood, BIT_SAD);
    } else {
        CLR_BIT(fluffy.mood, BIT_SAD);
    }

    if (fluffy.energy == 0U) {
        SET_BIT(fluffy.mood, BIT_ASLEEP);
    }
}

static void feed(void) {
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("\n[REFUSED] %s is sleeping! Wake it up first.\n", fluffy.name);
        fflush(stdout);
        return;
    }
    if (fluffy.food == STAT_MAX) {
        SET_BIT(fluffy.mood, BIT_SICK);
        printf("\n[OVERFED] %s was already full! Eating too much made it sick!\n", fluffy.name);
    } else {
        fluffy.food = (fluffy.food + 3U > STAT_MAX) ? STAT_MAX : (fluffy.food + 3U);
        printf("\n[FED] Nom nom! Food +3. Level: %u/%u.\n", fluffy.food, STAT_MAX);
    }
    updateMood();
    fflush(stdout);
}

static void play(void) {
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("\n[REFUSED] %s is sleeping! Wake it up to play.\n", fluffy.name);
        fflush(stdout);
        return;
    }
    fluffy.fun = (fluffy.fun + 3U > STAT_MAX) ? STAT_MAX : (fluffy.fun + 3U);
    fluffy.energy = (fluffy.energy >= 2U) ? (fluffy.energy - 2U) : 0U;
    printf("\n[PLAYED] Fun +3 (Now %u/%u), Energy -2 (Now %u/%u).\n",
           fluffy.fun, STAT_MAX, fluffy.energy, STAT_MAX);
    updateMood();
    fflush(stdout);
}

static void sleepPet(void) {
    TOGGLE_BIT(fluffy.mood, BIT_ASLEEP);
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        printf("\n[SLEEP] %s goes to bed. Goodnight! Zzz...\n", fluffy.name);
    } else {
        if (READ_BIT(fluffy.mood, BIT_SICK)) {
            CLR_BIT(fluffy.mood, BIT_SICK);
            printf("\n[WAKE] %s wakes up refreshed and cured of sickness!\n", fluffy.name);
        } else {
            printf("\n[WAKE] %s wakes up ready for the day!\n", fluffy.name);
        }
    }
    updateMood();
    fflush(stdout);
}

static void hourPasses(void) {
    fluffy.food = (fluffy.food > 0U) ? (fluffy.food - 1U) : 0U;
    fluffy.fun = (fluffy.fun > 0U) ? (fluffy.fun - 1U) : 0U;
    if (READ_BIT(fluffy.mood, BIT_ASLEEP)) {
        fluffy.energy = (fluffy.energy + 2U > STAT_MAX) ? STAT_MAX : (fluffy.energy + 2U);
    } else {
        fluffy.energy = (fluffy.energy > 0U) ? (fluffy.energy - 1U) : 0U;
    }
    fluffy.hours++;
    updateMood();
    printf("\n[TIME] 1 hour elapsed. Total lifetime: %u hour(s).\n", fluffy.hours);
    drawFace();
    fflush(stdout);
}

static void petReport(void) {
    printf("\n================ %s'S CARE CARD ================\n", fluffy.name);
    printf("Lifetime Alive  : %u hours\n", fluffy.hours);
    printf("Overall Status  : %s\n", isHappy() ? "Happy & Healthy" : "Needs Attention");
    drawFace();
    printf("\nStatistics:\n");
    drawStat("Food", fluffy.food);
    drawStat("Fun", fluffy.fun);
    drawStat("Energy", fluffy.energy);
    printf("\nMood Byte (Bin) : 0b");
    for (int8_t i = 7; i >= 0; --i) {
        putchar(((fluffy.mood >> i) & 1U) ? '1' : '0');
    }
    printf(" (Asleep:%u, Hungry:%u, Sad:%u, Sick:%u)\n",
           READ_BIT(fluffy.mood, BIT_ASLEEP),
           READ_BIT(fluffy.mood, BIT_HUNGRY),
           READ_BIT(fluffy.mood, BIT_SAD),
           READ_BIT(fluffy.mood, BIT_SICK));
    printf("Diagnosis       : ");
    if (READ_BIT(fluffy.mood, BIT_ASLEEP))      printf("Asleep and resting peacefully.\n");
    else if (READ_BIT(fluffy.mood, BIT_SICK))   printf("Sick! Needs sleep to recover.\n");
    else if (READ_BIT(fluffy.mood, BIT_HUNGRY)) printf("Hungry! Needs feeding soon.\n");
    else if (READ_BIT(fluffy.mood, BIT_SAD))    printf("Sad/Bored! Needs playtime.\n");
    else                                        printf("Feeling happy, energized and satisfied!\n");
    printf("================================================\n");
    fflush(stdout);
}

int main(void) {
    int choice = -1;
    hatchPet();
    do {
        clearScreen();
        printf("======= FLUFFY ROBOT PET MENU =======\n"
               "1. Care Card / Status Report\n"
               "2. Feed Pet\n"
               "3. Play with Pet\n"
               "4. Put Pet to Bed / Wake Up\n"
               "5. Let 1 Hour Pass\n"
               "6. Hatch Brand-New Pet\n"
               "0. Exit\nEnter choice: ");
        fflush(stdout);
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("\n[POLITE NOTICE] Please enter a valid number.\n");
            pauseScreen();
            choice = -1;
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1: petReport(); pauseScreen(); break;
            case 2: feed(); pauseScreen(); break;
            case 3: play(); pauseScreen(); break;
            case 4: sleepPet(); pauseScreen(); break;
            case 5: hourPasses(); pauseScreen(); break;
            case 6: hatchPet(); pauseScreen(); break;
            case 0: printf("\nGoodbye! Take care of %s!\n", fluffy.name); fflush(stdout); break;
            default: printf("\n[ERROR] Unknown option %d.\n", choice); pauseScreen(); break;
        }
    } while (choice != 0);
    return 0;
}
