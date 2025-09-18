#include "sbi.h"
#include "types.h"
// Legacy Extensions (for console I/O)
#define SBI_EID_CONSOLE_PUTCHAR 0x01
#define SBI_EID_CONSOLE_GETCHAR 0x02
// Timer Extension (TIME)
#define SBI_EID_TIME 0x54494D45
#define SBI_FID_SET_TIMER 0
// IPI Extension (IPI)
#define SBI_EID_IPI 0x735049
#define SBI_FID_SEND_IPI 0
// Remote Fence Extension (RFENCE)
#define SBI_EID_RFENCE 0x52464E43
#define SBI_FID_REMOTE_FENCE_I 0
#define SBI_FID_REMOTE_SFENCE_VMA 1
#define SBI_FID_REMOTE_SFENCE_VMA_ASID 2
// System Reset Extension (SRST)
#define SBI_EID_SRST 0x53525354
#define SBI_FID_SYSTEM_RESET 0
#define SBI_RESET_TYPE_SHUTDOWN 0
#define SBI_RESET_REASON_NO_REASON 0

long inline sbi_call(uint64 eid, uint64 fid, uint64 arg0, uint64 arg1, uint64 arg2)
{
	register uint64 a0 asm("a0") = arg0;
	register uint64 a1 asm("a1") = arg1;
	register uint64 a2 asm("a2") = arg2;
	register uint64 a6 asm("a6") = fid;
	register uint64 a7 asm("a7") = eid;
	asm volatile("ecall"
		     : "+r"(a0)
		     : "r"(a1), "r"(a2), "r"(a6), "r"(a7)
		     : "memory");
	return a0;
}

void console_putchar(int c)
{
	sbi_call(SBI_EID_CONSOLE_PUTCHAR, 0, c, 0, 0);
}

int console_getchar()
{
	return sbi_call(SBI_EID_CONSOLE_GETCHAR, 0, 0, 0, 0);
}

void shutdown()
{
	sbi_call(SBI_EID_SRST, SBI_FID_SYSTEM_RESET, SBI_RESET_TYPE_SHUTDOWN, SBI_RESET_REASON_NO_REASON, 0);
}