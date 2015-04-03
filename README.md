Introduction and implement of an Outlier Detection Algorithm:CELL<br />
Learn more about it, please read file report.pdf<br />
<br />
compile:<br />
	g++ demo.cpp methods.cpp outlier-detection.cpp -o Outliers<br />
<br />
run:<br />
	./Outliers -r input_file -n num_of_items -a num_of_attr -c fraction -d radius<br />
//<-r>: input file<br />
//<-n>: num of items<br />
//<-a>: num of attributes<br />
//<-c>: a fraction of total items<br />
//<-d>: neighbour radius<br />

注释掉demo.cpp里面的system("rm -r tmp")可保留临时文件<br />
数据点坐标为整型<br />
