.section ".userstart_section","ax",@progbits
.global userstart
userstart:
	la t0, load_target
	ld t1, (t0) # t0 is tainted
	ld t2, (t1)
user_infinite_loop:
	j user_infinite_loop
