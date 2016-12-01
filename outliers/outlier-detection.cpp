#include"outlier-detection.h"

DataNode::DataNode(int Id)
{
	id = Id;
}

DataNode::DataNode(int Id, vector<int> attr)
{
	id = Id;
	attributes = attr;
}

DataNode::DataNode(const DataNode &src)
{
	id = src.getID();
	attributes = src.getAttributes();
}

int DataNode::getID()const
{
	return id;
}

vector<int> DataNode::getAttributes()const
{
	return attributes;
}

/*增加坐标*/
void DataNode::addAttributes(vector<int>::iterator begin, vector<int>::iterator end)
{
	while(begin != end)
	{
		attributes.push_back(*begin);
		begin++;
	}
}

/*计算数据点所在块ID*/
int DataNode::getCellID(double width,int base)
{
	int id = 0;
	for(vector<int>::iterator ite = attributes.begin(); ite != attributes.end(); ite++)
		id = id * base + (int)floor(*ite / width);
	return id;
}

/*和另一个数据点的欧氏距离*/
double DataNode::distance(DataNode node)
{
	double result = 0;
	vector<int> attr2 = node.getAttributes();
	vector<int>::iterator ite1 = attributes.begin();
	vector<int>::iterator ite2 = attr2.begin();
	for(; ite1 != attributes.end() && ite2 != attr2.end(); ite1++, ite2++)
		result += pow((*ite1 - *ite2),2.0);
	return sqrt(result);
}

DataCell::DataCell(int Id)
{
	id = Id;
	flag = 0;
	cellNodeCount = 0;
	layer1CellNodeCount = 0;
	layer2CellNodeCount = 0;
}

DataCell::DataCell(const DataCell &src)
{
	id = src.getID();
	flag = src.getFlag();
	cellNodeCount = src.getCellNodeCount();
	nodeList = src.getNodeList();
	nodeIdList = src.getNodeIdList();
	layer1CellNodeCount = src.getLayer1CellNodeCount();
	layer1CellIDList = src.getLayer1CellIDList();
	layer2CellNodeCount = src.getLayer2CellNodeCount();
	layer2CellIDList = src.getLayer2CellIDList();
}

int DataCell::getID()const
{
	return id;
}

int DataCell::getFlag()const
{
	return flag;
}

int DataCell::getCellNodeCount()const
{
	return cellNodeCount;
}

vector<DataNode> DataCell::getNodeList()const
{
	return nodeList;
}

vector<int> DataCell::getNodeIdList()const
{
	return nodeIdList;
}

int DataCell::getLayer1CellNodeCount()const
{
	return layer1CellNodeCount;
}

vector<int> DataCell::getLayer1CellIDList()const
{
	return layer1CellIDList;
}

int DataCell::getLayer2CellNodeCount()const
{
	return layer1CellNodeCount;
}

vector<int> DataCell::getLayer2CellIDList()const
{
	return layer2CellIDList;
}

void DataCell::setFlag(int f)
{
	flag = f;
}
/*增加数据点,按照ID升序排列*/
void DataCell::addDataNodeId(int id)
{
	for(vector<int>::iterator ite = nodeIdList.begin(); ite != nodeIdList.end(); ite++)
	{
		//数据点已存在
		if(*ite == id)
			return ;
		//未存在
		else if(*ite > id)
		{
			nodeIdList.insert(ite, id);
			//更新块内数据点数目
			cellNodeCount++;
			return ;
		}
	}
	//未存在且ID最大，在后面插入
	nodeIdList.push_back(id);
	cellNodeCount++;
}

/*从块中移除数据点*/
void DataCell::removeDataNode(int id)
{
	//从数据点ID列表中移除
	for(vector<DataNode>::iterator ite = nodeList.begin(); ite != nodeList.end(); ite++)
	{
		if((*ite).getID() == id)
		{
			nodeList.erase(ite);
			cellNodeCount--;
			break ;
		}
	}
	//从数据点列表中移除
	for(vector<int>::iterator ite = nodeIdList.begin(); ite != nodeIdList.end(); ite++)
	{
		if(*ite == id)
		{
			nodeIdList.erase(ite);
			break ;
		}
	}
}

