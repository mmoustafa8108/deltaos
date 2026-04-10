#ifndef ARCH_AMD64_FPU_H
#define ARCH_AMD64_FPU_H

#include <arch/types.h>

#define ARCH_FPU_STATE_SIZE 512

typedef struct __attribute__((aligned(16))) arch_fpu_state {
    uint8 bytes[ARCH_FPU_STATE_SIZE];
} arch_fpu_state_t;

struct thread;

void arch_fpu_init(void);
void arch_fpu_init_thread(arch_fpu_state_t *state);
void arch_fpu_save(arch_fpu_state_t *state);
void arch_fpu_restore(const arch_fpu_state_t *state);
void arch_fpu_activate_thread(struct thread *thread);
void arch_fpu_thread_exit(struct thread *thread);
int arch_fpu_handle_device_not_available(void);

#endif
