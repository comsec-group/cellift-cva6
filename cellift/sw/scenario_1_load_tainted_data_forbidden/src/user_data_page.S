.section ".user_data_page_section","ax",@progbits
.global user_data_page_base
user_data_page_base:
	ret
user_data_page_infinite_loop:
	j user_data_page_infinite_loop