/*从文件中加载数据点坐标,文件名即为块ID*/
void DataCell::loadDataNodeFromFile(int dimension)
{
	ifstream ifs;
	char path[30];
	sprintf(path,"tmp/%d.txt",id);
	ifs.open(path);
	if(!ifs)
	{
		cout << "Can not open file " << path << endl;
		exit(0);
	}
	char * line = (char *)malloc(sizeof(char) * (8 * dimension));
	while(!ifs.eof())
	{
		ifs.getline(line, 8 * dimension);
		string sline(line);
		//根据读入的非空行产生一个数据点
		if(sline.size() != 0)
		{
			string tmp;
			size_t bpos = 0;
			size_t epos = 0;
			vector<int> vint;
			while(bpos != string::npos)
			{
				bpos++;
				epos = sline.find_first_of(' ', bpos);
				if(epos != string::npos)
				{
					tmp = sline.substr(bpos, epos - bpos);
					vint.push_back(atoi(tmp.c_str()));
				}
				bpos = epos;
			}
			vector<int>::iterator ite = vint.begin();
			DataNode dataNode(*ite);
			ite++;
			dataNode.addAttributes(ite, vint.end());
			nodeList.push_back(dataNode);
		}
	}
	ifs.close();
}

bool DataCell::isLayer1Cell(int cellId)
{
	for(vector<int>::iterator ite = layer1CellIDList.begin(); ite != layer1CellIDList.end(); ite++)
		if(*ite == cellId)
			return true;
	return false;
}

bool DataCell::isLayer2Cell(int cellId)
{
	for(vector<int>::iterator ite = layer2CellIDList.begin(); ite != layer2CellIDList.end(); ite++)
		if(*ite == cellId)
			return true;
	return false;
}

/*增加块到层1,块按照ID升序排列*/
void DataCell::addLayer1Cell(DataCell &cell)
{
	//当前块
	if(cell.getID() == id)
		return ;
	for(vector<int>::iterator ite = layer1CellIDList.begin(); ite != layer1CellIDList.end(); ite++)
	{
		//已存在
		if(*ite == cell.getID())
			return ;
		//未存在
		else if(*ite > cell.getID())
		{
			//插入块到层1
			layer1CellIDList.insert(ite, cell.getID());
			//更新层1数据点数目
			layer1CellNodeCount += cell.getCellNodeCount();
			return ;
		}
	}
	//未存在且ID最大，在后面插入
	layer1CellIDList.push_back(cell.getID());
	layer1CellNodeCount += cell.getCellNodeCount();
}

/*增加块到层2,块按照ID升序排列*/
void DataCell::addLayer2Cell(DataCell &cell)
{
	//是当前块或者是层1的块
	if(cell.getID() == id || isLayer1Cell(cell.getID()))
		return ;
	for(vector<int>::iterator ite = layer2CellIDList.begin(); ite != layer2CellIDList.end(); ite++)
	{
		//块已在层2中存在
		if(*ite == cell.getID())
			return ;
		else if(*ite > cell.getID())
		{
			//插入块到层2
			layer2CellIDList.insert(ite, cell.getID());
			//更新层2数据点数目
			layer2CellNodeCount += cell.getCellNodeCount();
			return ;
		}
	}
	layer2CellIDList.push_back(cell.getID());
	layer2CellNodeCount += cell.getCellNodeCount();
}

