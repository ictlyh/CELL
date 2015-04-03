#include"methods.h"

/*块ID转换*/
int vectorToIntCellID(vector<int> vId, int base)
{
	int id = 0;
	for(vector<int>::iterator ite = vId.begin(); ite != vId.end(); ite++)
		id = id  * base + *ite;
	return id;
}

/*块ID转换*/
vector<int> intToVectorCellID(int id ,int base, int dimension)
{
	vector<int> vId;
	while(id != 0)
	{
		vId.insert(vId.begin(), id % base);
		id = id / base;
	}
	while(vId.size() < dimension)
		vId.insert(vId.begin(), 0);
	return vId;
}

/*
 *通过递归计算块层1和层2块的ID列表
 *vId：vector形式块ID
 *base：基数
 *层1和层2的宽度;层1即为1，层2为2*sqrt(l)，其中l是坐标维数
*/

vector<int> getLayerCellIdListByRecursive(vector<int> vId ,int base, int width)
{
	vector<int> result;
	//只有一维
	if(vId.size() == 1)
	{
		for(int i = -width; i <= width; i++)
		{
			int tmp = *vId.begin() + i;
			if(tmp >=0 && tmp < base)
				result.push_back(tmp);
		}
	}
	else
	{
		vector<int>::iterator ite = vId.begin();
		//最高维ID(左为高位)
		int id = *ite;
		vector<int> remain;
		for(++ite; ite != vId.end(); ite++)
			remain.push_back(*ite);
		//通过递归计算低维对应的块ID
		vector<int> idList = getLayerCellIdListByRecursive(remain, base, width);
		int basic = (int)pow(base, vId.size() - 1);
		for(int i = -width; i <= width; i++)
		{
			if(id + i >=0 && id + i < base)
			{
				for(vector<int>::iterator innerIte = idList.begin(); innerIte != idList.end(); innerIte++)
					result.push_back(basic * (id + i) + *innerIte);
			}
		}
	}
	return result;
}

int gaussRand(double e, double v)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;

    if ( phase == 0 )
	{
		do{
			double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;

            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
		} while(S >= 1 || S == 0);
		X = V1 * sqrt(-2 * log(S) / S);
    }
	else
        X = V2 * sqrt(-2 * log(S) / S);
    phase = 1 - phase;
    return (int)(X*v+e);
}

/*输出结果到文件*/
void resultOutput(string path, vector<int> outliers, long cost_time)
{/*
	ofstream ofs;
	ofs.open(path.c_str(), ios::out | ios::trunc);
	ofs << "The number of outliers: " << outliers.size() << endl;
	for(vector<int>::iterator ite = outliers.begin(); ite != outliers.end(); ite++)
		ofs << *ite << " ";
	ofs << "\nUsing " << cost_time << " us" << endl;
	ofs.close();*/
	cout << "The number of outliers: " << outliers.size() << endl;
	cout << "Using " << cost_time << " us\n" << endl;
}
