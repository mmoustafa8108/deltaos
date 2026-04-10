#ifndef ARCH_FPU_H
#define ARCH_FPU_H

#include <arch/types.h>

/*
 * architecture-independent FPU/SIMD interface
 * each architecture provides its implementation in arch/<arch>/fpu.h
 */

#if defined(ARCH_AMD64)
    #include <arch/amd64/fpu.h>
#elif defined(ARCH_X86)
    #error "x86 not implemented"
#elif defined(ARCH_ARM64)
    #error "ARM64 not implemented"
#else
    #error "Unsupported architecture"
#endif

/*
 * required types:
 * arch_fpu_state_t - saved architectural floating-point/SIMD register state
 *
 * required MI functions - each arch must implement:
 *
 * arch_fpu_init() - initialize FP support on the current CPU and capture default state
 * arch_fpu_init_thread(state) - initialize a thread's saved FP state to the clean default
 * arch_fpu_save(state) - save the current CPU's FP/SIMD state to memory
 * arch_fpu_restore(state) - restore FP/SIMD state from memory to the current CPU
 * arch_fpu_activate_thread(thread) - prepare FP state for a scheduled-in thread
 * arch_fpu_thread_exit(thread) - release/save any live FP state owned by an exiting thread
 * arch_fpu_handle_device_not_available() - handle lazy-FPU trap (#NM) for current thread
 *
 * required constants:
 * ARCH_FPU_STATE_SIZE - size in bytes of the saved architectural FP state block
 */

#endif