/*计算层1块列表*/
void DataCell::calcLayer1CellList(vector<DataCell>& dataCellList, int base, int dimension)
{
	vector<int> vId = intToVectorCellID(id, base, dimension);
	//通过递归计算层1块ID列表
	layer1CellIDList = getLayerCellIdListByRecursive(vId, base, 1);
	layer1CellNodeCount = 0;
	vector<DataCell>::iterator cellIte = dataCellList.begin();
	vector<int>::iterator ite = layer1CellIDList.begin();
	//更新层1块列表和层1内数据点数目
	for( ; cellIte != dataCellList.end() && ite != layer1CellIDList.end(); )
	{
		if((*cellIte).getID() == *ite)
		{
			addLayer1Cell(*cellIte);
			ite++;
			cellIte++;
		}
		else if((*cellIte).getID() < *ite)
			cellIte++;
		else
			ite++;
	}
}

/*计算层2块列表*/
void DataCell::calcLayer2CellList(vector<DataCell>& dataCellList, int base, int dimension)
{
	vector<int> vId = intToVectorCellID(id, base, dimension);
	//通过递归计算层2块ID列表
	layer2CellIDList = getLayerCellIdListByRecursive(vId, base, (int)ceil(2 * sqrt(dimension)));
	layer2CellNodeCount = 0;
	vector<DataCell>::iterator cellIte = dataCellList.begin();
	vector<int>::iterator ite = layer2CellIDList.begin();
	//更新层2块列表和层1内数据点数目
	for( ; cellIte != dataCellList.end() && ite != layer2CellIDList.end(); )
	{
		if((*cellIte).getID() == *ite)
		{
			addLayer2Cell(*cellIte);
			ite++;
			cellIte++;
		}
		else if((*cellIte).getID() < *ite)
			cellIte++;
		else
			ite++;
	}
}

/*增加异常点*/
void Outliers::addOutlier(int src)
{
	for(vector<int>::iterator ite = outliers.begin();ite != outliers.end(); ite++)
	{
		if(*ite == src)
			return ;
		else if(*ite > src)
		{
			outliers.insert(ite, src);
			return ;
		}
	}
	outliers.push_back(src);
}

/*判断是否是异常点*/
bool Outliers::isOutlier(int index)
{
	for(vector<int>::iterator ite = outliers.begin(); ite != outliers.end(); ite++)
	{
		if(*ite == index)
			return true;
	}
	return false;
}

/*获取异常点*/
vector<int> Outliers::getOutliers()
{
	return outliers;
}

OutlierDetection::OutlierDetection(string infile, int num, double percent, int dimen, double r, int maxm, int mini)
{
	inFilepath = infile;
	numOfDataNode = num;
	dimension = dimen;
	percentage = percent;
	radius = r;
	max = maxm;
	min = mini;
}

/*增加数据块,按照块ID升序排列*/
void OutlierDetection::addCell(DataCell &cell)
{
	for(vector<DataCell>::iterator ite = dataCellList.begin(); ite != dataCellList.end(); ite++)
	{
		//数据块已存在
		if((*ite).getID() == cell.getID())
			return ;
		//未存在
		else if((*ite).getID() > cell.getID())
		{
			dataCellList.insert(ite, cell);
			return ;
		}
	}
	//块未存在且ID最大，在后面插入
	dataCellList.push_back(cell);
}

/*增加数据点到块*/
void OutlierDetection::addNodeToCell(int cellID, DataNode node)
{
	DIR *dp;
	if ((dp = opendir("tmp")) == NULL)
	{
		if(-1 == mkdir("tmp",S_IRUSR | S_IWUSR | S_IXUSR | S_IRWXG | S_IRWXO))
		{
			cout << "Create tmp directory fail" << endl;
			exit(0);
		}
	}
	//在数据块文件中写入数据点
	char tmp[30];
	sprintf(tmp,"tmp/%d.txt",cellID);
	ofstream ofs;
	ofs.open(tmp,ios::out | ios::app);
	if(!ofs)
	{
		cout << "Can not open file " << tmp << endl;
		return ;
	}
	string line;
	sprintf(tmp,"%d ",node.getID());
	line.append(tmp);
	vector<int> attr = node.getAttributes();
	for(vector<int>::iterator ite = attr.begin(); ite != attr.end(); ite++)
	{
		sprintf(tmp,"%d ",*ite);
		line.append(tmp);
	}
	line.append("\n");
	ofs << line;
	ofs.close();
	closedir(dp);

	//在数据块中按照数据点ID升序插入数据点
	for(vector<DataCell>::iterator ite = dataCellList.begin(); ite != dataCellList.end(); ite++)
	{
		//数据块已存在
		if((*ite).getID() == cellID)
		{
			(*ite).addDataNodeId(node.getID());
			return ;
		}
		//数据块未存在
		else if((*ite).getID() > cellID)
		{
			DataCell dataCell(cellID);
			dataCell.addDataNodeId(node.getID());
			dataCellList.insert(ite,dataCell);
			return ;
		}
	}
	//数据块未存在且ID最大
	DataCell dataCell(cellID);
	dataCell.addDataNodeId(node.getID());
	dataCellList.push_back(dataCell);
}

