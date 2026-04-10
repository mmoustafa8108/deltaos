section .text

global arch_fpu_init_hw
arch_fpu_init_hw:
    fninit
    ret

global arch_fpu_save_raw
arch_fpu_save_raw:
    fxsave [rdi]
    ret

global arch_fpu_restore_raw
arch_fpu_restore_raw:
    fxrstor [rdi]
    ret
