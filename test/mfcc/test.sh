python3 generate_data.py > save.out
python3 test.py > py.out
../../cmake-build-debug/test_mfcc < save.out > test.out