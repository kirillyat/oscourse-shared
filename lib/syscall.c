// System call stubs.

#include <inc/syscall.h>
#include <inc/lib.h>

static inline int64_t
syscall(int64_t num, int64_t check, int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5) {
  int64_t ret;

  // Generic system call: pass system call number in AX,
  // up to five parameters in DX, CX, BX, DI, SI.
  // Interrupt kernel with T_SYSCALL.
  //
  // The "volatile" tells the assembler not to optimize
  // this instruction away just because we don't use the
  // return value.
  //
  // The last clause tells the assembler that this can
  // potentially change the condition codes and arbitrary
  // memory locations.

  asm volatile("int %1\n"
               : "=a"(ret)
               : "i"(T_SYSCALL),
                 "a"(num),
                 "d"(a1),
                 "c"(a2),
                 "b"(a3),
                 "D"(a4),
                 "S"(a5)
               : "cc", "memory");

  if (check && ret > 0)
    panic("syscall %ld returned %ld (> 0)", (long)num, (long)ret);

  return ret;
}

void
sys_cputs(const char *s, size_t len) {
  syscall(SYS_cputs, 0, (uint64_t)s, len, 0, 0, 0);
}

int
sys_cgetc(void) {
  return syscall(SYS_cgetc, 0, 0, 0, 0, 0, 0);
}

int
sys_env_destroy(envid_t envid) {
  return syscall(SYS_env_destroy, 1, envid, 0, 0, 0, 0);
}

envid_t
sys_getenvid(void) {
  return syscall(SYS_getenvid, 0, 0, 0, 0, 0, 0);
}
