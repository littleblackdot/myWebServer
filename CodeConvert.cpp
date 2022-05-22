#include"CodeConvert.h"

#include "CodeConvert.h"
#include<cstring>
#include<iostream>
#include <iconv.h>


//转码部分

//16进制转10进制 
char Char2Int(char ch){
    if(ch>='0' && ch<='9')return (char)(ch-'0');
    if(ch>='a' && ch<='f')return (char)(ch-'a'+10);
    if(ch>='A' && ch<='F')return (char)(ch-'A'+10);
    return -1;
}


//获取两个字符表示的16进制数
char Str2Bin(char *str){
    char tempWord[2];
    char chn;

    tempWord[0] = Char2Int(str[0]);                                //make the B to 11 -- 00001011
    tempWord[1] = Char2Int(str[1]);                                //make the 0 to 0  -- 00000000

    chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

    return chn;
}

std::string UrlDecode(std::string str){
    std::string output="";
    char tmp[2];
    int i=0,idx=0,ndx,len=str.length();
    
    while(i<len){
            if(str[i]=='%'){
                tmp[0]=str[i+1];
                tmp[1]=str[i+2];
                output+=Str2Bin(tmp);
                i=i+3;
            }
            else if(str[i]=='+'){
                output+=' ';
                i++;
            }
            else{
                output+=str[i];
                i++;
            }
    }
        
    return output;
}

// 代码转换操作类 用于将utf-8 格式转成 gb2312
class CodeConverter {
private:
        iconv_t cd;
public:
        CodeConverter(const char *from_charset,const char *to_charset) {// 构造
            cd = iconv_open(to_charset,from_charset);
        }
                
        ~CodeConverter() {// 析构
            iconv_close(cd);
        }
                
        int convert(char *inbuf,int inlen,char *outbuf,int outlen) {// 转换输出
            char **pin = &inbuf;
            char **pout = &outbuf;

            memset(outbuf,0,outlen);
            return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
        }
};

//输入url_Utf-8 ,输出 gb2312
std::string utf8UrlCodeToGb2312Code(std::string instr){
    std::string input;
    input=UrlDecode(instr);

    const int  outlen=instr.length();
    char output[outlen];

    CodeConverter cc = CodeConverter("utf-8","gb2312");
    cc.convert((char *)input.c_str(),strlen(input.c_str()),output,outlen);

    return output;
}

//输入url_gb2312 ,输出 gb2312 实际上是直接调用 UrlDecode()
std::string gb2312UrlCodeToGb2312Code(std::string str){
        return UrlDecode(str);
}

