//#include <utility>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <algorithm>
#include <sys/stat.h>
#include <sys/types.h>
#define OPENCV
#include "http_server.h"
#include <vector>
#include "init.h"
#include "ccilpr.h"
#include <pthread.h>
#include <functional>

//#include <sys/file.h>
//#include<sys/time.h>



using namespace cv;
using namespace std;
using namespace FrameWork;

// typedef int  *(*FUNCALLBACK)(void * arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6);
// typedef struct {
//    FUNCALLBACK callback;
//    void * arg1;
//    void * arg2;
//    void * arg3;
//    void * arg4;
//    void * arg5;
//    void * arg6;
// } ARGS;
// void * sub_thread_entry(void * arg)
// {
//     ARGS args;
    
//     /* retrieve args */
//     args.callback = ((ARGS *)arg)->callback;
//     args.arg1 = ((ARGS *)arg)->arg1;
//     args.arg2 = ((ARGS *)arg)->arg2;
//     args.arg3 = ((ARGS *)arg)->arg3;
// 	args.arg4 = ((ARGS *)arg)->arg4;
// 	args.arg5 = ((ARGS *)arg)->arg5;
// 	args.arg6 = ((ARGS *)arg)->arg6;
//     free(arg);
    
//     return args.callback(args.arg1, args.arg2, args.arg3, args.arg4, args.arg5, args.arg6);
// }

// static int my_pthread_create(pthread_t *thread, const pthread_attr_t *attr,
//         FUNCALLBACK start_routine, void *arg1, void * arg2, void * arg3, void * arg4, void * arg5, void * arg6)
// {
//     ARGS * args;
    
//     if (NULL == (args=(ARGS *)malloc(sizeof(ARGS))))
//         return -1;
     
//     /* push args into buffer */
//     args->callback = start_routine;
//     args->arg1 = arg1;
//     args->arg2 = arg2;
//     args->arg3 = arg3;
// 	args->arg4 = arg4;
// 	args->arg5 = arg5;
// 	args->arg6 = arg6;
    
//     return pthread_create(thread, attr, sub_thread_entry, args);
// }

//  string  getCurrentTimeStr()
//  {
//  	SYSTEMTIME sys;
//  	GetLocalTime(&sys);
//  	//printf("%4d/%02d/%02d %02d:%02d:%02d.%03d ����%1d\n", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond, sys.wMilliseconds, sys.wDayOfWeek);
//  	string time = to_string(sys.wHour) + to_string(sys.wMinute) + to_string(sys.wSecond);
//  	return time;
//  }
//base64编解码
//  static std::string base64Decode(const char* Data, int DataByte) {
// 	 //解码表
// 	 const char DecodeTable[] =
// 	 {
// 		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 		 62, // '+'
// 		 0, 0, 0,
// 		 63, // '/'
// 		 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
// 		 0, 0, 0, 0, 0, 0, 0,
// 		 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
// 		 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
// 		 0, 0, 0, 0, 0, 0,
// 		 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
// 		 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
// 	 };
// 	 std::string strDecode;
// 	 int nValue;
// 	 int i = 0;
// 	 while (i < DataByte) {
// 		 if (*Data != '\r' && *Data != '\n') {
// 			 nValue = DecodeTable[*Data++] << 18;
// 			 nValue += DecodeTable[*Data++] << 12;
// 			 strDecode += (nValue & 0x00FF0000) >> 16;
// 			 if (*Data != '=') {
// 				 nValue += DecodeTable[*Data++] << 6;
// 				 strDecode += (nValue & 0x0000FF00) >> 8;
// 				 if (*Data != '=') {
// 					 nValue += DecodeTable[*Data++];
// 					 strDecode += nValue & 0x000000FF;
// 				 }
// 			 }
// 			 i += 4;
// 		 }
// 		 else {
// 			 Data++;
// 			 i++;
// 		 }
// 	 }
// 	 return strDecode;
//  }
static std::string base64Encode(const unsigned char* Data, int DataByte) {
	 //编码表
	 const char EncodeTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	 //返回值
	 std::string strEncode = "";
	 unsigned char Tmp[4] = { 0 };
	 int LineLength = 0;
	 for (int i = 0; i < (int)(DataByte / 3); i++) {
		 Tmp[1] = *Data++;
		 Tmp[2] = *Data++;
		 Tmp[3] = *Data++;
		 strEncode += EncodeTable[Tmp[1] >> 2];
		 strEncode += EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		 strEncode += EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		 strEncode += EncodeTable[Tmp[3] & 0x3F];
		 //if (LineLength += 4, LineLength == 76) { strEncode += "\r\n"; LineLength = 0; }
	 }
	 //对剩余数据进行编码
	 int Mod = DataByte % 3;
	 if (Mod == 1) {
		 Tmp[1] = *Data++;
		 strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		 strEncode += EncodeTable[((Tmp[1] & 0x03) << 4)];
		 strEncode += "==";
	 }

	 else if (Mod == 2) {
		 Tmp[1] = *Data++;
		 Tmp[2] = *Data++;
		 strEncode += EncodeTable[(Tmp[1] & 0xFC) >> 2];
		 strEncode += EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		 strEncode += EncodeTable[((Tmp[2] & 0x0F) << 2)];
		 strEncode += "=";
	 }
	 return strEncode;
}
 //imgType 包括png bmp jpg jpeg等opencv能够进行编码解码的文件
static std::string Mat2Base64(const cv::Mat &img, std::string imgType)
{
	 //Mat转base64
	 std::string img_data = "";
	 std::vector<uchar> vecImg;
	 std::vector<int> vecCompression_params;
	 vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	 vecCompression_params.push_back(90);
	 imgType = "." + imgType;
	 cv::imencode(imgType, img, vecImg, vecCompression_params);
	 img_data = base64Encode(vecImg.data(), vecImg.size());
	 return img_data;
}
//  static cv::Mat Base2Mat(std::string &base64_data)
//  {
// 	 cv::Mat img;
// 	 std::string s_mat;
// 	 s_mat = base64Decode(base64_data.data(), base64_data.size());
// 	 std::vector<char> base64_img(s_mat.begin(), s_mat.end());
// 	 img = cv::imdecode(base64_img, CV_LOAD_IMAGE_COLOR);
// 	 return img;
//  }
 //linux时间获取函数
