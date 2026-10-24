#!/bin/sh

echo "========================================"
echo "       STARTING LOCAL TESTS             "
echo "========================================"

# 1. Verify skeleton.cpp exists in the current directory
if [ ! -f "skeleton.cpp" ]; then
    echo "Error: skeleton.cpp not found in the current directory."
    echo "Make sure you are running this from the folder containing your skeleton.cpp."
    exit 1
fi

# 2. Verify local_tester directory exists
if [ ! -d "local_tester" ]; then
    echo "Error: local_tester folder not found."
    exit 1
fi

# 3. Copy skeleton.cpp directly to local_tester/student.cpp
cp skeleton.cpp local_tester/student.cpp

# Move into the tester directory so logs and relative paths work perfectly
cd local_tester || exit 1

# Clear previous run logs if they exist
rm -f test_results.log

# 4. Compile and Run Tests Sequence
for test_name in local_test1 local_test2a local_test2b local_test2c local_test3a local_test3b local_test4; do
    echo ""
    echo "---> Compiling $test_name.cpp ..."
    
    # Compile with warnings suppressed (-w) to keep student terminal clean
    g++ -std=c++17 -w "$test_name.cpp" -o "$test_name.out"
    
    if [ $? -eq 0 ]; then
        echo "---> Running $test_name ..."
        # Run the test, print to terminal, and append to log file for counting
        ./"$test_name.out" | tee -a test_results.log
    else
        echo "[COMPILE FAIL] $test_name.cpp failed to compile."
        echo "Check the linker errors above to see which missing functions you need to implement next."
    fi
done

# Calculate total passes
if [ -f test_results.log ]; then
    TOTAL_PASS=$(grep -c '\[PASS\]' test_results.log)
else
    TOTAL_PASS=0
fi

echo ""
echo "========================================"
echo "       TOTAL TESTS PASSED: $TOTAL_PASS / 11"
echo "========================================"

# 5. Clean up temporary test files
echo ""
echo "Cleaning up temporary files..."
rm -f student.cpp *.out *.txt test_results.log