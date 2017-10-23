#ifndef SERVER_IMPL_SERVICE_H
#define SERVER_IMPL_SERVICE_H

#include "common/LuaRefFwd.h"  // forward LuaRef

#include <grpc_cb_core/server/service.h>  // for Service and CallSptr
#include <LuaIntf/LuaIntf.h>  // for LuaRef

#include <vector>

namespace impl {

// Adapt lua service table to grpc_cb_core::Service.
class Service : public grpc_cb_core::Service
{
public:
    using LuaRef = LuaIntf::LuaRef;
    explicit Service(const LuaRef& luaService);
    ~Service();

public:
    using string = std::string;
    const string& GetFullName() const override;
    size_t GetMethodCount() const override;
    bool IsMethodClientStreaming(size_t iMthdIdx) const override;
    const string& GetMethodName(size_t iMthdIdx) const override;
    void CallMethod(size_t iMthdIdx, grpc_byte_buffer* pReqBuf,
        const grpc_cb_core::CallSptr& pCall) override;

private:
    void InitMethods();
    void InitMethod(size_t iMthdIdx, const LuaRef& luaMethod);
    // DEL
    //void CallMethod(size_t iMthdIdx, LuaIntf::LuaString& strReq,
    //    const grpc_cb_core::CallSptr& pCall);

private:
    LuaRef m_luaService;
    LuaRef m_luaCallMethod;
    string m_sFullName;  // like "helloworld.Greeter"

    struct MethodInfo
    {
        string sName;
        // Method request name is like: "/helloworld.Greeter/SayHello"
        string sRequestName;
        string sInputType;  // full name, like "helloworld.HelloRequest"
        string sOutputType;  // full name, like "helloworld.HelloReply"
        bool IsClientStreaming;
        bool IsServerStreaming;
    };  // struct MethodInfo
    std::vector<MethodInfo> m_vMethods;
};  // class Service

}  // namespace impl
#endif  // SERVER_IMPL_SERVICE_H
