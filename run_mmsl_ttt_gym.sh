cd build
cmake .. -DCMAKE_BUILD_TYPE=RELEASE -DAGENTS_BENCH=OFF -DBENCH_STATIC=ON -DAGENTS_SANITIZE=OFF -DAGENTS_TEST=OFF
# cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DAGENTS_BENCH=OFF -DBENCH_STATIC=OFF -DAGENTS_SANITIZE=ON
if [ $? -ne 0 ]; then
    echo CMAKE FAILED
    exit
fi
make
if [ $? -ne 0 ]; then
    echo MAKE FAILED
    exit
fi
./src/mmsl_ttt_gym
cd ..
