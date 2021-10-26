INCLUDE_DIRS="../../include ../../third-party-libraries/libuv/include"
CHECKS="*,-llvm-include-order,-llvm-header-guard,-misc-unused-parameters"
SRC_FILES="../../gameserver/src/commands/* ../../gameserver/src/* ../../CORE/src/*"

clang-tidy-8 \
	-checks=$CHECKS \
	-header-filter=.* \
	$SRC_FILES -- \
	-I "../../include" "../../third-party-libraries/libuv/include"
