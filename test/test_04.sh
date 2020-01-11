#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../04)"
progname="dubs"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 1675
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_check_equal "$(evalprog input 2)" 1142
}

atf_test_case bigboypt1
bigboypt1_head() {
	atf_set "descr" "bigboy, pt1"
}
bigboypt1_body() {
	atf_check_equal "$(evalprog bigboy 1)" 24309
}

atf_test_case bigboypt2
bigboypt2_head() {
	atf_set "descr" "bigboy, pt2"
}
bigboypt2_body() {
	atf_check_equal "$(evalprog bigboy 2)" 18330
}

atf_test_case evenbiggerboypt1
evenbiggerboypt1_head() {
	atf_set "descr" "even bigger boy, pt1"
}
evenbiggerboypt1_body() {
	atf_check_equal "$(evalprog evenbiggerboy 1)" 1562275
}

atf_test_case evenbiggerboypt2
evenbiggerboypt2_head() {
	atf_set "descr" "even bigger boy, pt2"
}
evenbiggerboypt2_body() {
	atf_check_equal "$(evalprog evenbiggerboy 2)" 1223329
}

atf_init_test_cases() {
	atf_add_test_case myinputpt1
	atf_add_test_case myinputpt2
	atf_add_test_case bigboypt1
	atf_add_test_case bigboypt2
	atf_add_test_case evenbiggerboypt1
	atf_add_test_case evenbiggerboypt2
}
