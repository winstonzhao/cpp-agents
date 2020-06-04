cd build
cmake ..
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
