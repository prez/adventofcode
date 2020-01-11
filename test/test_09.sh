#!/usr/bin/env atf-sh

cdir="$(readlink -f $(atf_get_srcdir)/../09)"
progname="fuckyou"

# 1 - input file, 2 - part
evalprog() {
	${cdir}/${progname} "${cdir}/${1}" | cut -f 2 | sed -n "${2}p"
}

atf_test_case example1pt1
example1pt1_head() {
	atf_set "descr" "example 1, pt1"
}
example1pt1_body() {
	atf_check_equal "$(evalprog ex/ex-1 1)" "109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99"
}

atf_test_case example2pt1
example2pt1_head() {
	atf_set "descr" "example 2, pt1"
}
example2pt1_body() {
	atf_check_equal "$(evalprog ex/ex-2 1)" 1219070632396864
}

atf_test_case example3pt1
example3pt1_head() {
	atf_set "descr" "example 3, pt1"
}
example3pt1_body() {
	atf_check_equal "$(evalprog ex/ex-3 1)" 1125899906842624
}

atf_test_case anon1pt1
anon1pt1_head() {
	atf_set "descr" "anons intput, pt1"
}
anon1pt1_body() {
	atf_check_equal "$(evalprog ex/anon-1 1)" 2427443564
}

atf_test_case myinputpt1
myinputpt1_head() {
	atf_set "descr" "input, pt1"
}
myinputpt1_body() {
	atf_check_equal "$(evalprog input 1)" 2351176124
}

atf_test_case myinputpt2
myinputpt2_head() {
	atf_set "descr" "input, pt2"
}
myinputpt2_body() {
	atf_check_equal "$(evalprog input 2)" 73110
}

atf_test_case anon1pt2
anon1pt2_head() {
	atf_set "descr" "anons intput, pt2"
}
anon1pt2_body() {
	atf_check_equal "$(evalprog ex/anon-1 2)" 87221
}

atf_init_test_cases() {
	atf_add_test_case example1pt1
	atf_add_test_case example2pt1
	atf_add_test_case example3pt1
	atf_add_test_case anon1pt1
	atf_add_test_case myinputpt1
	atf_add_test_case anon1pt2	
	atf_add_test_case myinputpt2
}
