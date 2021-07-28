#include <memory>
#include "http_server.h" 
#include <string>  
#include<thread>


using namespace std;
using namespace cv;

// 初始化HttpServer静态类成员
mg_serve_http_opts HttpServer::s_server_option;
std::string HttpServer::s_web_dir = "./web";
std::unordered_map<std::string, ReqHandler> HttpServer::s_handler_map;
std::unordered_set<mg_connection *> HttpServer::s_websocket_session_set;
vector <string> HttpServer::urllist;
ReqCallback HttpServer::s_req_callback;
int HttpServer::s_exit_flag = 0;
//Detector HttpServer::detector("./obj_12.cfg", "./obj_12_final.weights");
Detector HttpServer::detector("./obj_12.cfg", "./obj_12_last.weights");

string HttpServer::result_ip;
string HttpServer::result_port;
string HttpServer::log_path;
map <string, int> HttpServer::id_map;
unordered_map <string, int> HttpServer::task_sta;
string HttpServer::runtime;
string HttpServer::interval;
string HttpServer::sendurl;
std::vector<std::string> HttpServer::classes_names;
int HttpServer::max_cvs = 20;
mutex HttpServer::mutex_det;
int HttpServer::set_addr_flag = 0;


bool handle_fun1(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
	// do sth
	std::cout << "handle fun1" << std::endl;
	std::cout << "url: " << url << std::endl;
	std::cout << "body: " << body << std::endl;

	rsp_callback(c, "rsp1");

	return true;
}

bool handle_fun2(std::string url, std::string body, mg_connection *c, OnRspCallback rsp_callback)
{
	// do sth
	std::cout << "handle fun2" << std::endl;
	std::cout << "url: " << url << std::endl;
	std::cout << "body: " << body << std::endl;

	rsp_callback(c, "rsp2");

	return true;
}


int main(int argc, char *argv[])
{
	std::string port = "7999";  //调试端口设置为7999，在将可执行文件放入docker里面时要将7999改为80端口
	auto http_server = std::shared_ptr<HttpServer>(new HttpServer);//使用智能指针创建一个服务端对象
	http_server->Read_file_postandlog();  //读取配置文件1
	http_server->Read_file_time("./configurationfile/cofigueationfile_time.txt");//读取时间间隔配置文件
	http_server->read_classes_names("./obj_12.names");
	http_server->Init(port);  //服务初始化
    http_server->Init_plate_license_model(); //初始化车牌识别模型
	http_server->Start();  //开始监听
	http_server->Delete_plate_license_model();//释放车牌识别模型

	return 0;
}