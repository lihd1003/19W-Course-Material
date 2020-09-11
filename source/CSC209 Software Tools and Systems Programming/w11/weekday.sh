echo what day is it\?
read day
if test -z $day; then
    echo I don\'t know what day it is
elif test $day = "Sunday" -o $day = "Saturday" ; then
    echo The weekend\! Yipee\!
else
    echo Back to work :\(
fi