// string  getCurrentTimeStr() {
// 	time_t t = time(NULL);
// 	char ch[64] = { 0 };
// 	char result[100] = { 0 };
// 	struct timeval tv;
// 	gettimeofday(&tv, NULL);
// 	int ms = (tv.tv_sec * 1000 + tv.tv_usec / 1000) % 1000;
// 	strftime(ch, sizeof(ch) - 1, "%Y-%m-%d %H:%M:%S:", localtime(&t));     
// 	sprintf(result, "%s%d", ch, ms);
// 	return string(result);
// }
//http响应输出
void handle_func(std::string rsp)
{
	//;
	string resultData = rsp;
	cout << resultData << endl;
	Logger::GetInstance().Info(resultData);
}
void HttpServer::Init_plate_license_model() {//初始化车牌模型
	int lpr = InitPr(&pr);
}
void HttpServer::Delete_plate_license_model() {//释放车牌模型
	int lpr = UnInitPr(pr);
	if (lpr != 0)
	{
		printf("lpr UnInitPr error\n");
	}
}
string HttpServer::plate_license_det(cv::Mat &img, bbox_t bbox) {//车牌模型检测函数
	Mat image = img;
	if (bbox.x >= 0 && bbox.w >= 0 && bbox.x + bbox.w <= img.cols && bbox.y >= 0 && bbox.h >=0 && bbox.y + bbox.h <= img.rows) {
		Rect rect(bbox.x, bbox.y, bbox.w, bbox.h);
		Mat car_img = image(rect);

		std::vector<cv::Rect> PlateRect;
		std::vector<std::string> PlateLicense;
		LprRecognize(pr, car_img, PlateRect, PlateLicense);
		int num = PlateRect.size();
		if (num == 0) {
			std::cout << "detect failed!" << std::endl;
			return "NULL";
		}
		else {
			for (int i = 0; i < num; i++) {
				return PlateLicense[i];
			}
		}
	}
	else {
		return "NULL";
	}
	
}
void HttpServer::Init(const std::string &port)
{
	//string Log_path = log_path + "/cy_";
	//InitLogging("./",INFO, Log_path.c_str()); //日志文件设置
	m_port = port;
	s_server_option.enable_directory_listing = "yes";
	s_server_option.document_root = s_web_dir.c_str();

	// 其他http设置
	// 开启 CORS，本项只针对主页加载有效.
	// s_server_option.extra_headers = "Access-Control-Allow-Origin: *";
}

bool HttpServer::Start()
{
	InitLogging("./",INFO, "./log/"); //日志文件设置
	mg_mgr_init(&m_mgr, NULL);
	mg_connection *connection = mg_bind(&m_mgr, m_port.c_str(), HttpServer::OnHttpWebsocketEvent);
	if (connection == NULL)
		return false;
	// for both http and websocket
	mg_set_protocol_http_websocket(connection);

	printf("starting http server at port: %s\n", m_port.c_str());
	// loop
	while (true)
		mg_mgr_poll(&m_mgr, 500); // ms

    return true;
}

void HttpServer::OnHttpWebsocketEvent(mg_connection *connection, int event_type, void *event_data)
{
	// 区分http和websocket
	if (event_type == MG_EV_HTTP_REQUEST)
	{
		http_message *http_req = (http_message *)event_data;
		HandleHttpEvent(connection, http_req);
	}
	else if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE ||
		event_type == MG_EV_WEBSOCKET_FRAME ||
		event_type == MG_EV_CLOSE)
	{
		websocket_message *ws_message = (struct websocket_message *)event_data;
		HandleWebsocketMessage(connection, event_type, ws_message);
	}
}
// 客户端的网络请求响应
void HttpServer::OnHttpEvent(mg_connection * connection, int event_type, void * event_data)
{
	http_message *hm = (struct http_message *)event_data;
	int connect_status;
	//s_exit_flag = 1;
	switch (event_type)
	{
	case MG_EV_CONNECT:
		connect_status = *(int *)event_data;
		if (connect_status != 0)
		{
			printf("Error connecting to server, error code: %d\n", connect_status);
			Logger::GetInstance().Info(" Error connecting to resultCallBack client!"); 
			s_exit_flag = 1;
		}
		break;
	case MG_EV_HTTP_REPLY:
	{
		//printf("Got reply:\n%.*s\n", (int)hm->body.len, hm->body.p);
		std::string rsp = std::string(hm->body.p, hm->body.len);
		connection->flags |= MG_F_SEND_AND_CLOSE;
		s_exit_flag = 1; // 每次收到请求后关闭本次连接，重置标记

		// 回调处理
		s_req_callback(rsp);
	}
	break;
	case MG_EV_CLOSE:
		if (s_exit_flag == 0)
		{
			//printf("Server closed connection\n");
			s_exit_flag = 1;
		};
		break;
	default:
		break;
	}
}

// ---- simple http ---- //
static bool route_check(http_message *http_msg, char *route_prefix)
{
	if (mg_vcmp(&http_msg->uri, route_prefix) == 0)
		return true;
	else
		return false;

	// TODO: 还可以判断 GET, POST, PUT, DELTE等方法
	//mg_vcmp(&http_msg->method, "GET");
	//mg_vcmp(&http_msg->method, "POST");
	//mg_vcmp(&http_msg->method, "PUT");
	//mg_vcmp(&http_msg->method, "DELETE");
}

