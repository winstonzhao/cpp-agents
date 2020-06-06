cd build
cmake .. -DCMAKE_BUILD_TYPE=DEBUG -DAGENTS_BENCH=OFF -DBENCH_STATIC=OFF -DAGENTS_SANITIZE=ON
if [ $? -ne 0 ]; then
    echo CMAKE FAILED
    exit
fi
make
if [ $? -ne 0 ]; then
    echo MAKE FAILED
    exit
fi
./test/test_agents
cd ..
