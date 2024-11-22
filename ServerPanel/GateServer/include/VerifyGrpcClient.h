#ifndef VERIFYGRPCCLIENT_H
#define VERIFYGRPCCLIENT_H

#include "const.h"
#include "message.grpc.pb.h"

#include <grpcpp/grpcpp.h>

using grpc::Status;
using grpc::Channel;
using grpc::ClientContext;

using message::GetVarifyReq;
using message::GetVarifyRsp;
using message::VarifyService;

class VerifyGrpcClient final : public Singleton<VerifyGrpcClient>
{
    friend class Singleton<VerifyGrpcClient>;
public:
    GetVarifyRsp GetVarifyCode(std::string email)
    {
        ClientContext context;
        GetVarifyReq request;
        GetVarifyRsp reply;

        request.set_email(email);

        Status status = _stub->GetVarifyCode(&context, request, &reply);
        if(status.ok())
        {
            return reply;
        }
        else
        {
            reply.set_error(ErrorCodes::RPCFailed);
            return reply;
        }
    }

private:
    std::unique_ptr<VarifyService::Stub> _stub;

    VerifyGrpcClient()
    {
        std::shared_ptr<Channel> channel = grpc::CreateChannel("0.0.0.0:50051", 
            grpc::InsecureChannelCredentials());
        
        _stub = VarifyService::NewStub(channel);
    }
};

#endif // !VERIFYGRPCCLIENT_H