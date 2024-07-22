#include <unistd.h>
#include <chrono>
#include <tuple>
#include <iostream>
#include <vector>
#include <map>
#include <sw/redis++/redis++.h>
#include <sw/redis++/sentinel.h>
#include <sw/redis++/connection.h>
#include <sw/redis++/connection_pool.h>
//using namespace std;
using namespace sw::redis;
int main()
{
	SentinelOptions sentinel_opts;
    // sentinel_opts.nodes = {{"127.0.0.1", 9000},
    //                     {"127.0.0.1", 9001},
    //                     {"127.0.0.1", 9002}};   // Required. List of Redis Sentinel nodes.
    sentinel_opts.nodes = {{"192.168.127.134", 26379}};// Required. List of Redis Sentinel nodes.
    // Optional. Timeout before we successfully connect to Redis Sentinel.
    // By default, the timeout is 100ms.
    sentinel_opts.connect_timeout = std::chrono::milliseconds(200);
 
    // Optional. Timeout before we successfully send request to or receive response from Redis Sentinel.
    // By default, the timeout is 100ms.
    sentinel_opts.socket_timeout = std::chrono::milliseconds(200);
 
    auto sentinel = std::make_shared<Sentinel>(sentinel_opts);
 
 
    ConnectionOptions connection_opts;
    //connection_opts.password = "auth";  // Optional. No password by default.
    connection_opts.db = 1; 
    connection_opts.connect_timeout = std::chrono::milliseconds(100);   // Required.
    connection_opts.socket_timeout = std::chrono::milliseconds(100);    // Required.
 
    ConnectionPoolOptions pool_opts;
    pool_opts.size = 3; // Optional. The default size is 1.
 
    auto redis = Redis(sentinel, "mymaster", Role::MASTER, connection_opts, pool_opts);
    Redis* p = &redis;
    std::map<std::string, std::string> hash;
    p->hgetall("PLATINFO",std::inserter(hash, hash.end()));
 
    for(auto it = hash.begin() ;it != hash.end(); it++)
    {
        std::cout <<"Plat ID："  <<it->first <<std::endl;
        std::cout <<  "Plat UserName & Password"<<it->second <<std::endl;
    }
 
    ConnectionOptions connection_opts2;
    //connection_opts.password = "auth";  // Optional. No password by default.
    connection_opts2.db = 2; 
    connection_opts2.connect_timeout = std::chrono::milliseconds(100);   // Required.
    connection_opts2.socket_timeout = std::chrono::milliseconds(100);    // Required.
 
    auto redisDB2 = Redis(sentinel, "mymaster", Role::MASTER, connection_opts2, pool_opts);
    Redis*pp  = &redisDB2;
    std::map<std::string, std::string> hashTerm;
    pp->hgetall("TERMINAL:LIST:test123456789012",std::inserter(hashTerm, hashTerm.end()));
    
    
    for(auto it1 = hashTerm.begin() ;it1 != hashTerm.end(); it1++)
    {
        std::cout <<"Plat ID："  <<it1->first <<std::endl;
        std::cout <<  "Plat UserName & Password"<<it1->second <<std::endl;
    }
 
    // 是否存在
    bool bb = p->hexists("PLATINFO","test123456789012");
    std::cout << "PLATINFO 里 存在 test123456789012:" << bb << std::endl; 
 
 
    // hget - 注意这里，OptionalString 是大部分查询命令的返回值类型，要想转为string 需要加*
    OptionalString strValue = p->hget("PLATINFO","test1234567890123");
 
    std::cout<< " test123456789012  的 details :" << *strValue << std::endl;
    std::string straa = *strValue;
    if(straa.empty())
        {
           std::cout << "we gete nothing " << std::endl ;
        }
    std::cout<< " ---- test123456789012  的 details :" << straa << std::endl;
 
    // 测试hkeys
    std::vector<std::string> vPaltIDs;
    p->hkeys("PLATINFO",std::inserter(vPaltIDs, vPaltIDs.end()));
 
    for(auto vIter = vPaltIDs.begin();vIter != vPaltIDs.end(); vIter ++)
    {
        std::cout << *vIter << std::endl;
    }
	return 0;
}
 
 
 
 
//g++ -std=c++11 -I/usr/local/include -L/usr/local/lib -Wl,-rpath=../libc++ -o app testRedisSentinel.cpp -lredis++ -lhiredis -pthread
//