#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../08)"
progname="layers"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 1340
}

atf_test_case bigboypt1
bigboypt1_head() {
	atf_set "descr" "bigboy, pt1"
}
bigboypt1_body() {
	atf_skip "broken, needs 46x36 dimensions in code"
	# atf_check_equal "$(evalprog interjection_46_36 1)" 99999999999
}

atf_init_test_cases() {
	atf_add_test_case myinputpt1
	atf_add_test_case bigboypt1
}
