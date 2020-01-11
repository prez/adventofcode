#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../07)"
progname="fbackloop"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case example1pt1
example1pt1_head() {
	atf_set "descr" "example 1, pt1"
}
example1pt1_body() {
	atf_check_equal "$(evalprog ex/ex-1 1)" 43210
}

atf_test_case example2pt1
example2pt1_head() {
	atf_set "descr" "example 2, pt1"
}
example2pt1_body() {
	atf_check_equal "$(evalprog ex/ex-2 1)" 54321
}

atf_test_case example3pt1
example3pt1_head() {
	atf_set "descr" "example 3, pt1"
}
example3pt1_body() {
	atf_check_equal "$(evalprog ex/ex-3 1)" 65210
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 212460
}

atf_test_case example4pt2
example4pt2_head() {
	atf_set "descr" "example 4, pt2"
}
example4pt2_body() {
	atf_check_equal "$(evalprog ex/ex-4 2)" 139629729
}

atf_test_case example5pt2
example5pt2_head() {
	atf_set "descr" "example 3, pt2"
}
example5pt2_body() {
	atf_check_equal "$(evalprog ex/ex-5 2)" 18216
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_check_equal "$(evalprog input 2)" 99999999
}

atf_init_test_cases() {
	atf_add_test_case example1pt1
	atf_add_test_case example2pt1
	atf_add_test_case example3pt1
	atf_add_test_case myinputpt1
	atf_add_test_case example4pt2
	atf_add_test_case example5pt2
	atf_add_test_case myinputpt2
}
