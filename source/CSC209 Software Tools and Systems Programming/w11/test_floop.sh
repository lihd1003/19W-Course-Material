upper=$1
file=$2

for count in `seq $upper`
  do 
     #echo calling floop $count $file
     if  ! ./floop $count "$file" > /dev/null 
        then 
         echo $count / $file  is floopy
     fi
  done
