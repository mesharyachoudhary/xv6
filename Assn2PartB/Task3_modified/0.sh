VAR=${1:-DEFAULT}
make clean
make SCHEDFLAG=${VAR}
make qemu SCHEDFLAG=${VAR}
