#!/bin/tcsh
mkdir out
sudoku 10 < ./tests/1.in > ./out/1.out
HW3 10 <./tests/1.in > ./out/1.expected
echo Test 1
echo ######
diff ./out/1.expected ./out/1.out
echo

sudoku 20 < ./tests/2.in > ./out/2.out
HW3 20 <./tests/2.in > ./out/2.expected
echo Test 2
echo ######
diff ./out/2.expected ./out/2.out
echo

sudoku 30 < ./tests/3.in > ./out/3.out
HW3 30 <./tests/3.in > ./out/3.expected
echo Test 3
echo ######
diff ./out/3.expected ./out/3.out
echo

sudoku 40 < ./tests/4.in > ./out/4.out
HW3 40 <./tests/4.in > ./out/4.expected
echo Test 4
echo ######
diff ./out/4.expected ./out/4.out
echo

sudoku 50 < ./tests/5.in > ./out/5.out
HW3 50 <./tests/5.in > ./out/5.expected
echo Test 5
echo ######
diff ./out/5.expected ./out/5.out
echo

sudoku 60 < ./tests/6.in > ./out/6.out
HW3 60 <./tests/6.in > ./out/6.expected
echo Test 6
echo ######
diff ./out/6.expected ./out/6.out
echo

sudoku 70 < ./tests/7.in > ./out/7.out
HW3 70 <./tests/7.in > ./out/7.expected
echo Test 7
echo ######
diff ./out/7.expected ./out/7.out
echo

sudoku 80 < ./tests/8.in > ./out/8.out
HW3 80 <./tests/8.in > ./out/8.expected
echo Test 8
echo ######
diff ./out/8.expected ./out/8.out
echo

sudoku 90 < ./tests/9.in > ./out/9.out
HW3 90 <./tests/9.in > ./out/9.expected
echo Test 9
echo ######
diff ./out/9.expected ./out/9.out
echo

sudoku 10 < ./tests/10.in > ./out/10.out
HW3 10 <./tests/10.in > ./out/10.expected
echo Test 10
echo ######
diff ./out/10.expected ./out/10.out
echo

sudoku 11 < ./tests/11.in > ./out/11.out
HW3 11 <./tests/11.in > ./out/11.expected
echo Test 11
echo ######
diff ./out/11.expected ./out/11.out
echo

sudoku 12 < ./tests/12.in > ./out/12.out
HW3 12 <./tests/12.in > ./out/12.expected
echo Test 12
echo ######
diff ./out/12.expected ./out/12.out
echo

sudoku 13 < ./tests/13.in > ./out/13.out
HW3 13 <./tests/13.in > ./out/13.expected
echo Test 13
echo ######
diff ./out/13.expected ./out/13.out
echo

sudoku 14 < ./tests/14.in > ./out/14.out
HW3 14 <./tests/14.in > ./out/14.expected
echo Test 14
echo ######
diff ./out/14.expected ./out/14.out
echo

sudoku 15 < ./tests/15.in > ./out/15.out
HW3 15 <./tests/15.in > ./out/15.expected
echo Test 15
echo ######
diff ./out/15.expected ./out/15.out
echo	

sudoku 16 < ./tests/16.in > ./out/16.out
HW3 16 <./tests/16.in > ./out/16.expected
echo Test 16
echo ######
diff ./out/16.expected ./out/16.out
echo

sudoku 17 < ./tests/17.in > ./out/17.out
HW3 17 <./tests/17.in > ./out/17.expected
echo Test 17
echo ######
diff ./out/17.expected ./out/17.out
echo

sudoku 18 < ./tests/18.in > ./out/18.out
HW3 18 <./tests/18.in > ./out/18.expected
echo Test 18
echo ######
diff ./out/18.expected ./out/18.out
echo

sudoku 19 < ./tests/19.in > ./out/19.out
HW3 19 <./tests/19.in > ./out/19.expected
echo Test 19
echo ######
diff ./out/19.expected ./out/19.out
echo

