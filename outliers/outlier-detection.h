#ifndef OUTLIER_DETECTION_H_
#define OUTLIER_DETECTION_H_

#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<map>
#include<string>
#include <sys/stat.h>
#include <sys/types.h>
#include<sys/time.h>
#include <dirent.h>
#include<unistd.h>
#include"methods.h"

using namespace std;

class DataNode{
private:
	int id;
	vector<int> attributes;					//属性(坐标)，整型
public:
	DataNode(int id);
	DataNode(int id, vector<int> attr);
	DataNode(const DataNode &src);
	
	int getID()const;
	vector<int> getAttributes()const;
	void addAttributes(vector<int>::iterator being,vector<int>::iterator end);
	int getCellID(double width, int base);	//根据超立方体边长和坐标基数计算所在的块
	double distance(DataNode node);			//和另一个数据点的距离
};

class DataCell{
private:
	int id;
	int flag;								//-1表示块内的都是outliers，0表示不确定，1表示都不是
	int cellNodeCount;						//块内数据点数目
	int layer1CellNodeCount;				//层1块的数据点数目
	int layer2CellNodeCount;				//层2块的数据数目
	
	vector<DataNode> nodeList;				//块内数据点列表，为节省内存，只在需要计算数据点距离的时候从文件加载
	vector<int> nodeIdList;					//块内数据点ID列表
	vector<int> layer1CellIDList;			//层1块ID列表
	vector<int> layer2CellIDList;			//层2块ID列表

public:
	DataCell(int Id);
	DataCell(const DataCell &src);

	int getID()const;						//获取块ID
	int getFlag()const;						//获取块标记
	int getCellNodeCount()const;			//获取块内数据点数目
	int getLayer1CellNodeCount()const;		//获取层1数据点数目
	int getLayer2CellNodeCount()const;		//获取层2数据点数目
	vector<DataNode> getNodeList()const;	//获取层内数据列表
	vector<int> getNodeIdList()const;		//获取层内数据点ID列表
	vector<int> getLayer1CellIDList()const;	//获取层1块ID列表
	vector<int> getLayer2CellIDList()const;	//获取层2块ID列表
	
	void setFlag(int f);
	void addDataNodeId(int id);				//增加一个数据点到块
	void removeDataNode(int id);			//移除块内的数据点
	void loadDataNodeFromFile(int dimension);//从文件中加载数据点坐标
	bool isLayer1Cell(int cellId);			//判断块是否属于层1
	bool isLayer2Cell(int cellId);			//判断块是否属于层2
	void addLayer1Cell(DataCell &cell);		//增加块到层1
	void addLayer2Cell(DataCell &cell);		//增加块到层2
	void calcLayer1CellList(vector<DataCell>& dataCellList, int base, int dimension);//计算层1块列表
	void calcLayer2CellList(vector<DataCell>& dataCellList, int base, int dimension);//计算层2块列表
};

class Outliers{
private:
	vector<int> outliers;					//outlier ID列表
public:
	void addOutlier(int src);				//增加outliers
	bool isOutlier(int index);				//判断是否为outlier
	vector<int> getOutliers();				//获取outliers ID列表
};

class OutlierDetection{
private:
	vector<DataCell> dataCellList;			//数据库列表
	string inFilepath;						//输入文件路径
	int numOfDataNode;						//数据点总数
	int dimension;							//坐标维度
	int max;								//坐标上界
	int min;								//坐标下界
	double percentage;						//异常点百分数阀值
	double radius;							//半径
	
public:
	OutlierDetection(string infile, int num, double percent, int dimen, double r, int maxm, int mini);

	void addCell(DataCell &src);			//增加数据块
	void addNodeToCell(int cellID, DataNode);//增加数据点到块
	
	void randomGenerate();					//随机生成数据,坐标为整型
	void generateCell();					//生成数据块
	void layerNodeCount();					//计算每个块层1和层2数据点数目
	Outliers outlierDetection();			//异常点检测
};

#endif