/*随机生成数据点*/
void OutlierDetection::randomGenerate()
{
	ofstream ofs;
	ofs.open(inFilepath.c_str(), ios::out | ios::trunc);
	if(!ofs)
	{
		cout << "Can not open file " << inFilepath.c_str() << endl;
		exit(0);
	}
	srand( (unsigned)time( NULL ) );
	for(int i = 1; i <= numOfDataNode; i++)
	{
		string line;
		char ctmp[30];
		sprintf(ctmp,"%d ",i);			//数据点ID
		line.append(ctmp);
		for(int j = 1; j <= dimension; j++)	//数据点坐标
		{
			int itmp;
			do
			{
				itmp = gaussRand((max + min)/2, 500);
			}while(itmp > max || itmp < min);
			sprintf(ctmp,"%d ",itmp);
			line.append(ctmp);
		}
		line.append("\n");
		ofs << line;
	}
	ofs.close();
}

/*生成数据块*/
void OutlierDetection::generateCell()
{
	double width = radius / (2 * sqrt(dimension));//超立方体边长
	int base = (int)ceil(2 * sqrt(dimension) * (max - min) / radius);//坐标基数
	ifstream ifs;
	ifs.open(inFilepath.c_str());
	if(!ifs)
	{//打开输入文件失败
		cout << "Can not open file " << inFilepath.c_str() << endl;
		exit(0) ;
	}
	char * line = (char *)malloc(sizeof(char) * (8 * dimension));
	while(!ifs.eof())
	{
		ifs.getline(line, 8 * dimension);
		string sline(line);
		//根据行产生一个数据点
		if(sline.size() != 0)
		{
			string tmp;
			size_t bpos = 0;
			size_t epos = 0;
			vector<int> vint;
			while(bpos < sline.size())
			{
				epos = sline.find_first_of(' ', bpos);
				if(epos != string::npos)
				{
					tmp = sline.substr(bpos, epos - bpos);
					vint.push_back(atoi(tmp.c_str()));
				}
				bpos = epos + 1;
			}
			//产生数据点
			vector<int>::iterator ite = vint.begin();
			DataNode dataNode(*ite);
			ite++;
			dataNode.addAttributes(ite, vint.end());
			//增加数据点到数据块
			addNodeToCell(dataNode.getCellID(width, base), dataNode);
		}
	}
	ifs.close();
}

/*统计每个块层1和层2数据点数目*/
void OutlierDetection::layerNodeCount()
{
	for(vector<DataCell>::iterator ite = dataCellList.begin(); ite != dataCellList.end(); ite++)
	{
		(*ite).calcLayer1CellList(dataCellList,(int)ceil(2 * sqrt(dimension) * (max - min) / radius), dimension);
		(*ite).calcLayer2CellList(dataCellList,(int)ceil(2 * sqrt(dimension) * (max - min) / radius), dimension);
	}
}

