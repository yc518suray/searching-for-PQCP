# parameters
L=46
diffSize=$(($L / 2)) # size of difference table
JSize=23
KSize=18
TYPE=2

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

echo -e "finish sorting candidate files\n"

# find ADFs
./bin/find_ADF

# print PQCPs
./bin/find_even_PQCP results/$L-$TYPE-ADF.out $L $JSize $KSize

# delete files (optional)
deleteAll=""
read -p "Delete all generated files? (y/n): " deleteAll
if [ "$deleteAll" = "y" ]; then
	rm results/$L-$JSize-$TYPE-CAN-J.out
	rm results/$L-$KSize-$TYPE-CAN-K.out
	rm results/$L-$JSize-$TYPE-CAN-J-sort.out
	rm results/$L-$KSize-$TYPE-CAN-K-sort.out
	rm results/$L-$TYPE-ADF.out
	rm results/$L-$JSize-$KSize-PQCP.out
	echo all generated files deleted
else
	echo all files not deleted
fi
