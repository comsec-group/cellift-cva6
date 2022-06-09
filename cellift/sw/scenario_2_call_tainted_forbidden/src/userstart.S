.section ".userstart_section","ax",@progbits
.global userstart
userstart:
	call user_pages
user_infinite_loop:
	j user_infinite_loop
