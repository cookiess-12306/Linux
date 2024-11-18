#pragma once

#include <iostream>
#include <memory>

namespace ProtocolNs
{
    // 问题
    // 1.结构化数据的序列和反序列化
    // 2.还要解决用户区分报文的边界问题----数据包粘报问题

    // 方法
    // 1.自定义协议
    // 2.成熟方案解决序列和反序列化问题

    const std::string ProtSep = " ";
    const std::string LineBreakSep = "\n";

    std::string Encode(std::string &message)
    {
        std::string len = std::to_string(message.size());
        std::string package = len + LineBreakSep + message + LineBreakSep;
        return package;
    }

    bool Decode(std::string &package, std::string *massage)
    {
        auto pos = package.find(LineBreakSep);
        if (pos == std::string::npos)
            return false;
        std::string lens = package.substr(0, pos);
        int message_len = std::stoi(lens);
        int total = lens.size() + message_len + 2 * LineBreakSep.size();
        if (package.size() < total)
            return false;
        *massage = package.substr(pos+LineBreakSep.size(), message_len);
        package.erase(0, total);
        return true;
    }

    class Request
    {
    public:
        Request() :_data_x(0), _data_y(0), _oper(0)
        {
        }
        Request(int x, int y, char op) : _data_x(x), _data_y(y), _oper(op)
        {
        }
        void Debug()
        {
            std::cout << "_data_x: " << _data_x << std::endl;
            std::cout << "_data_y: " << _data_y << std::endl;
            std::cout << "_oper: " << _oper << std::endl;
        }
        void Inc()
        {
            _data_x++;
            _data_y++;
        }

        // 结构化数据->字符串数据
        bool Serialize(std::string *out)
        {
            *out = std::to_string(_data_x) + ProtSep + _oper + ProtSep + std::to_string(_data_y);
            return true;
        }

        bool Deserialize(std::string &in)
        {
            auto left = in.find(ProtSep);
            if (left == std::string::npos)
                return false;
            auto right = in.rfind(ProtSep);
            if (right == std::string::npos)
                return false;
            _data_x = std::stoi(in.substr(0, left));
            _data_y = std::stoi(in.substr(right + ProtSep.size()));
            std::string oper = in.substr(left + ProtSep.size(), right - (left + ProtSep.size()));
            if (oper.size() != 1)
                return false;
            _oper = oper[0];
            return true;
        }

        int GetX()
        {
            return _data_x;
        }

        int GetY()
        {
            return _data_y;

        }
        
        char GetOp()
        {
            return _oper;

        }
    private:
        // _data_x _oper _data_y
        // "len\nx op y\n"----报文的自描述字段
        int _data_x; // 第一个参数
        int _data_y; // 第二个参数
        char _oper;  // + - * / %
    };

    class Response
    {
    public:
        Response() :_result(0), _code(0)
        {
        }
        Response(int result, int code) : _result(result), _code(code)
        {
        }
        bool Serialize(std::string *out)
        {
            *out = std::to_string(_result) + ProtSep + std::to_string(_code);
            return true;
        }

        bool Deserialize(std::string &in)
        {
            auto pos = in.find(ProtSep);
            if (pos == std::string::npos)
                return false;
            _result = std::stoi(in.substr(0, pos));
            _code = std::stoi(in.substr(pos + ProtSep.size()));
            return true;
        }
        
        void SetResult(int res){_result = res;}       
        void SetCode(int code){_code = code;}
        int GetResult(){return _result;}
        int GetCode(){return _code;}


    private:
        // "len\n_result _code\n"
        int _result; // 运算结果
        int _code;   // 运算状态
    };

    // 简单的工厂模式，建造类设计模式
    class Factory
    {
    public:
        std::shared_ptr<Request> BuildRequest()
        {
            std::shared_ptr<Request> req = std::make_shared<Request>();
            return req;
        }
        std::shared_ptr<Request> BuildRequest(int x, int y, char op)
        {
            std::shared_ptr<Request> req = std::make_shared<Request>(x, y, op);
            return req;
        }
        std::shared_ptr<Response> BuildResponse()
        {
            std::shared_ptr<Response> resp = std::make_shared<Response>();
            return resp;
        }
        std::shared_ptr<Response> BuildResponse(int result, int code)
        {
            std::shared_ptr<Response> req = std::make_shared<Response>(result, code);
            return req;
        }
    };
}