sudoku 100 < ./tests/20.in > ./out/20.out
HW3 100 <./tests/20.in > ./out/20.expected
echo Test 20
echo ######
diff ./out/20.expected ./out/20.out
echo

sudoku 3 < ./tests/21.in > ./out/21.out
HW3 3 <./tests/21.in > ./out/21.expected
echo Test 21
echo ######
diff ./out/21.expected ./out/21.out
echo

sudoku 3 < ./tests/22.in > ./out/22.out
HW3 3 <./tests/22.in > ./out/22.expected
echo Test 22
echo ######
diff ./out/22.expected ./out/22.out
echo

sudoku 3 < ./tests/23.in > ./out/23.out
HW3 3 <./tests/23.in > ./out/23.expected
echo Test 23
echo ######
diff ./out/23.expected ./out/23.out
echo

sudoku 2 < ./tests/24.in > ./out/24.out
HW3 2 <./tests/24.in > ./out/24.expected
echo Test 24
echo ######
diff ./out/24.expected ./out/24.out
echo

sudoku 3 < ./tests/25.in > ./out/25.out
HW3 3 <./tests/25.in > ./out/25.expected
echo Test 25
echo ######
diff ./out/25.expected ./out/25.out
echo

sudoku 2 < ./tests/26.in > ./out/26.out
HW3 2 <./tests/26.in > ./out/26.expected
echo Test 26
echo ######
diff ./out/26.expected ./out/26.out
echo

sudoku 2 < ./tests/27.in > ./out/27.out
HW3 2 <./tests/27.in > ./out/27.expected
echo Test 27
echo ######
diff ./out/27.expected ./out/27.out
echo

sudoku 3 < ./tests/28.in > ./out/28.out
HW3 3 <./tests/28.in > ./out/28.expected
echo Test 28
echo ######
diff ./out/28.expected ./out/28.out
echo

sudoku 2 < ./tests/29.in > ./out/29.out
HW3 2 <./tests/29.in > ./out/29.expected
echo Test 29
echo ######
diff ./out/29.expected ./out/29.out
echo

sudoku 1 < ./tests/30.in > ./out/30.out
HW3 1 <./tests/30.in > ./out/30.expected
echo Test 30
echo ######
diff ./out/30.expected ./out/30.out
echo

sudoku 4 < ./tests/31.in > ./out/31.out
HW3 4 <./tests/31.in > ./out/31.expected
echo Test 31
echo ######
diff ./out/31.expected ./out/31.out
echo

sudoku 2 < ./tests/32.in > ./out/32.out
HW3 2 <./tests/32.in > ./out/32.expected
echo Test 32
echo ######
diff ./out/32.expected ./out/32.out
echo

sudoku 2 < ./tests/33.in > ./out/33.out
HW3 2 <./tests/33.in > ./out/33.expected
echo Test 33
echo ######
diff ./out/33.expected ./out/33.out
echo

sudoku 2 < ./tests/34.in > ./out/34.out
HW3 2 <./tests/34.in > ./out/34.expected
echo Test 34
echo ######
diff ./out/34.expected ./out/34.out
echo

sudoku 3 < ./tests/35.in > ./out/35.out
HW3 3 <./tests/35.in > ./out/35.expected
echo Test 35
echo ######
diff ./out/35.expected ./out/35.out
echo	

sudoku 2 < ./tests/36.in > ./out/36.out
HW3 2 <./tests/36.in > ./out/36.expected
echo Test 36
echo ######
diff ./out/36.expected ./out/36.out
echo

sudoku 2 < ./tests/37.in > ./out/37.out
HW3 2 <./tests/37.in > ./out/37.expected
echo Test 37
echo ######
diff ./out/37.expected ./out/37.out
echo

sudoku 4 < ./tests/38.in > ./out/38.out
HW3 4 <./tests/38.in > ./out/38.expected
echo Test 38
echo ######
diff ./out/38.expected ./out/38.out
echo
