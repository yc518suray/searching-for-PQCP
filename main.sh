# parameters
L=46
diffSize=$(($L / 2)) # size of difference table
JSize=20
KSize=19
TYPE=1

# generate ADF candidates
./bin/gen_ADF_cand

# sorting candidates sets
string=""
count=1
while [ $count -le $diffSize ]; do
	str="-nk$count,$count"
	string="$string $str"
	count=$((count+1))
done

sort $string results/$L-$JSize-$TYPE-CAN-J.out > results/$L-$JSize-$TYPE-CAN-J-sort.out
sort $string results/$L-$KSize-$TYPE-CAN-K.out > results/$L-$KSize-$TYPE-CAN-K-sort.out

echo finish sorting candidate files

# find ADFs
./bin/find_ADF
