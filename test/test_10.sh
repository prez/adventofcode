#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../10)"
progname="asteroids"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case example1pt1
example1pt1_head() {
	atf_set "descr" "example 1, pt1"
}
example1pt1_body() {
	atf_check_equal "$(evalprog ex/ex-1 1)" 8
}

atf_test_case example2pt1
example2pt1_head() {
	atf_set "descr" "example 2, pt1"
}
example2pt1_body() {
	atf_check_equal "$(evalprog ex/ex-2 1)" 33
}

atf_test_case example3pt1
example3pt1_head() {
	atf_set "descr" "example 3, pt1"
}
example3pt1_body() {
	atf_check_equal "$(evalprog ex/ex-3 1)" 35
}

atf_test_case example4pt1
example4pt1_head() {
	atf_set "descr" "example 4, pt1"
}
example4pt1_body() {
	atf_check_equal "$(evalprog ex/ex-4 1)" 41
}

atf_test_case example5pt1
example5pt1_head() {
	atf_set "descr" "example 5, pt1"
}
example5pt1_body() {
	atf_check_equal "$(evalprog ex/ex-5 1)" 210
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 296
}

atf_test_case example6pt2
example6pt2_head() {
	atf_set "descr" "example 6, pt2"
}
example6pt2_body() {
	atf_skip "36th not 200th asteroid, cant adjust that in code"
	#atf_check_equal "$(evalprog ex/ex-6 2)" 143
}

atf_test_case example5pt2
example5pt2_head() {
	atf_set "descr" "example 5, pt2"
}
example5pt2_body() {
	atf_check_equal "$(evalprog ex/ex-5 2)" 802
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_check_equal "$(evalprog input 2)" 204
}

atf_init_test_cases() {
	atf_add_test_case example1pt1
	atf_add_test_case example2pt1
	atf_add_test_case example3pt1
	atf_add_test_case example4pt1
	atf_add_test_case example5pt1
	atf_add_test_case myinputpt1
	atf_add_test_case example6pt2
	atf_add_test_case example5pt2
	atf_add_test_case myinputpt2
}
