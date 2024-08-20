库文件名: lpHeatMap
功能说明: 热力图模块封装
相关文件: lpHeatMap.dll
依赖库：
无

// 代码使用示例
#include "../lpHeatMap/lpheatmap_global.h"
#if _DEBUG
#pragma comment	(lib, "lpHeatMapd.lib")
#else
#pragma comment (lib, "lpHeatMap.lib")
#endif

{
	m_heatMapPtr =lpCreateHeatMap(row,cols,gridWidth,gridHeight,this);
	m_heatMapPtr->setTabData(int nums, QList<QPoint>points);  //设置数据
	m_heatMapPtr->setChannelTabs(int channel); //设置通道数
	
}


V1.0.0.1 2024-07-10
 1、初版
 2、热力图模块 用于显示片子缺陷位置， 对缺陷在膜片上的位置分布进行统计
>SInpect软件运行后，刚开始没有新建工单时，热力图是没有数据，且没有网格坐标
>当新建工单时，会获取工单信息的尺寸跟流道数，设置到热力图模块才会显示网格坐标，但统计缺陷个数为0；
>点击开始时 ，获取了片子的缺陷信息，拿出缺陷坐标，才会统计每个区域内的缺陷个数
>内部处理数据 线程化
>行列，可配置
 3、尺寸大小跟随窗口 伸缩
 4、支持多流道
 5、加了防呆  不能配置0行0列 和负数   最小为1行1列
 6、配置文件：lpIPUServer.json
     "extend_funtion":{
        "heat_map_info":{
            "enable":true,
            "rows":5,
            "cols":5
            }

		解释：1.enable：功能是否开启

			  2.rows:设置行（根据传入工单信息的模板宽高，自己配置设置几行几列）

			  3.cols:设置列




