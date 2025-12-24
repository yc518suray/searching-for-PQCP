# parameters
L=46
diffSize=$(($L - 1))
JSize=20
KSize=19
TYPE=1

# generate ADF candidates
./bin/gen_ADF_cand

# match candidates sets
sort -g -k1,$diffSize results/$L-$JSize-$TYPE-CAN-J.out > results/$L-$JSize-$TYPE-CAN-J-sort.out
sort -g -k1,$diffSize results/$L-$KSize-$TYPE-CAN-K.out > results/$L-$KSize-$TYPE-CAN-K-sort.out

echo finish sorting candidate files
