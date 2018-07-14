all:

	patch /usr/src/servers/pm/table.c table_diff.txt

	patch /usr/src/servers/pm/Makefile makefile_diff.txt

	patch /usr/src/lib/libc/sys-minix/Makefile.inc makefileinc_diff.txt

	patch /usr/src/include/minix/callnr.h callnr_diff.txt
	
	patch /usr/src/servers/pm/proto.h proto_diff.txt

	patch /usr/src/servers/pm/mproc.h mproc_diff.txt

	patch /usr/src/include/unistd.h unistd_diff.txt

	cp do_tag.c  /usr/src/servers/pm

	cp set_tag.c /usr/src/lib/libc/sys-minix

	cp get_tag.c /usr/src/lib/libc/sys-minix

	cp set_tag.2 /usr/man/man2

	cp get_tag.2 /usr/man/man2

	cp gettag.c /root

	cp settag.c /root	

	cd /usr/src/releasetools && make services && make install
	
	cd /usr/src && make build && reboot 