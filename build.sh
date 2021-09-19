BUILD_DIR="build"

try_create_dir()
{
	if [ ! -d $1 ]; then
		mkdir $1
	fi
}

try_create_dir $BUILD_DIR
cd $BUILD_DIR
cmake ..
make -j 8
