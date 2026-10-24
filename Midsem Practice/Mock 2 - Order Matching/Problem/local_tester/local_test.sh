#!/bin/sh
# Run from the folder that contains skeleton.cpp:   sh ./local_tester/local_test.sh

echo "========================================"
echo "       STARTING LOCAL TESTS             "
echo "========================================"

if [ ! -f "skeleton.cpp" ]; then
    echo "Error: skeleton.cpp not found in the current directory."
    exit 1
fi
if [ ! -d "local_tester" ]; then
    echo "Error: local_tester folder not found."
    exit 1
fi

# Same check the judge does during the exam: does the submission compile on its own?
echo ""
echo "---> Compile check (what DOMjudge does during the exam) ..."
if g++ -std=c++17 -w skeleton.cpp -o local_tester/compile_check.out; then
    echo "[COMPILES] skeleton.cpp"
else
    echo "[COMPILE ERROR] skeleton.cpp  ==> this would be ZERO marks. Fix or comment out before submitting."
fi
if grep -q "bits/stdc++" skeleton.cpp; then
    echo "[WARNING] bits/stdc++.h used ==> 50% penalty."
fi

cp skeleton.cpp local_tester/student.cpp
cd local_tester || exit 1
rm -f test_results.log

for test_name in local_test1 local_test2 local_test3 local_test4 local_test5 local_test6 local_test7 local_test8 local_test9a local_test9b; do
    echo ""
    echo "---> Compiling $test_name.cpp ..."
    g++ -std=c++17 -w -fmax-errors=3 "$test_name.cpp" -o "$test_name.out"
    if [ $? -eq 0 ]; then
        echo "---> Running $test_name ..."
        ./"$test_name.out" | tee -a test_results.log
    else
        echo "[COMPILE FAIL] $test_name.cpp failed to compile."
        echo "Usually a missing class/method or a signature that differs from the UML. Read the first error above."
    fi
done

if [ -f test_results.log ]; then
    TOTAL_PASS=$(grep -c '\[PASS\]' test_results.log)
else
    TOTAL_PASS=0
fi

echo ""
echo "========================================"
echo "       TOTAL TESTS PASSED: $TOTAL_PASS / 28"
echo "========================================"

rm -f student.cpp *.out test_results.log
