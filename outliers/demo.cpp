#include"outlier-detection.h"
//<-r>: input file
//<-n>: num of items
//<-a>: num of attributes
//<-c>: a fraction of total items
//<-d>: neighbour radius
int gettimeofday(struct timeval *tv, struct timezone *tz);
int main(int argc, char **argv)
{
	struct timeval t_start,t_end;
	long cost_time = 0;
	Outliers outliers;
	bool readFileFlag, numOfItemFlag, attriNumFlag, fracTotalFlag, distFlag;
	readFileFlag = numOfItemFlag = attriNumFlag = fracTotalFlag = distFlag = false;

	string inFile;
	int numOfDatas, numOfDim;
	double fracTotal, neighborRadius;
	char ch;
	while ((ch = getopt(argc, argv, "r:n:a:c:d:")) != -1)
	{
		switch (ch)
		{
		case 'r':
			inFile = optarg;
			readFileFlag = true;
			break;
		case 'n':
			numOfDatas = atoi(optarg);
			numOfItemFlag = true;
			break;
		case 'a':
			numOfDim = atoi(optarg);
			attriNumFlag = true;
			break;
		case 'c':
			fracTotal = atof(optarg);
			fracTotalFlag = true;
			break;
		case 'd':
			neighborRadius = atof(optarg);
			distFlag = true;
			break;
		case '?':
			cout << "Unknown option: -" << (char)optopt << endl;
			return 1;
		}
	}

    if(!readFileFlag)
    {
	cout << "please give input file name <-r>" << endl;
        return 2;
    }
    if(!fracTotalFlag)
    {
        cout << "please give a fraction <-c>" << endl;
        return 2;
    }
    if(!distFlag)
    {
		cout << "please give neighbour radius <-d>" << endl;
		return 2;
    }
    if(!numOfItemFlag)
    {
        cout << "please give number of items <-n>" << endl;
		return 2;
    }
    if( !attriNumFlag)
    {
		cout << "please give number of attributes <-a>" << endl;
		return 2;
	}
	gettimeofday(&t_start, NULL);
	OutlierDetection outlierDetection(inFile, numOfDatas, fracTotal, numOfDim, neighborRadius, 4, 0);
	outlierDetection.randomGenerate();
	outlierDetection.generateCell();
	outlierDetection.layerNodeCount();
	outliers = outlierDetection.outlierDetection();
	gettimeofday(&t_end, NULL);
	cost_time = (t_end.tv_sec - t_start.tv_sec) * 1000000  + t_end.tv_usec - t_start.tv_usec;
	resultOutput("out.txt",outliers.getOutliers(),cost_time);
	system(("rm -r " + inFile).c_str());
	system("rm -r tmp");
	return 0;
}
