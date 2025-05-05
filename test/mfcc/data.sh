cd data
python3 data.py > data.out
cd ../
time ../../cmake-build-debug/get_audio < data/data.out > data_conver.out
