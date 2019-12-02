for file in * ; 
do 
  if [ "${file: -4}" == ".tip" ]; then
    echo "----------$file----------"
    ../build/tipc -t $file
  fi
done