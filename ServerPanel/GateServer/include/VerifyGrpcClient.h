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
    GetVarifyRsp GetVarifyCode(std::string email);

private:
    std::unique_ptr<VarifyService::Stub> _stub;

    VerifyGrpcClient();
};

#endif // !VERIFYGRPCCLIENT_H