BUILD_DIR="build"

try_create_dir()
{
	if [ ! -d $1 ]; then
		mkdir $1
	fi
}

try_create_dir $BUILD_DIR
cd $BUILD_DIR
export CXX="g++-9"
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On -DCMAKE_BUILD_TYPE="Debug" ..
make -j 8
