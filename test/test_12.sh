#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../12)"
progname="loops"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case example1pt1
example1pt1_head() {
	atf_set "descr" "example 1, pt1"
}
example1pt1_body() {
	atf_skip "required are 10 steps instead of 1000"
	# atf_check_equal "$(evalprog ex/ex-1 1)" 179
}

atf_test_case example2pt1
example2pt1_head() {
	atf_set "descr" "example 2, pt1"
}
example2pt1_body() {
	atf_skip "required are 100 steps instead of 1000"
	# atf_check_equal "$(evalprog ex/ex-2 1)" 1940
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 12644
}

atf_test_case example1pt2
example1pt2_head() {
	atf_set "descr" "example 1, pt2"
}
example1pt2_body() {
	atf_check_equal "$(evalprog ex/ex-1 2)" 2772
}

atf_test_case example2pt2
example2pt2_head() {
	atf_set "descr" "example 2, pt2"
}
example2pt2_body() {
	atf_check_equal "$(evalprog ex/ex-2 2)" 4686774924
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_check_equal "$(evalprog input 2)" 290314621566528
}

atf_init_test_cases() {
	atf_add_test_case example1pt1
	atf_add_test_case example2pt1
	atf_add_test_case myinputpt1
	atf_add_test_case example1pt2
	atf_add_test_case example2pt2
	atf_add_test_case myinputpt2
}
