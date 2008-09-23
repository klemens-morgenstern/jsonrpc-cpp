/**
 * \file tcp-server.cpp
 * \brief Simple JSON-RPC TCP server.
 * \author Sebastien Vincent
 */

#include <stdio.h>
#include <stdlib.h>

#include <jsonrpc/jsonrpc.h>

class Plop
{
  public:
    bool Print(const Json::Value& root, Json::Value& response)
    {
      std::cout << "Plop" << std::endl;
      response["jsonrpc"] = 2.0;
      response["id"] = root["id"];
      response["result"] = "success";
      return true;
    }

    Json::Value GetDescription()
    {
      Json::FastWriter writer;
      Json::Value root;
      Json::Value parameters;
      Json::Value param1;

      root["description"] = "Print Plop";

      /* type of parameter named arg1 */
      param1["type"] = "integer";
      param1["description"] = "argument 1";

      /* push it into the parameters list */
      parameters["arg1"] = param1;
      root["parameters"] = parameters;

      /* no value returned */
      root["returns"] = Json::Value::null;

      return root;
    }
};

int main(int argc, char** argv)
{
  Plop a;
  Json::Rpc::TcpServer server(std::string("127.0.0.1"), 8086);

  server.AddMethod(new Json::Rpc::RpcMethod<Plop>(a, &Plop::Print, std::string("system.print")));

  if(!server.Bind())
  {
    std::cout << "Bind failed" << std::endl;
    return -1;
  }

  if(!server.Listen())
  {
    std::cout << "Listen failed" << std::endl;
    return -1;
  }

  while(1)
  {
    server.WaitMessage(1000);
  }

  return 0;
}

