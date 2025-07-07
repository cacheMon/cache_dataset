rm -rf libCacheSim
git clone https://github.com/1a1a11a/libCacheSim.git
pushd libCacheSim
bash scripts/install_dependency.sh
export PATH=$HOME/software/cmake/bin:$PATH
bash scripts/install_libcachesim.sh
popd