void HttpServer::SendHttpRsp(mg_connection *connection, std::string rsp)
{
	// --- 未开启CORS
	// 必须先发送header, 暂时还不能用HTTP/2.0
	mg_printf(connection, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\nContent-Type: application/json\r\n\r\n");
	// 以json形式返回
	mg_printf_http_chunk(connection, "%s", rsp.c_str());
	// 发送空白字符快，结束当前响应
	mg_send_http_chunk(connection, "", 0);

	// --- 开启CORS
	/*mg_printf(connection, "HTTP/1.1 200 OK\r\n"
			  "Content-Type: text/plain\n"
			  "Cache-Control: no-cache\n"
			  "Content-Length: %d\n"
			  "Access-Control-Allow-Origin: *\n\n"
			  "%s\n", rsp.length(), rsp.c_str()); */
}

void HttpServer::SendReq(const std::string & url, ReqCallback req_callback, const string &returndata)
{	
	s_req_callback = req_callback; // 给回调函数赋值
	mg_mgr mgr;
	mg_mgr_init(&mgr, NULL);
	auto connection = mg_connect_http(&mgr, OnHttpEvent, url.c_str(), "Content-Type: application/json\r\n", returndata.c_str());
	mg_set_protocol_http_websocket(connection);
	//printf("Send http request %s\n", url.c_str());
	s_exit_flag = 0;
	while (s_exit_flag == 0)
	   mg_mgr_poll(&mgr, 500);
	mg_mgr_free(&mgr);
}

int HttpServer::HandleHttpEvent(mg_connection *connection, http_message *http_req)
{
	std::string req_str = std::string(http_req->message.p, http_req->message.len);
	//printf("got request: %s\n", req_str.c_str());
	cout<<"received a request!"<<endl;
	std::string url = std::string(http_req->uri.p, http_req->uri.len);
	std::string body = std::string(http_req->body.p, http_req->body.len);
	//cout << body << endl;
	Logger::GetInstance().Info(url + " " + body);  //日志写入
	if (route_check(http_req, "/api/ads/v1/setup-addr")) //设置服务地址接口
	{
		if (Setup_addr(connection, body) == 0)
		{
			SendHttpRsp(connection, (SendSetup_addrJson(0, "Setting IP and port succeeded")).c_str());
			cout << "Setting IP and port succeeded" << endl;
		}
	}
	else if (route_check(http_req, "/api/ads/v1/tasks"))//创建并启动视频流分析任务
	{
		if (Tasks(connection, body) == 0)//创建任务json数据判断
		{
			set_addr_flag = 1;  //当此标志位被置1 说明视频流分析任务已经被创建  不能设置服务接口位置了
			Json::Reader reader;
			Json::Value value_task;
			reader.parse(body, value_task);
			//任务id	
			string task_id = value_task["task_id"].asString();
			urllist.push_back(task_id);//若task_id不在urllist列表中，将其添加进去
			task_sta[task_id] = 0;    //初始时默认任务状态为０（正常）
			id_map[task_id] = 1;
			//rtsp流
			string rtsp;
			//任务类型
			switch (value_task["task_type"].asInt())
			{
				case 0: rtsp = value_task["stream_url"].asCString(); 
						break; 
				case 1: rtsp = value_task["iosd_url"].asCString(); 
						break; 
				default: 
						break;
			}

			std::vector <event_type_and_point> all_type_points(0); //
			
			//解析分析规则
			for (unsigned int i = 0; i < value_task["analysis_rules"].size(); i++)
			{
				int n = value_task["analysis_rules"][i]["detect_areas"].size();
				event_type_and_point tmp;
				tmp.point.resize(n); //初始化n行，n个检测区域
				tmp.event_type = value_task["analysis_rules"][i]["event_type"].asInt();
				tmp.autonomy = value_task["analysis_rules"][i]["autonomy"].asBool();

				for (int k = 0; k < n; k++) {
					for (unsigned int j = 0; j < value_task["analysis_rules"][i]["detect_areas"][k]["area"].size(); j++)
					{   
						cv::Point2i p;
						p.x = value_task["analysis_rules"][i]["detect_areas"][k]["area"][j]["point_x"].asInt();
						p.y = value_task["analysis_rules"][i]["detect_areas"][k]["area"][j]["point_y"].asInt();
						tmp.point[k].push_back(p);
					}
				}
				all_type_points.push_back(tmp);
			}

			Json::Value private_data;//私有数据透传
			Json::FastWriter fast_writer;
			string private_data_string = fast_writer.write(value_task["private_data"]);
			reader.parse(private_data_string, private_data);
			// pthread_t thread_id;
			// pthread_attr_t thread_attr;
			// struct sched_param thread_param;
			// int res;
			// res = pthread_attr_init(&thread_attr);
			// if (res) {
			// 	cout << "pthread_attr_init failed:" << res << endl;
			// }
			// res = pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
			// if (res) {
			// 	cout << "pthread_attr_setdetachstate failed: " << res << endl;
			// }
			// res = my_pthread_create(&thread_id, &thread_attr, &MultiThread_Docker, (void *)(&rtsp), (void *)(&all_type_points), (void *)(&runtime), (void *)(&task_id),(void *)(&interval), (void *)(&private_data));
			//请求响应
			SendHttpRsp(connection, (SendTasksJson(0, "The data was sent successfully and will be returned to " + sendurl, value_task["task_id"].asString(), rtsp, value_task["task_type"].asInt())).c_str());
			std::thread mythread(MultiThread_Docker, rtsp, all_type_points, runtime,task_id, interval, private_data);//多线程处理
			mythread.detach();//设置当前线程为分离状态
			//}			
		}
	}
	else if (route_check(http_req, "/api/v1/ads/resources"))//查询算法资源信息(算法能力集资源及使用情况)接口
	{
		Json::Value resources_value;
		resources_value["code"] = 0;
		resources_value["error_msg"] = "NULL";
		resources_value["algo_type"] = 1;
		resources_value["model_version"][0] = "";
		resources_value["capability_info"]["max_tps"] = 0;
		resources_value["capability_info"]["cur_tps"] = 0;
		resources_value["capability_info"]["max_cvs"] = max_cvs;
		resources_value["capability_info"]["cur_cvs"] = int(urllist.size());

		SendHttpRsp(connection, Json::FastWriter().write(resources_value));
		Logger::GetInstance().Info(Json::FastWriter().write(resources_value));
		cout<< Json::FastWriter().write(resources_value)<<endl;
	}
	else if (route_check(http_req, "/api/v1/ads/task-status"))//查询任务状态信息接口
	{
		Json::Value status_value;
		if (urllist.size() == 0)
		{
			status_value["code"] = 0;
			status_value["error_msg"] = "The number of tasks is zero!";
		}
		else {
			status_value["code"] = 0;
			status_value["error_msg"] = "NULL";
		}
		for (int i = 0; i < urllist.size(); i++)
		{
			Json::Value task_value;
			task_value["task_id"] = urllist[i];
			if (task_sta[urllist[i]] == 2) {
				task_value["status"] = 2;
				task_value["abnormal_msg"] = "The rtsp can't open!";
			}
			else {
				task_value["status"] = 0;
			}

			status_value["task_info"].append(task_value);
		}
		SendHttpRsp(connection, Json::FastWriter().write(status_value));
		Logger::GetInstance().Info(Json::FastWriter().write(status_value));
		cout<< Json::FastWriter().write(status_value)<<endl;
	}
	else if (!mg_vcmp(&http_req->method, "DELETE"))//api/ads/v1/task/{task_id}
	{
		int delete_index = url.find("/api/ads/v1/task/");
		if (delete_index == url.npos)
		{
			Json::Value delete_value;
			delete_value["code"] = 9;
			delete_value["error_msg"] = "url is not /api/ads/v1/task/{task_id}";
			delete_value["task_id"] = -1;

			SendHttpRsp(connection, Json::FastWriter().write(delete_value));
			Logger::GetInstance().Info(Json::FastWriter().write(delete_value));
		}
		else
		{
			Json::Value delete_value;
			string task_id = url.substr(17, url.length());
			vector<string>::iterator it = find(urllist.begin(), urllist.end(), task_id);
			if (it != urllist.end()) 
			{
				id_map[task_id] = 0;
				urllist.erase(it);  //删除任务id
				for (auto i = task_sta.begin(); i != task_sta.end(); ) {
					if (i->first == task_id) {
						i = task_sta.erase(i);
					}
					else {
						++i;
					}
				}			
				delete_value["code"] = 0;
				delete_value["error_msg"] = "this video will be deleted";
				delete_value["task_id"] = task_id;

				SendHttpRsp(connection, Json::FastWriter().write(delete_value));
				Logger::GetInstance().Info(Json::FastWriter().write(delete_value));
			}
			else
			{
				delete_value["code"] = 1;
				delete_value["error_msg"] = "this video not runing";
				delete_value["task_id"] = task_id;

				SendHttpRsp(connection, Json::FastWriter().write(delete_value));
				Logger::GetInstance().Info(Json::FastWriter().write(delete_value));
			}			
		}
	}	
	else
	{
		mg_printf(
			connection,
			"%s",
			"HTTP/1.1 400 Not Implemented\r\n"
			"Content-Length: 0\r\n\r\n");
		//InitLogging("./", INFO, "./log/cy_")
		Logger::GetInstance().Info("HTTP/1.1 400 Not Implemented\r\nContent-Length: 0\r\n\r\n");	
	}
	return 0;
}
 
// ---- websocket ---- //
int HttpServer::isWebsocket(const mg_connection *connection)
{
	return connection->flags & MG_F_IS_WEBSOCKET;
}

void HttpServer::HandleWebsocketMessage(mg_connection *connection, int event_type, websocket_message *ws_msg)
{
	if (event_type == MG_EV_WEBSOCKET_HANDSHAKE_DONE)
	{
		cout<<"client websocket connected"<<endl;
		// 获取连接客户端的IP和端口
		char addr[32];
		mg_sock_addr_to_str(&connection->sa, addr, sizeof(addr), MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_PORT);
		cout<<"client addr: "<< addr << endl;

		// 添加 session
		s_websocket_session_set.insert(connection);

		SendWebsocketMsg(connection, "client websocket connected");
	}
	else if (event_type == MG_EV_WEBSOCKET_FRAME)
	{
		/*mg_str received_msg = {
			(char *)ws_msg->data, ws_msg->size
		};
*/
		//char buff[1024] = {0};
		//strncpy_s(buff, received_msg.p, received_msg.len); // must use strncpy, specifiy memory pointer and length

		// do sth to process request
		//cout<<"received msg: %s\n", buff;
		//SendWebsocketMsg(connection, "send your msg back: " + std::string(buff));
		//BroadcastWebsocketMsg("broadcast msg: " + std::string(buff));
	}
	else if (event_type == MG_EV_CLOSE)
	{
		if (isWebsocket(connection))
		{
			cout << "client websocket closed" << endl;
			// 移除session			
			if (s_websocket_session_set.find(connection) != s_websocket_session_set.end())
				s_websocket_session_set.erase(connection);
		}
	}
}

void HttpServer::SendWebsocketMsg(mg_connection *connection, std::string msg)
{
	mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}

void HttpServer::BroadcastWebsocketMsg(std::string msg)
{
	for (mg_connection *connection : s_websocket_session_set)
		mg_send_websocket_frame(connection, WEBSOCKET_OP_TEXT, msg.c_str(), strlen(msg.c_str()));
}

bool HttpServer::Close()
{
	mg_mgr_free(&m_mgr);
	return true;
}
std::vector<bbox_t> HttpServer::IOU(std::vector<bbox_t>v1, std::vector<bbox_t>v2, float th)
{
	v1 = bbox_remove(v1);
	v2 = bbox_remove(v2);
	double iou = 0;
	std::vector<bbox_t> iou_1(0);
	for (int i = 0; i < v1.size(); i++)
	{
		for (int j = 0; j < v2.size(); j++)
		{
			double right = min(v1[i].x + v1[i].w, v2[j].x + v2[j].w);
			double bottom = min(v1[i].y + v1[i].h, v2[j].y + v2[j].h);
			double left = max(v1[i].x, v2[j].x);
			double top = max(v1[i].y, v2[j].y);
			if (right <= left || bottom <= top)
				continue;
			double inter = (right - left) * (bottom - top);
			iou = (inter) / (v1[i].w * v1[i].h + v2[j].w * v2[j].h - inter);
			if (iou > th) {
				iou_1.push_back(v2[j]);
			}
		}
	}
	return iou_1;
}
std::vector<bbox_t> HttpServer::IU(std::vector<bbox_t> v1, std::vector<bbox_t> v2)
{
	v1.insert(v1.end(), v2.begin(), v2.end());
	std::vector<bbox_t> iou_11 = bbox_remove(v1);
	return iou_11;
}
std::vector<bbox_t> HttpServer::bbox_remove(std::vector<bbox_t> v)
{
	vector<int> mark;
	if (v.size() <= 1)
	{
		return v;
	}

	else if (v.size() == 2)
	{
		double right = min(v[0].x + v[0].w, v[1].x + v[1].w);
		double bottom = min(v[0].y + v[0].h, v[1].y + v[1].h);
		double left = max(v[0].x, v[1].x);
		double top = max(v[0].y, v[1].y);
		if (right <= left || bottom <= top)
			return v;
		double inter = (right - left) * (bottom - top);
		double iou = (inter) / (v[0].w * v[0].h + v[1].w * v[1].h - inter);
		if (iou > 0.8)
		{
			v.pop_back();
		}
		return v;
	}
	else
	{
		int i = 0, j = 0;
		for (vector<bbox_t>::iterator it1 = v.begin(); it1 != v.end() - 1; it1++, i++)
		{
			j = i + 1;
			for (vector<bbox_t>::iterator it2 = it1 + 1; it2 != v.end(); it2++, j++)
			{
				double right = min((*it1).x + (*it1).w, (*it2).x + (*it2).w);
				double bottom = min((*it1).y + (*it1).h, (*it2).y + (*it2).h);
				double left = max((*it1).x, (*it2).x);
				double top = max((*it1).y, (*it2).y);
				if (right <= left || bottom <= top)
					continue;
				double inter = (right - left) * (bottom - top);
				double iou = (inter) / ((*it1).w * (*it1).h + (*it2).w * (*it2).h - inter);
				if (iou > 0.7)
				{
					//iou_1.push_back(v2[j]);
					mark.push_back(j);
				}
			}
		}
		if (mark.size() != 0)
		{
			sort(mark.begin(), mark.end());
			vector<int>::iterator iter = unique(mark.begin(), mark.end());
			mark.erase(iter, mark.end());
			int k = 0;
			for (auto it = mark.begin(); it != mark.end(); it++, k++)
			{
				(*it) = (*it) - k;
				v.erase(v.begin() + (*it));
			}
		}
		return v;
	}
}
img_and_bbox_result HttpServer::detect_IU(const string &rtsp, const string &task_id)//检测函数
{
	img_and_bbox_result result;
	std::vector<bbox_t>  result_img_IU(0);
	int flag = 1;
    VideoCapture cap ;
	cap.open(rtsp);

	if (!cap.isOpened())
	{   
		cout << "first open rtsp" << endl;
		flag = 2;
	}
	if (flag ==2){
		cap.open(rtsp);
		if (!cap.isOpened())
		{   
			cout << "second open rtsp" << endl;
			flag = 3;
		}
	}
	if (flag == 3){
		cap.open(rtsp);
		if (!cap.isOpened())
		{   
			printf("In the detect_IU of rtsp can't open!\n");
			Logger::GetInstance().Info("task_id:" + task_id + " the url of this task_id can't open!");
			img_and_bbox_result tmp;
			// std::vector<bbox_t> vec(0);
			// tmp.box = vec;
			// cv::Mat M(600,600,CV_8UC3);
			// tmp.img = M;
			tmp.time_Stamp = 0;
			return tmp;
		}
	}

	Mat Camera_CImg;
	
	int frame_count = 0, empty_count = 0, empty_flag = 0;

	while (1)
	{
		cap >> Camera_CImg;
		if (Camera_CImg.empty())
		{
			empty_count++;
			if (empty_count == 10)
			{
				empty_flag = 1;
				break;
			}
			continue;
		}
			
		frame_count++;		
		if (frame_count == 10 || frame_count == 11 || frame_count == 12)
		{
			mutex_det.lock();
			std::vector<bbox_t>  result_img = detector.detect(Camera_CImg);
			mutex_det.unlock();

			result_img_IU = IU(result_img, result_img_IU);
		}

		if (frame_count == 13) 
		{
			break;
		}			
	}
	cap.release();
	u_int64_t time_stamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	//time_t time_stamp = time(NULL);
	if (empty_flag == 1)
	{
		img_and_bbox_result tmp;
		// std::vector<bbox_t> vec(0);
		// tmp.box = vec;
		// cv::Mat M(600,600,CV_8UC3);
		// tmp.img = M;
		tmp.time_Stamp = 0;
		Logger::GetInstance().Info("task_id:" + task_id + " the url of this task_id open error!");
		return tmp;
	}

	result.img = Camera_CImg;
	result.box = result_img_IU;
	result.time_Stamp = time_stamp;

	return result;
}

int HttpServer::MultiThread_Docker(string  rtsp,std::vector <event_type_and_point>  type_and_points , string  time,  string  id, string  interval, Json::Value  private_data)
{
	std::this_thread::get_id;
	thread::id thread_id = this_thread::get_id(); //获取当前线程id
	cout << "thread id:" << thread_id << endl;
	vector <int> event_types(0); //遍历事件类型

	bool autonomy ;
	for (auto & i:type_and_points) {
		event_types.push_back(i.event_type);
	}
	while (id_map[id])
	{
		img_and_bbox_result  result_img_IU_1;
		img_and_bbox_result  result_img_IU_5;
		img_and_bbox_result  result_img_IOU;

		std::vector<bbox_t>  temp(0);
		int times = atoi(time.c_str());
		//对应城云类别 2009：出店经营 ,2008:流动经营, 2010：非机动车乱停放 ,2011：人行道违停
		//没找到以下四种事件则不进行时间判断
		if(count(event_types.begin(),event_types.end(),2009) == 0 
			&& count(event_types.begin(),event_types.end(),2008) == 0 
			&& count(event_types.begin(),event_types.end(),2010) == 0
			&& count(event_types.begin(),event_types.end(),2011) == 0)
		{
			if (id_map[id] == 0) { //任务id已经删除　不用识别了
				id_map.erase(id);
				cout << id << " : exit!" << endl;
				return 0;
			}
			result_img_IU_1 = detect_IU(rtsp, id);   //检测到整张图片的框
			if (result_img_IU_1.time_Stamp == 0) {//time_Stamp = 0说明此任务视频流异常
				task_sta[id] = 2;//设置异常状态为２
				cout << "can't open url" << endl;
				return 0;//退出线程
			}
			//遍历检测到的每个框 是否在其对应类别所画的的区域内
			for (auto & box_i:result_img_IU_1.box) {
				 //算法检测结果 1：出店 2：流动 3：违停 6：非机动车
				for (auto & type_point:type_and_points) {	

					if (type_point.event_type == 2007 && box_i.obj_id == 5){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								//这次自制任务的检测区域一般是上次检测到的框，所以是四个顶点，找到左上角的点和左下角的点
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);//将这次检测到的框和检测区域进行IOU，阈值大于0.7的视为有效结果
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2006 && box_i.obj_id == 0){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2002 && box_i.obj_id == 9){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2004 && box_i.obj_id == 10){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2000 && box_i.obj_id == 7){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2005 && box_i.obj_id == 11){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2003 && box_i.obj_id == 4){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2001 && box_i.obj_id == 8){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务

								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
				}
			}
			Mat img_result = result_img_IU_1.img;
			vector<bbox_t> remove_rec = bbox_remove(temp);
			u_int64_t timeStamp = result_img_IU_1.time_Stamp;

			//cv::Mat d_img = draw_boxes(img_result,remove_rec,classes_names);  //画框
			if (id_map[id] == 0) {
				id_map.erase(id);
				cout << id << " : exit!" << endl;
				return 0;
			}
			if(remove_rec.size()!=0)
			{
				//string resultdata = SendReqJson(remove_rec, d_img, value_task,timeStamp);
				string resultdata = SendReqJson(remove_rec, img_result, private_data,timeStamp);
				Json::Reader reader;
				Json::Value value_log;
				reader.parse(resultdata, value_log);

				value_log.removeMember("img_full");//去除img_full（Base64格式） 后显示
				value_log["img_full"]["data"] = to_string(remove_rec.size()) + " objects";
				Logger::GetInstance().Info(Json::StyledWriter().write(value_log));
				if (id_map[id] == 0) {
					id_map.erase(id);
					cout << id << " : exit!" << endl;
					return 0;
				}
				cout << Json::StyledWriter().write(value_log) << endl;				
				SendReq(sendurl, handle_func, resultdata);
			}
			else
			{
				Json::Value send_req;
				send_req["result"][0]["obj_type"] = 9; //场景对象
				send_req["result"][0]["feature"] = "";
				send_req["result"][0]["attr_data"]["event_sort"] = 2013;
				send_req["img_full"]["data"] = Mat2Base64(img_result,"jpg");	
			
				//int64_t timems = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				u_int64_t timems = timeStamp;
				send_req["timestamp"] = double(timems);
				send_req["model_version"] = "";
				send_req["private_data"] = private_data;
				string resultdata = Json::FastWriter().write(send_req);

				send_req.removeMember("img_full");
				send_req["img_full"]["data"] = "task_id:" + id  + " :no objects";
				Logger::GetInstance().Info(Json::StyledWriter().write(send_req));
				if (id_map[id] == 0) {
					id_map.erase(id);
					cout << id << " : exit!" << endl;
					return 0;
				}
				cout << Json::StyledWriter().write(send_req) << endl;

				SendReq(sendurl, handle_func, resultdata);
			}
		}
		else //下面需要时间判断
		{
			if (id_map[id] == 0) { //任务id已经删除　不用识别了
				id_map.erase(id);
				cout << id << " : exit!" << endl;
				return 0; //直接返回　退出退出当前线程
			}
			result_img_IU_1 = detect_IU(rtsp, id);  //第一次检测
			if (result_img_IU_1.time_Stamp == 0) {
				task_sta[id] = 2;
				cout << "can't open url" << endl;
				return 0;
			}

			sleep(times);//睡眠一定时间　（时间间隔在配置文件中读取）

			if (id_map[id] == 0) { //任务id已经删除　不用识别了
				id_map.erase(id);
				cout << id << " : exit!" << endl;
				return 0;
			}
			result_img_IU_5 = detect_IU(rtsp, id);//第二次检测
			if (result_img_IU_5.time_Stamp == 0) {
				task_sta[id] = 2;
				cout << "can't open url" << endl;
				return 0;
			}
			
			result_img_IOU.box = IOU(result_img_IU_1.box, result_img_IU_5.box, 0.5);//将两次检测到的结果做iou
			//result_img_IOU.box = bbox_remove(result_img_IOU.box);

			for (auto & box_i:result_img_IOU.box) {
				for (auto & type_point:type_and_points) {

					if (type_point.event_type == 2009 && box_i.obj_id == 1){
						
						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								//这次自制任务的检测区域一般是上次检测到的框，所以是四个顶点，找到左上角的点和左下角的点
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);//将这次检测到的框和检测区域进行IOU，阈值大于0.7的视为有效结果
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2008 && box_i.obj_id == 2){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {

							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2010 && box_i.obj_id == 6){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {

							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);
								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2011 && box_i.obj_id == 3){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2007 && box_i.obj_id == 5){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {

							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2006 && box_i.obj_id == 0){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2002 && box_i.obj_id == 9){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {

							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2004 && box_i.obj_id == 10){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
							    int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2000 && box_i.obj_id == 7){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
							    int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2005 && box_i.obj_id == 11){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
							    int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2003 && box_i.obj_id == 4){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {

							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
					else if (type_point.event_type == 2001 && box_i.obj_id == 8){

						int center_x = box_i.x + ((box_i.w) / 2); //检测到的目标中心坐标ｘ
						int center_y = box_i.y + ((box_i.h) / 2); //检测到的目标中心坐标ｙ
						cv::Point centerpoint;
						centerpoint.x = center_x;
						centerpoint.y = center_y;
						for (int i = 0; i < type_point.point.size(); i++) {
							if (type_point.autonomy == true && type_point.point[i].size() == 4) { // 自制任务
								int x1 = type_point.point[i][0].x, y1 = type_point.point[i][0].y;
							    int x2 = type_point.point[i][0].x, y2 = type_point.point[i][0].y;
								for (int k = 0; k < 4; k++) {
									x1 = min(x1, type_point.point[i][k].x);
									y1 = min(y1, type_point.point[i][k].y);
									x2 = max(x2, type_point.point[i][k].x);
									y2 = max(y2, type_point.point[i][k].y);

								}
								bbox_t bb {x1, y1, x2 - x1, y2 - y1};
								vector<bbox_t> t1(0), t2(0), t3(0);
								t1.push_back(bb), t2.push_back(box_i);
								t3 = IOU(t1, t2, 0.7);
								if (t3.size() != 0) {
									temp.push_back(t3[0]);
								}
							}
							else {
								vector<vector<Point2i>> contours;
								contours.push_back(type_point.point[i]);
								double f1 = pointPolygonTest(contours[0], centerpoint, 1);
								if (f1 > 0) {
									temp.push_back(box_i);
									break;		
								}			
							}
						}	
					}
				}
			}
			Mat img_result = result_img_IU_5.img;
			vector<bbox_t> remove_rec = bbox_remove(temp); 
			u_int64_t timeStamp = result_img_IU_5.time_Stamp;

			//cv::Mat d_img = draw_boxes(img_result,remove_rec,classes_names);
			if (id_map[id] == 0) {
				id_map.erase(id);
				cout << id << " : exit!" << endl;
				return 0;
			}
			if(remove_rec.size()!=0)
			{
                //string resultdata = SendReqJson(remove_rec, d_img, private_data,timeStamp);
			    string resultdata = SendReqJson(remove_rec, img_result, private_data,timeStamp);   //在dockr容器中不需要画框
				Json::Reader reader;
				Json::Value value_log;
				reader.parse(resultdata, value_log);
				value_log.removeMember("img_full");	
				value_log["img_full"]["data"] = to_string(remove_rec.size()) + " objects";
				Logger::GetInstance().Info(Json::StyledWriter().write(value_log));
				//value_log.removeMember("img_full");
				if (id_map[id] == 0) {
					id_map.erase(id);
					cout << id << " : exit!" << endl;
					return 0;
				}				
				cout << Json::StyledWriter().write(value_log) << endl;					
				SendReq(sendurl, handle_func, resultdata);
			}
			else
			{
				Json::Value send_req;
				send_req["result"][0]["obj_type"] = 9; //场景对象
				send_req["result"][0]["feature"] = "";
				send_req["result"][0]["attr_data"]["event_sort"] = 2013;
				send_req["img_full"]["data"] = Mat2Base64(img_result,"jpg");	
			
				//int64_t timems = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				u_int64_t timems = timeStamp;
				send_req["timestamp"] = double(timems);
				send_req["model_version"] = "";
				send_req["private_data"] = private_data;
				string resultdata = Json::FastWriter().write(send_req);

				// Json::Reader reader;
				// Json::Value value_log;
				// reader.parse(resultdata, value_log);

				// value_log.removeMember("img_full");//去除img_full（Base64格式） 后显示
				// value_log["img_full"]["data"] = id  + "no objects";
				send_req.removeMember("img_full");
				send_req["img_full"]["data"] = "task_id:" + id  + " :no objects";
				Logger::GetInstance().Info(Json::StyledWriter().write(send_req));
				if (id_map[id] == 0) {
					id_map.erase(id);
					cout << id << " : exit!" << endl;
					return 0;
				}
				cout << Json::StyledWriter().write(send_req) << endl;
				SendReq(sendurl, handle_func, resultdata);
			}	    					
		}
		sleep(atoi(interval.c_str())*60);
	}
	id_map.erase(id);
	cout << id << " : exit!" << endl;
	return 0;
}

int HttpServer::Setup_addr(mg_connection *connection, const string & body)
{
	auto stringerrorjudge = [](string a)->int {if ((a.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMLOPQRSTUVWXYZ/\\;'`~!#@$%^&*()_+{}[];|?><,", 0) != -1)) { return -1; }};
	int code = 1;			
	Json::Value value;
	Json::Reader reader;
	//string a = "{\"result_port\":\"8195\",\"log_path\":\"/home/app/coredump/log/algo\"}";
	//reader.parse(body, value);
	if (set_addr_flag == 1)  //服务地址接口设置必须在任务开启前，否则错误
	{
		SendHttpRsp(connection, (SendSetup_addrJson(9, "Setup-addr must be set before the tasks,now the tasks is running,you don't set it!")).c_str());
		return -1;
	}
	else if(!reader.parse(body, value))  //是否有json数据
	{
		SendHttpRsp(connection, (SendSetup_addrJson(code, "Not Json data")).c_str());
		return -1;
	}
	else if (!value["result_ip"].isString()|| !value["result_port"].isString()|| !value["log_path"].isString())  //是否有这几种类型的数据
	{   
		SendHttpRsp(connection, (SendSetup_addrJson(code, "Json data error")).c_str());
		return -1;
	}
	else if (stringerrorjudge(value["result_ip"].asCString()) == -1)          //IP是否有非法字符
	{
		SendHttpRsp(connection, (SendSetup_addrJson(code, "result_ip error")).c_str());
		return -1;
	}
	else if (stringerrorjudge(value["result_port"].asCString()) == -1)       //port是否有非法字符
	{
		SendHttpRsp(connection, (SendSetup_addrJson(code, "result_ip error")).c_str());
		return -1;
	}
	else
	{
		std::string logpath = value["log_path"].asCString();               
		if (logpath.find("/") == string::npos)                      //log路径中是否有必须的 "/"
		{
			SendHttpRsp(connection, (SendSetup_addrJson(code, "log_path error")).c_str());
			return -1;
		}
		log_path = value["log_path"].asCString();
		result_ip = value["result_ip"].asCString();
		result_port = value["result_port"].asCString();
		sendurl = "http://" + result_ip + ":" + result_port + "/api/vias/v1/callback/result";
		Logger::GetInstance().Info(Json::StyledWriter().write(sendurl));
		ofstream configfile("./configurationfile/cofigueationfile_clientpostandlog.txt", ios::trunc);
		if (configfile.fail()) {
			cout << "error to open configurationfile\n";
			Logger::GetInstance().Info(Json::StyledWriter().write("error to open configurationfile"));
		}		
		configfile << result_ip << "\n" << result_port << "\n" << log_path << "\n";
		configfile.close();
		return 0;
	}		
}

int HttpServer::Tasks(mg_connection * connection, string body)
{
	int code = 1;
	Json::Reader reader;
	//lambda函数
	auto stringerrorjudge = [](string a)->int {if ((a.find_first_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMLOPQRSTUVWXYZ/\\;'`~!#@$%^&*()_+{}[];|?><.,", 0) != -1)) { return -1; }};
    Json::Value value;
	//reader.parse(body,value);
	//将json字符串解析成json对象：value
	
	if(urllist.size() >= max_cvs)
	{
		SendHttpRsp(connection, (SendTasksJson(9, "The maximum number of max_cvs is " +  to_string(max_cvs) + ",now it has more than " + to_string(max_cvs) + "!","-1","NULL",0)).c_str());
		return -1;
	}
	else if (!reader.parse(body,value)) 
	{  
	  	SendHttpRsp(connection, (SendTasksJson(code, "Not json data","-1","NULL",0)).c_str());
		Logger::GetInstance().Info("Not json data");
	 	return -1;
	}
	//判断是否有以下几种数据
	else if (!value["task_id"].isString() || !value["stream_url"].isString() || !value["iosd_url"].isString() || !value["task_type"].isInt() || !value["analysis_rules"].isArray())
	{
		SendHttpRsp(connection, (SendTasksJson(code, "Json data error","-1", "NULL",0)).c_str());
		return -1;
	}
	// else if (stringerrorjudge(value["task_id"].asCString()) == -1)
	// {
	// 	SendHttpRsp(connection, (SendTasksJson(code, "task_id error", "-1", value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
	// 	return -1;
	// }

	//若task_id在urllist中，说明任务已经存在
	else if (find(urllist.begin(), urllist.end(), value["task_id"].asString()) != urllist.end())
	{
		switch (value["task_type"].asInt())
		{
			case 0:
				SendHttpRsp(connection, (SendTasksJson(code, "this video is running !", value["task_id"].asString(), value["stream_url"].asCString(), value["task_type"].asInt())).c_str());
				break;
			case 1:
				SendHttpRsp(connection, (SendTasksJson(code, "this video is running !", value["task_id"].asString(), value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
				break;
			default:	
				break;	
		}	
		return -1;
	}
	else
	{
		for (unsigned int i = 0; i < value["analysis_rules"].size(); i++)
		{	
			if (!value["analysis_rules"][i]["event_type"].isInt())
			{
				SendHttpRsp(connection, (SendTasksJson(code, "event_type error", value["task_id"].asCString(), value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
				return -1;
			}
			if(value["analysis_rules"][i]["event_type"].asInt() == 2011)
			{
				for (unsigned int j = 0; j < value["analysis_rules"][i]["detect_areas"][0]["area"].size(); j++)
				{
					if (!value["analysis_rules"][i]["detect_areas"][0]["area"][j]["point_id"].isInt())
					{
						SendHttpRsp(connection, (SendTasksJson(code, "area error", value["task_id"].asCString(), value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
						return -1;
					}
					if (!value["analysis_rules"][i]["detect_areas"][0]["area"][j]["point_x"].isInt())
					{
						SendHttpRsp(connection, (SendTasksJson(code, "area error", value["task_id"].asCString(), value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
						return -1;
					}
					if (!value["analysis_rules"][i]["detect_areas"][0]["area"][j]["point_y"].isInt())
					{
						SendHttpRsp(connection, (SendTasksJson(code, "area error", value["task_id"].asCString(), value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
						return -1;
					}
				}
			}
			if (!value["analysis_rules"][i]["autonomy"].isBool())		
			{
				SendHttpRsp(connection, (SendTasksJson(code, "Json data error","-1", "NULL",0)).c_str());
				return -1;
			}
		}
		//视频分析任务标识：0：视频流分析 ，1：智能视频字符叠加（IOSD）
		// switch (value["task_type"].asInt())
		// {
		// 	case 0:
		// 	{ 
		// 		//打开三次，若还是没打开 就返回错误
		// 		string stream_url = value["stream_url"].asCString();
		// 		//string stream_url = "rtsp://admin:admin@192.168.1.130:554/car_1.mp4";
		// 		//cout << stream_url << endl;
		// 		VideoCapture cap(stream_url);	
		// 		int flag = 1;		
		// 		if (!cap.isOpened())
		// 		{
		// 			flag = 2;
		// 		}
		// 		if (flag == 2)
		// 		{
		// 			cout << "Try to open stream_url again!" << endl;
		// 			VideoCapture cap(stream_url);
		// 			if (!cap.isOpened())
		// 			{
		// 				flag = 3;
		// 			}
		// 		}
		// 		if (flag == 3)
		// 		{
		// 			cout << "Try to open stream_url three times!" << endl;
		// 			VideoCapture cap(stream_url);
		// 			if (!cap.isOpened())
		// 			{
		// 				SendHttpRsp(connection, (SendTasksJson(code, "stream_url error", value["task_id"].asCString(), value["stream_url"].asCString(), value["task_type"].asInt())).c_str());
		// 				return -1;
		// 			}
		// 		}
		// 		cap.release();
		// 		break;
		// 	}
		// 	case 1:
		// 	{
		// 		int flag = 1;
		// 		string iosd_url = value["iosd_url"].asCString();
		// 		VideoCapture cap(iosd_url);
		// 		if (!cap.isOpened())
		// 		{
		// 			flag = 2;
		// 		}
		// 		if (flag == 2)
		// 		{
		// 			cout << "Try to open iosd_url again!" << endl;
		// 			VideoCapture cap(iosd_url);
		// 			if (!cap.isOpened())
		// 			{
		// 				flag = 3;
		// 			}
		// 		}
		// 		if (flag == 3)
		// 		{
		// 			cout << "Try to open iosd_url three times!" << endl;
		// 			VideoCapture cap(iosd_url);
		// 			if (!cap.isOpened())
		// 			{
		// 				SendHttpRsp(connection, (SendTasksJson(code, "iosd_url error", value["task_id"].asCString(), value["iosd_url"].asCString(), value["task_type"].asInt())).c_str());
		// 				return -1;
		// 			}
		// 		}
		// 		cap.release();
		// 		break;
		// 	}
		// 	default:
		// 		SendHttpRsp(connection, (SendTasksJson(2, "task_type error", value["task_id"].asCString(), "NULL", value["task_type"].asInt())).c_str());
		// 		return -1;
		// }
	}
	return 0;
}

string HttpServer::SendSetup_addrJson(int code ,const string &msg)
{
	Json::Value json_setupaddr;
	json_setupaddr["code"] = Json::Value(code);
	json_setupaddr["error_msg"] = Json::Value(msg);
	string errordata = Json::FastWriter().write(json_setupaddr);
	Logger::GetInstance().Info(errordata);
	return errordata;
}

string HttpServer::SendTasksJson(int code, const string &msg, const string &task_id, const string & url, int task_type)
{
	Json::Value json_tasks;
	json_tasks["code"] = Json::Value(code);
	json_tasks["error_msg"] = Json::Value(msg);
	json_tasks["task_id"] = Json::Value(task_id);
	switch (task_type)
	{
		case 1:
			json_tasks["iosd_url"] = Json::Value(url);
			break;
		default:
			break;
	}	
	string errordata = Json::FastWriter().write(json_tasks);
	Logger::GetInstance().Info(errordata);
	cout << errordata << endl;
	return errordata;
}

// map <string, int> HttpServer::idlist_init()
// {
// 	map <string, int> id_list;
// 	for (int i = 0; i < 300; i++)
// 	{
// 		id_list.insert(map<string, int>::value_type(to_string(i), 1));
// 	}
// 	return id_list;
// }

string HttpServer::SendReqJson(const vector<bbox_t> &result, const cv::Mat & image, const Json::Value & private_data, const u_int64_t &timestamp)
{
	//auto classes = [](int obj_id)->string {switch (obj_id){case 0:return "laji";case 1:return "outManagement";case 2:return "FlowManagement";case 3:return "Illegal_parking";case 4:return "Rolling_gate";default:break;}};
	auto index = [](int obj_id)->int {
		switch (obj_id)
		{
			case 0:return 2006;
			case 1:return 2009;
			case 2:return 2008;
			case 3:return 2011;
			case 4:return 2003;
			case 5:return 2007;
			case 6:return 2010;
			case 7:return 2000;
			case 8:return 2001;
			case 9:return 2002;
			case 10:return 2004;
			case 11:return 2005;
			default:break;
		}
	};
	Json::Value send_req;

	for (unsigned int i = 0; i < result.size(); i++)
	{
		send_req["result"][i]["obj_id"] = result[i].obj_id;
		send_req["result"][i]["obj_type"] = 9; //场景对象
		send_req["result"][i]["quality"] = 100;
		send_req["result"][i]["accuracy"] = (result[i].prob)*100;
		send_req["result"][i]["feature"] = "";
		send_req["result"][i]["short_feature"] = "";
		if (result[i].obj_id == 3) {
			Mat img = image;
			string plate_license = plate_license_det(img, result[i]);
			if ( plate_license != "NULL" && plate_license != "none") {
				send_req["result"][i]["attr_data"]["PlateClass"] = "02";
				send_req["result"][i]["attr_data"]["PlateNo"] = plate_license;		
			}
		}
		send_req["result"][i]["attr_data"]["event_sort"] = index(result[i].obj_id);
		send_req["result"][i]["img_part_list"][0]["data"] = "";
		send_req["result"][i]["img_part_list"][0]["format"] = 0;
		send_req["result"][i]["img_part_list"][0]["pos_type"] = 0;
		send_req["result"][i]["img_part_list"][0]["left_x"] = result[i].x;
		send_req["result"][i]["img_part_list"][0]["left_y"] = result[i].y;
		send_req["result"][i]["img_part_list"][0]["width"] = result[i].w;
		send_req["result"][i]["img_part_list"][0]["high"] = result[i].h;
	}
	
    send_req["img_full"]["data"] = Mat2Base64(image,"jpg");	
	send_req["img_full"]["format"] = 1;
	//int64_t timems = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	u_int64_t timems = timestamp;
	send_req["timestamp"] = double(timems);
	send_req["model_version"] = "";
	send_req["private_data"] = private_data;
	string reultdata = Json::FastWriter().write(send_req);

	return reultdata;
}
	
void HttpServer::Read_file_postandlog(const string & file_name)
{
	string buffTmp;
	ifstream infile;
	infile.open(file_name);
	if (!infile.is_open()) {
		Logger::GetInstance().Info("Could not open the configure file!");
		cout << "Could not open the configure file!" << endl;
		return;
	}
	if (infile.good())
	{	
		for (int i = 0; i <= 2; i++)
		{
			getline(infile, buffTmp);
			string::iterator it;
			for (it = buffTmp.begin(); it < buffTmp.end(); it++){if (*it == '\r'){buffTmp.erase(it);it--;}}
			switch (i)
			{
			case 0:
				result_ip = buffTmp;
				break;
			case 1:
				result_port = buffTmp;
				break;
			case 2:
				log_path = buffTmp;
				break;
			default:
				break;
			}
		}
	}
	sendurl = "http://" + result_ip + ":" + result_port + "/api/vias/v1/callback/result";
	infile.close();
}
void HttpServer::Read_file_time(string file_name_1)
{
	string buffTmp;
	ifstream infile;
	infile.open(file_name_1);
	if (!infile.is_open()) {
		Logger::GetInstance().Info("Could not open the configure file!");
		cout << "Could not open the configure file!" << endl;
		//return "-1";
	}
	if (infile.good())
	{	
		for (int i = 0; i <= 1; i++)
		{
			getline(infile, buffTmp);
			string::iterator it;
			for (it = buffTmp.begin(); it < buffTmp.end(); it++) { if (*it == '\r') { buffTmp.erase(it); it--; } }
			switch (i)
			{
			case 0:
				runtime = buffTmp.substr(5, 3);
				break;
			case 1:
				interval = buffTmp.substr(9, 2);
				break;
			default:
				break;
			}
		}
	}

	
}
// string HttpServer::Read_file_imagepath(const string & file_name)
// {
// 	string buffTmp;
// 	ifstream infile;
// 	infile.open(file_name);
// 	if (!infile.is_open()) {
// 		Logger::GetInstance().Info("Could not open the configure file!");
// 		cout << "Could not open the configure file!" << endl;
// 		return "-1";
// 	}
// 	if (infile.good())
// 	{
// 		getline(infile, buffTmp);
// 	}
// 	string::iterator it;
// 	for (it = buffTmp.begin(); it < buffTmp.end(); it++) { if (*it == '\r') { buffTmp.erase(it); it--; } }
// 	infile.close();
// 	return buffTmp;
// }
void HttpServer::read_classes_names(const string & names_file)
{
	std::ifstream ifs(names_file.c_str());
	std::string line;
	while (getline(ifs, line)) 
		classes_names.push_back(line);
	ifs.close();
	// for (const auto & i:classes_names) {
	// 	cout << i << endl;
	// }
}
// cv::Mat HttpServer::draw_boxes(cv::Mat & mat_img, std::vector<bbox_t> &result_vec, std::vector<std::string> &obj_names)
// {  
// 	 cv::Mat img = mat_img;
// 	for (auto & i : result_vec) {
// 		cv::Scalar color(60, 160, 260);
// 		cv::rectangle(img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
// 		if(obj_names.size() > i.obj_id)
// 			putText(img, obj_names[i.obj_id], cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, color);
// 		if(i.track_id > 0)
// 			putText(img, std::to_string(i.track_id), cv::Point2f(i.x+5, i.y + 15), cv::FONT_HERSHEY_COMPLEX_SMALL, 1, color);
// 	}
// 	//cv::imshow("window name", img);
// 	return img;
// }
cv::Mat HttpServer::draw_boxes(cv::Mat  mat_img, std::vector<bbox_t> & result_vec, std::vector<std::string> & obj_names)
{
		int const colors[6][3] = { { 1,0,1 },{ 0,0,1 },{ 0,1,1 },{ 0,1,0 },{ 1,1,0 },{ 1,0,0 } };

	for (auto &i : result_vec) {
		cv::Scalar color = obj_id_to_color(i.obj_id);
		cv::rectangle(mat_img, cv::Rect(i.x, i.y, i.w, i.h), color, 2);
		if (obj_names.size() > i.obj_id) {
			std::string obj_name = obj_names[i.obj_id];
			if (i.track_id > 0) obj_name += " - " + std::to_string(i.track_id);
			cv::Size const text_size = getTextSize(obj_name, cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, 1, 0);
			int const max_width = (text_size.width > i.w + 2) ? text_size.width : (i.w + 2);
			cv::rectangle(mat_img, cv::Point2f(std::max((int)i.x - 1, 0), std::max((int)i.y - 30, 0)),
				cv::Point2f(std::min((int)i.x + max_width, mat_img.cols - 1), std::min((int)i.y, mat_img.rows - 1)),
				color, CV_FILLED, 8, 0);
			putText(mat_img, obj_name, cv::Point2f(i.x, i.y - 10), cv::FONT_HERSHEY_COMPLEX_SMALL, 1.2, cv::Scalar(0, 0, 0), 2);
		}
	}
	return mat_img;
}