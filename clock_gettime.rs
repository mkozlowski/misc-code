use std::arch::asm;

#[derive(Debug)]
#[repr(C)]
struct Timespec {
    tv_sec: usize,
    tv_nsec: usize,
}

#[cfg(target_arch = "x86_64")]
#[no_mangle]
fn sys_clock_gettime(clockid: usize, ts: &mut Timespec) -> isize {
    let mut ret: isize;
    let syscall_no: usize = 228;

    unsafe {
        asm!(
            "syscall",
            inout("rax") syscall_no => ret,
            in("rdi") clockid,
            in("rsi") ts,
            out("rcx") _,
            out("r11") _,
            options(nostack, preserves_flags)
        );
    }

    return ret;
}

fn main() {
    let mut ts = Timespec {
        tv_sec: 0,
        tv_nsec: 0,
    };

    let ret = sys_clock_gettime(0, &mut ts);
    assert!(ret == 0);
    println!("ts: {:?}", ts);
}
