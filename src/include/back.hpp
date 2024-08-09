#define HAND  8087
#define ServerAddr "0.0.0.0"
#define MAX_EVENTS 40

#define EXIT -11
#define SUCCESS 0
#define FAILURE -1
//测试


//登录
#define LOGIN 1
//注册
#define REGISTER 2
//退出
#define EXIT_SUCCESS1 3
//忘记密码
#define FORGET_PASSWORD 4
//登录成功
#define LOGIN_SUCCESS 5
//注册失败_用户名已存在
#define CHANGE_PASSWORD 6
//改变密码
#define REGISTER_FAILURE_PWD 7
//登录失败_用户名或密码错误
#define LOGIN_FAILURE 8
//登录成功 返回主页

//主页
#define HOME 9

//好友
//修改昵称
#define CHANGE_NAME 10
//添加好友
#define FRIEND_ADD 11
//删除好友
#define FRIEND_DELETE 12
//好友消息
#define FRIEND_ADD_2 13
//发送好友消息
#define FRIEND_MESSAGE_SEND 14
//接收好友消息
#define FRIEND_MESSAGE_RECEIVE 15
//删除好友消息
#define FRIEND_MESSAGE_DELETE 16
//注销账号1 
#define  DELETE_SELF1 17
//注销账号2
#define  DELETE_SELF2 18




//用户查询
#define  USER_QUERY  19
//好友申请列表
#define FRIEND_APPLY_LIST 20
//处理好友申请
#define FRIEND_APPLY 21
//查看好友列表
#define FRIEND_LIST 22

//私聊
// 聊天消息
#define FRIEND_MESSAGE 23
// 离线消息
#define OFFLINE_MESSAGE 24
// 历史消息
#define HISTORY_MESSAGE 25
// 好友屏蔽
#define FRIEND_SHIELD 26
// 好友取消屏蔽
#define FRIEND_UNSHIELD 27
//被屏蔽了
#define FRIEND_SHIELDED 28

//被删好友了，嘤嘤嘤
#define FRIEND_DLEED 29
//
#define MAX 100

//
//群组
//创建群聊
#define GROUP_CREATE 30
//群聊邀请成员
#define GROUP_INVITE 32
//查看群聊列表
#define GROUP_LIST 33



//群聊消息
#define GROUP_MESSAGE 31
