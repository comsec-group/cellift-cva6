.section ".user_pages_start","ax",@progbits
.global user_pages
user_pages:
	ret
user_pages_infinite_loop:
	j user_pages_infinite_loop
