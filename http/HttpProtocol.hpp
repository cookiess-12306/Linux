#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

const std::string HttpSep = "\r\n";

class HttpProtocol
{
public:
    HttpProtocol(){}
    bool GetLine(std::string &str, std::string *line)
    {
        auto pos = str.find(HttpSep);
        if (pos == std::string::npos)
            return false;
        *line = str.substr(0, pos); // \r\n
        str.erase(0, pos + HttpSep.size());
        return true;
    }
    bool Deserialize(std::string &request)
    {
        std::string line;
        bool ok = GetLine(request, &line);
        if(!ok) return false;
        _req_line = line;

        while (true)
        {
            bool ok = GetLine(request, &line);
            if(ok && line.empty())
            {
                _req_content = request;
                break;
            }
            else if(ok && !line.empty())
            {
                _req_header.push_back(line);
            }
            else
            {
                break;
            }
        }
        return true;
    }

    void ParseReqHttp()
    {
        std::stringstream ss(_req_line);
        ss >> _methon >> _url >> _http_version;
    }

    void Parse()
    {
        ParseReqHttp();
    }

    void DebugHttp()
    {
        std::cout << "_req_line" << _req_line << std::endl;
        for(auto &line : _req_header)
        {
            std::cout << "---> " << line << std::endl;
        }
        std::cout << "_req_blank: " << _req_blank << std::endl;
        std::cout << "_req_content: " << _req_content << std::endl;
    }
    std::string Url() {return _url;}
    ~HttpProtocol(){}
private:
    //http的报文字段
    std::string _req_line;
    std::vector<std::string> _req_header;
    std::string _req_blank;
    std::string _req_content;

    //解析后的内容
    std::string _methon;
    std::string _url;
    std::string _http_version;

};