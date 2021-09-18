valgrind --leak-check=full \
         --read-var-info=yes \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file="valgrind.log" \
         ./a.out