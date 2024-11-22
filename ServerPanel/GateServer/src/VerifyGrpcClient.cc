#include "../include/VerifyGrpcClient.h"

GetVarifyRsp VerifyGrpcClient::GetVarifyCode(std::string email)
{
    ClientContext context;
    GetVarifyReq request;
    GetVarifyRsp reply;

    request.set_email(email);

    Status status = _stub->GetVarifyCode(&context, request, &reply);
    if (status.ok())
    {
        return reply;
    }
    else
    {
        reply.set_error(ErrorCodes::RPCFailed);
        return reply;
    }
}

VerifyGrpcClient::VerifyGrpcClient()
{
    std::shared_ptr<Channel> channel = grpc::CreateChannel("127.0.0.1:50051", 
        grpc::InsecureChannelCredentials());
    
    _stub = VarifyService::NewStub(channel);
}