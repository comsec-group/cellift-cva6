.section ".user_code_page_base","ax",@progbits
.global user_code_page_base
user_code_page_base:
	la t0, user_data_page_base
	ld t1, (t0)
user_code_infinite_loop:
	j user_code_infinite_loop
