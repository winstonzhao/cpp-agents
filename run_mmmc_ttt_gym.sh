cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DAGENTS_BENCH=ON -DBENCH_STATIC=ON -DAGENTS_SANITIZE=OFF -DAGENTS_TEST=OFF

if [ $? -ne 0 ]; then
    echo CMAKE FAILED
    exit
fi
make
if [ $? -ne 0 ]; then
    echo MAKE FAILED
    exit
fi
./bench/bench_mmmc_ttt_gym
cd ..
