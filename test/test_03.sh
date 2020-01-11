#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../03)"
progname="wires"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case example1pt1
example1pt1_head() {
	atf_set "descr" "example 1, pt1"
}
example1pt1_body() {
	atf_check_equal "$(evalprog ex/ex-1 1)" 6
}

atf_test_case example2pt1
example2pt1_head() {
	atf_set "descr" "example 2, pt1"
}
example2pt1_body() {
	atf_check_equal "$(evalprog ex/ex-2 1)" 159
}

atf_test_case example3pt1
example3pt1_head() {
	atf_set "descr" "example 3, pt1"
}
example3pt1_body() {
	atf_check_equal "$(evalprog ex/ex-3 1)" 135
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 2129
}

atf_test_case example1pt2
example1pt2_head() {
	atf_set "descr" "example 1, pt2"
}
example1pt2_body() {
	atf_check_equal "$(evalprog ex/ex-1 2)" 30
}

atf_test_case example2pt2
example2pt2_head() {
	atf_set "descr" "example 2, pt2"
}
example2pt2_body() {
	atf_check_equal "$(evalprog ex/ex-2 2)" 610
}

atf_test_case example3pt2
example3pt2_head() {
	atf_set "descr" "example 3, pt2"
}
example3pt2_body() {
	atf_check_equal "$(evalprog ex/ex-3 2)" 410
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_check_equal "$(evalprog input 2)" 134662
}

atf_test_case bigboypt1
bigboypt1_head() {
	atf_set "descr" "bigboy, pt1"
}
bigboypt1_body() {
	atf_check_equal "$(evalprog bigboy 1)" 9999999999
}

atf_test_case bigboypt2
bigboypt2_head() {
	atf_set "descr" "bigboy, pt2"
}
bigboypt2_body() {
	atf_check_equal "$(evalprog bigboy 2)" 9999999999
}

atf_init_test_cases() {
	atf_add_test_case example1pt1
	atf_add_test_case example2pt1
	atf_add_test_case example3pt1
	atf_add_test_case myinputpt1
	atf_add_test_case example1pt2
	atf_add_test_case example2pt2
	atf_add_test_case example3pt2
	atf_add_test_case myinputpt2
}
