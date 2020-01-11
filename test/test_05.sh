#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../05)"
progname="turing"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_skip
	# atf_check_equal "$(evalprog input 1)" 7286649
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_skip
	# atf_check_equal "$(evalprog input 2)" 15724522
}

atf_init_test_cases() {
	atf_add_test_case myinputpt1
	atf_add_test_case myinputpt2
}
