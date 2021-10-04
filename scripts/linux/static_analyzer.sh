CHECKS="*,-llvm-include-order,-llvm-header-guard"
SRC_FILES="../../gameserver/src/* ../../CORE/src/*"

clang-tidy-8 \
	-checks=$CHECKS \
	-header-filter=.* \
	$SRC_FILES -- \
	-I../../include