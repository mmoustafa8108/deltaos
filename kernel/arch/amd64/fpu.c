#include <arch/fpu.h>
#include <arch/cpu.h>
#include <arch/percpu.h>
#include <lib/string.h>
#include <proc/thread.h>

extern void arch_fpu_init_hw(void);
extern void arch_fpu_save_raw(void *state);
extern void arch_fpu_restore_raw(const void *state);

static arch_fpu_state_t arch_fpu_default_state;

static inline void arch_fpu_set_ts(void) {
    uint64 cr0;
    __asm__ volatile ("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= (1ULL << 3);
    __asm__ volatile ("mov %0, %%cr0" :: "r"(cr0) : "memory");
}

static inline void arch_fpu_clear_ts(void) {
    __asm__ volatile ("clts" ::: "memory");
}

void arch_fpu_init(void) {
    arch_fpu_init_hw();
    arch_fpu_save_raw(&arch_fpu_default_state);
    arch_fpu_set_ts();
}

void arch_fpu_init_thread(arch_fpu_state_t *state) {
    if (!state) return;
    memcpy(state, &arch_fpu_default_state, sizeof(*state));
}

void arch_fpu_save(arch_fpu_state_t *state) {
    if (!state) return;
    arch_fpu_save_raw(state);
}

void arch_fpu_restore(const arch_fpu_state_t *state) {
    if (!state) return;
    arch_fpu_restore_raw(state);
}

void arch_fpu_activate_thread(struct thread *thread) {
    percpu_t *cpu = percpu_get();
    if (!cpu || !thread) {
        arch_fpu_set_ts();
        return;
    }

    if (cpu->fpu_owner == thread) {
        arch_fpu_clear_ts();
    } else {
        arch_fpu_set_ts();
    }
}

void arch_fpu_thread_exit(struct thread *thread) {
    percpu_t *cpu = percpu_get();
    if (!cpu || !thread) return;

    if (cpu->fpu_owner == thread) {
        arch_fpu_clear_ts();
        arch_fpu_save(&thread->fpu_state);
        cpu->fpu_owner = NULL;
        arch_fpu_set_ts();
    }
}

int arch_fpu_handle_device_not_available(void) {
    percpu_t *cpu = percpu_get();
    thread_t *current = thread_current();

    if (!cpu || !current) {
        return -1;
    }

    arch_fpu_clear_ts();

    if (cpu->fpu_owner == current) {
        return 0;
    }

    if (cpu->fpu_owner) {
        thread_t *owner = (thread_t *)cpu->fpu_owner;
        arch_fpu_save(&owner->fpu_state);
        owner->fpu_used = 1;
    }

    if (current->fpu_used) {
        arch_fpu_restore(&current->fpu_state);
    } else {
        arch_fpu_restore(&arch_fpu_default_state);
        current->fpu_used = 1;
    }

    cpu->fpu_owner = current;
    return 0;
}
