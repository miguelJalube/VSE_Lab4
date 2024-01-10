sudo apt update

# Required for QML
sudo apt install qtdeclarative5-dev

# Required by pistache
sudo apt install rapidjson-dev

# Required by pistache
sudo apt install libc6-dev-i386



#update googletest
wget https://github.com/google/googletest/archive/release-1.12.0.tar.gz
tar xf release-1.12.0.tar.gz
cd googletest-release-1.12.0
cmake -DBUILD_SHARED_LIBS=ON .
make

sudo cp -a googletest/include/gtest /usr/include
sudo cp -a googlemock/gtest/libgtest_main.so googlemock/gtest/libgtest.so /usr/lib/

# The easiest/best way:
sudo make install  # Note: before v1.11 this can be dangerous and is not supported




# A new version of cmake
wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1-linux-x86_64.sh

chmod 755 cmake-3.25.1-linux-x86_64.sh

./cmake-3.25.1-linux-x86_64.sh


#Install anyRPC
# in a temporary directory
git clone https://github.com/sgieseking/anyrpc.git
cd anyrpc
mkdir build
cd build
cmake -DBUILD_EXAMPLES=OFF -DBUILD_WITH_LOG4CPLUS=OFF -DBUILD_PROTOCOL_MESSAGEPACK=OFF ..
cmake --build .
sudo cmake --install .

cd ../..


# Install Spix
git clone https://github.com/faaxm/spix
cd spix
mkdir build && cd build
../../cmake-3.25.1-linux-x86_64/bin/cmake -DSPIX_QT_MAJOR=5 ..
../../cmake-3.25.1-linux-x86_64/bin/cmake --build .
sudo ../../cmake-3.25.1-linux-x86_64/bin/cmake --install .