/*异常点检测*/
Outliers OutlierDetection::outlierDetection()
{
	Outliers outliers;
	int threshold = (int)ceil(numOfDataNode * percentage);
	//检测每个块
	for(vector<DataCell>::iterator cellIte = dataCellList.begin(); cellIte != dataCellList.end(); cellIte++)
	{
		//情况1：块内数据点数目大于阀值，块内数据点都不是异常点
		if((*cellIte).getCellNodeCount() + (*cellIte).getLayer1CellNodeCount() > threshold)
		{
			(*cellIte).setFlag(1);
		}
		//情况2：块内和层1层2数据点总数小于阀值，块内数据点都是异常点
		else if((*cellIte).getCellNodeCount() + (*cellIte).getLayer1CellNodeCount() + (*cellIte).getLayer2CellNodeCount() < threshold + 1)
		{
			(*cellIte).setFlag(-1);
			vector<int> nodeIdList = (*cellIte).getNodeIdList();
			for(vector<int>::iterator ite = nodeIdList.begin(); ite != nodeIdList.end(); ite++)
				outliers.addOutlier(*ite);
		}
		//情况3：需要计算块A内每个点到层2块B中每个数据点的距离
		/*思想：每次处理层2的一个块B，计算块A内每个数据点到块B
		每个数据点的距离，如果小于半径，则是一个邻居，更新邻居
		数目，如果邻居数目达到阀值，则该数据点不是异常点，从块
		A中移除；如此处理每个层2块，都处理完之后仍在块A中的数据
		点都是异常点*/
		else
		{
			//cout << "情况3" << endl;
			//新建一个块用于计算
			DataCell dataCell(*cellIte);
			//从文件中读入块内数据点坐标
			dataCell.loadDataNodeFromFile(dimension);
			//数据点-邻居数目 映射
			map<int,int> nodeId_count;
			vector<int> tmp1 = dataCell.getNodeIdList();
			//块内和层1块内数据点都是邻居
			for(vector<int>::iterator ite = tmp1.begin();ite != tmp1.end(); ite++)
				nodeId_count.insert(pair<int,int>(*ite, dataCell.getCellNodeCount() + dataCell.getLayer1CellNodeCount()));

			vector<int> layer2CellIdList = dataCell.getLayer2CellIDList();
			//每次处理一个层2块
			for(vector<int>::iterator intIte = layer2CellIdList.begin(); intIte != layer2CellIdList.end(); intIte++)
			{
				//新建层2的一个块
				DataCell layer2Cell(*intIte);
				//从文件中读入数据点坐标
				layer2Cell.loadDataNodeFromFile(dimension);
				vector<DataNode> nodeList = dataCell.getNodeList();
				//计算块内每个点到层2块内每个点的距离
				for(vector<DataNode>::iterator ite1 = nodeList.begin(); ite1 != nodeList.end(); ite1++)
				{
					vector<DataNode> layer2NodeList = layer2Cell.getNodeList();
					//层2块内的每个点
					for(vector<DataNode>::iterator ite2 = layer2NodeList.begin(); ite2 != layer2NodeList.end(); ite2++)
					{
						//是r半径内邻居
						if((*ite1).distance(*ite2) < radius)
						{
							//获取数据点已有r邻居数目
							map<int,int>::iterator mapIte = nodeId_count.find((*ite1).getID());
							if(mapIte != nodeId_count.end())
							{
								//更新数据点r邻居数目
								mapIte->second = mapIte->second + 1;
								//数据点r邻居数目达到阀值，不是异常点
								//从块中删除，不再需要检测
								if(mapIte->second > threshold)
								{
									nodeId_count.erase(mapIte);
									dataCell.removeDataNode(mapIte->first);
								}
							}
						}
					}
				}
			}
			//剩下的数据点是这个块中的异常点，加入异常点列表
			for(map<int,int>::iterator mapIte = nodeId_count.begin(); mapIte != nodeId_count.end(); mapIte++)
				outliers.addOutlier(mapIte->first);
		}//end 情况三
	}//end 处理每个数据块
	return outliers;
}
