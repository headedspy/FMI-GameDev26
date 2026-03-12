#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IHttpRouter.h"
#include "IWebSocketServer.h"
#include "INetworkingWebSocket.h"
#include "MaydayGameInstance.generated.h"

UCLASS()
class UMaydayGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    virtual void Init() override;
    virtual void Shutdown() override;

private:
    TSharedPtr<IHttpRouter> HttpRouter;
    TUniquePtr<IWebSocketServer> WebSocketServer;
    TArray<INetworkingWebSocket*> ConnectedClients;

    FTimerHandle BroadcastTimerHandle;
    FTimerHandle ServerTickHandle;

    void OnClientConnected(INetworkingWebSocket* Client);
    void OnClientMessage(void* Data, int32 DataSize);
    void BroadcastDummyData();
};