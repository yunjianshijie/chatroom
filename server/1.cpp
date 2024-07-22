#include <iostream>
#include <string>
#include <cstring>
#include <hiredis/hiredis.h>
int main() {
    redisContext *rc = redisConnect("127.0.0.1", 6379);
    if (rc->err) {
        redisFree(rc);
        printf("Connect to redisServer faile\n");
        return 0;
    }
    printf("Connect to redisServer Success\n");
    //
    const char *command1 = "set stest1 value1";
    redisReply *r = (redisReply *)redisCommand(rc, command1);

    if (NULL == r) {
        printf("Execut command1 failure\n");
        redisFree(rc);
        return 0;
    }
    if (!(r->type == REDIS_REPLY_STATUS && strcasecmp(r->str, "OK") == 0)) {
        printf("Failed to execute command[%s]\n", command1);
        freeReplyObject(r);
        return 0;
    }
   
    printf("Succeed to execute command[%s]\n", command1);
//命令一完成
    const char *command2 = "strlen stest1";
    r = (redisReply *)redisCommand(rc, command2);
    if (r->type != REDIS_REPLY_INTEGER) {
        printf("Failed to execute command[%s]\n", command2);
        freeReplyObject(r);
        redisFree(c);
        return;
    }
    int length = r->integer;
    freeReplyObject(r);
    printf("The length of 'stest1' is %d.\n", length);
    printf("Succeed to execute command[%s]\n", command2);

   






    printf("%%2wdadddsa");
    return 0;

}
