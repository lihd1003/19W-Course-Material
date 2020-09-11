for arg in "$@"  # or $* will work here 
do
  ./fibonacci $arg
done

# Alternative:
# for i 
# do
# 	./fibonacci $i
# done
