#pragma once

#include <iostream>
#include <memory>
#include "Protocol.hpp"

namespace CalculateNS
{
    enum ErrCode
    {
        Sucess = 0,
        DivZeroErr,
        ModZeroErr,
        UnKnowErr
    };

    class Calculate
    {
    public:
        Calculate() {}
        std::shared_ptr<ProtocolNs::Response> Cal(std::shared_ptr<ProtocolNs::Request> req)
        {
            std::shared_ptr<ProtocolNs::Response> resp = factory.BuildResponse();
            resp->SetCode(Sucess);
            switch (req->GetOp())
            {
            case '+':
                resp->SetResult(req->GetX() + req->GetY());
                break;
            case '-':
                resp->SetResult(req->GetX() - req->GetY());
                break;
            case '*':
                resp->SetResult(req->GetX() * req->GetY());
                break;
            case '/':
            {
                if (req->GetY() == 0)
                {
                    resp->SetCode(DivZeroErr);
                }
                else
                {
                    resp->SetResult(req->GetX() / req->GetY());
                }
                break;
            }

            case '%':
            {
                if (req->GetY() == 0)
                {
                    resp->SetCode(ModZeroErr);
                }
                else
                {
                    resp->SetResult(req->GetX() % req->GetY());
                }
                break;
            }
            default:
                resp->SetCode(UnKnowErr);
                break;
            }
            return resp;
        }
        ~Calculate() {}

    private:
        ProtocolNs::Factory factory;
    };
}
