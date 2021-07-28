#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <thread>
#include <mutex>
#include <unordered_map>
#include "mongoose.h"
//#include "ThreadPool.h"
#include "json/json.h"
#include "json/value.h"
#include "json/writer.h"
#include "json/reader.h"
#include "json/version.h"
#include "json/config.h"
#include <sys/time.h>
#include <unistd.h>
#include "logger.h"

#include <ctime>

#include "yolo_v2_class.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>

//typedef unsigned __int64 size_t;
//size_t a = 4;
using namespace std;

// 定义http返回callback
using ReqCallback = std::function<void(std::string)>;
typedef void OnRspCallback(mg_connection *c, std::string);

// 定义http请求handler
using ReqHandler = std::function<bool(std::string, std::string, mg_connection *c, OnRspCallback)>;

struct event_type_and_point
{
	std::vector <std::vector<cv::Point2i>> point;
	int event_type;
	//std::vector <cv::Point2i> point;
	bool autonomy;
};

struct img_and_bbox_result
{
	cv::Mat img;
	vector<bbox_t> box;
	u_int64_t time_Stamp;
};

class HttpServer
{
public:
	HttpServer(){}
	~HttpServer() {}
	void Init(const std::string &port); // 初始化设置
	bool Start(); // 启动httpserver
	bool Close(); // 关闭
	
	static void Init_plate_license_model(); //初始化车牌模型
	static void Delete_plate_license_model();//释放车牌模型
	static string plate_license_det(cv::Mat & img, bbox_t bbox);//车牌模型检测函数　
	static std::unordered_map<std::string, ReqHandler> s_handler_map; // 回调函数映射表	
	static std::vector<bbox_t> IOU(std::vector<bbox_t>v1, std::vector<bbox_t>v2, float th); //计算框的交并比
	static std::vector<bbox_t> IU(std::vector<bbox_t>v1, std::vector<bbox_t>v2);//计算框的并集
	static std::vector<bbox_t> bbox_remove(std::vector<bbox_t> v);//去除重复框
	static img_and_bbox_result detect_IU(const string &rtsp, const string &task_id); //目标检测函数
	static int MultiThread_Docker(string rtsp,std::vector <event_type_and_point> type_and_points , string  time,  
								string  id, string  interval, Json::Value  private_data);//多线程任务处理函数
	static void Read_file_time(string file_name_1 = "./configurationfile/cofigueationfile_time.txt");//读取时间配置文件
	//static string Read_file_imagepath(const string & file_name = "./configurationfile/cofigueationfile_imagepath.txt");
	static void Read_file_postandlog(const string & file_name = "./configurationfile/cofigueationfile_clientpostandlog.txt"); //读取服务端回调地址和log路径配置文件
	static ReqCallback s_req_callback; 
	static int s_exit_flag; 
	static std::string s_web_dir;// 网页根目录  
	static mg_serve_http_opts s_server_option; // web服务器选项
	static void read_classes_names(const string & names_file );//获取类别
	static cv::Mat draw_boxes(cv::Mat  mat_img, std::vector<bbox_t> &result_vec, std::vector<std::string> &obj_names);//画框函数

private:
	// 静态事件响应函数
	static void OnHttpWebsocketEvent(mg_connection *connection, int event_type, void *event_data);
	static void OnHttpEvent(mg_connection *connection, int event_type, void *event_data);//服务端响应函数
	static int HandleHttpEvent(mg_connection *connection, http_message *http_req);//处理http响应函数
	static void SendHttpRsp(mg_connection *connection, std::string rsp); //发送http响应
	static void SendReq(const std::string &url, ReqCallback req_callback, const string &returndata);//发送回调http请求
	static int isWebsocket(const mg_connection *connection); // 判断是否是websoket类型连接
	static void HandleWebsocketMessage(mg_connection *connection, int event_type, websocket_message *ws_msg);
	static void SendWebsocketMsg(mg_connection *connection, std::string msg); // 发送消息到指定连接
	static void BroadcastWebsocketMsg(std::string msg); // 给所有连接广播消息
	static std::unordered_set<mg_connection *> s_websocket_session_set; // 缓存websocket连接
	static int Setup_addr(mg_connection *connection, const string & body);//设置服务地址接口json数据判断
	static int Tasks(mg_connection *connection, string body);//创建并启动视频流分析任务json数据判断
	static string SendSetup_addrJson(int code ,const string &msg);//设置服务地址接口响应
	static string SendTasksJson(int code, const string &msg, const string &task_id, const string & url, int task_type);//创建并启动视频流分析任务
	static string SendReqJson(const vector<bbox_t> &result, const cv::Mat &image, const Json::Value & private_data,const u_int64_t &timestamp);//发送请求(数据结果回调)
	//static map <string, int> idlist_init();//初始化任务列表
	
	std::string m_port;    // 端口
	mg_mgr m_mgr;         // 连接管理器 
	static Detector detector;  //检测器
	static vector <string> urllist; //正在执行任务列表
	static map <string,int> id_map; //任务列表
	static unordered_map<string, int> task_sta;//任务状态
	static mutex mutex_det;      //互斥锁
	static string result_ip; //数据结果回调ip
	static string result_port;//数据结果回调端口
	static string log_path; //日志位置
	static string runtime; //违停等的判定时间
	static string interval;//检查间隔
	static string sendurl; //数据结果回调地址
	static std::vector<std::string> classes_names; //类别名字
	static int max_cvs;//最大并发数
	static int set_addr_flag;//回调地址设置标志位
};



