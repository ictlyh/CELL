#  Outlier Detection Algorithm: CELL
[![Build Status](https://travis-ci.org/ictlyh/CELL.svg?branch=master)](https://travis-ci.org/ictlyh/CELL)

Introduction and implement of an Outlier Detection Algorithm:CELL  
Learn more about it, please read file report.pdf  
  
compile:  
make -C outliers
  
run:  
	./Outliers -r input_file -n num_of_items -a num_of_attr -c fraction -d radius  
//<-r>: input file  
//<-n>: num of items  
//<-a>: num of attributes  
//<-c>: a fraction of total items  
//<-d>: neighbour radius  

注释掉demo.cpp里面的system("rm -r tmp")可保留临时文件  
数据点坐标为整型  
