times=$1
filename=$2

total=0
#echo I was supposed to call this $times times

for i in `seq $times`
do
    #echo calling it for time $i 
    num_reads=`./time_reads 1 "$filename" | cut -d " " -f 1`
    echo $num_reads reads on pass $i
    total=`expr $total + $num_reads`
done

echo total is $total
echo average is `expr $total / $times`

