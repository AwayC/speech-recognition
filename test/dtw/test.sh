# bin/bash
time python3 generate_data.py>series1.txt
time python3 implement_dtw.py
time ../../cmake-build-debug/test_dtw<series1.txt>test.out
