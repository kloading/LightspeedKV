#!/usr/bin/env python3
from bcc import BPF
from bcc.utils import printb
from ctypes import *

device = "lo"
b = BPF(src_file="mf_db.c")
fn = b.load_func("mf_db", BPF.XDP)
tail_fn = b.load_func("tail_call", BPF.XDP)
prog_array = b.get_table("prog_array")
prog_array[c_int(2)] = c_int(tail_fn.fd)
b.attach_xdp(device, fn, 0)

try:
    b.trace_print()
except KeyboardInterrupt:
    pass

b.remove_xdp(device, 0) 
