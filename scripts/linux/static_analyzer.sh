CHECKS="*,-llvm-include-order,-llvm-header-guard,-misc-unused-parameters"
SRC_FILES="../../authserver/src/commands/* ../../authserver/src/* ../../gameserver/src/commands/* ../../gameserver/src/* ../../CORE/src/* ../../lib/commands-processor/src/*"

clang-tidy-8 											\
	-checks=$CHECKS 									\
	-header-filter=.* 									\
	$SRC_FILES -- 										\
	-I ../../include 									\
