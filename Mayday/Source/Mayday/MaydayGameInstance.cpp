#include "MaydayGameInstance.h"
#include "IHttpRouter.h"
#include "HttpServerModule.h"
#include "HttpServerResponse.h"
#include "HttpPath.h"
#include "SocketSubsystem.h"
#include "IWebSocketNetworkingModule.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"

void UMaydayGameInstance::Init()
{
    Super::Init();
    // netsh interface portproxy add v4tov4 listenaddress=0.0.0.0 listenport=8080 connectaddress=127.0.0.1 connectport=8080
    // Get local IP
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    bool bCanBind = false;
    TSharedRef<FInternetAddr> LocalAddr = SocketSubsystem->GetLocalHostAddr(*GLog, bCanBind);
    FString LocalIP = LocalAddr->ToString(false);

    FHttpServerModule& HttpModule = FHttpServerModule::Get();
    HttpRouter = HttpModule.GetHttpRouter(8080);

    FHttpRequestHandler IndexHandler = FHttpRequestHandler::CreateLambda(
        [](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            FString Html = TEXT(R"(
            <!DOCTYPE html>
            <html>
            <body>
                <h1> INDEX PAGE
            </body>
            </html>
            )");

            auto Response = FHttpServerResponse::Create(Html, TEXT("text/html"));
            OnComplete(MoveTemp(Response));
            return true;
        }
    );
    HttpRouter->BindRoute(FHttpPath(TEXT("/index")), EHttpServerRequestVerbs::VERB_GET, IndexHandler);

    FHttpRequestHandler DataHandler = FHttpRequestHandler::CreateLambda(
        [](const FHttpServerRequest& Request, const FHttpResultCallback& OnComplete)
        {
            FString Html = TEXT(R"(
            <!DOCTYPE html>
            <html>
            <body>
                <pre id="output">Connecting...</pre>
                <script>
                    const ws = new WebSocket("ws://" + location.hostname + ":8081");
                    ws.onopen = () => document.getElementById("output").textContent = "Connected!";
                    ws.onmessage = (e) => {
                        const reader = new FileReader();
                        reader.onload = () => {
                            document.getElementById("output").textContent = JSON.stringify(JSON.parse(reader.result), null, 2);
                        };
                        reader.readAsText(e.data);
                    };
                    ws.onerror = () => document.getElementById("output").textContent = "Connection failed.";
                    ws.onclose = () => document.getElementById("output").textContent = "Disconnected.";
                </script>
            </body>
            </html>
            )");

            auto Response = FHttpServerResponse::Create(Html, TEXT("text/html"));
            OnComplete(MoveTemp(Response));
            return true;
        }
    );
    HttpRouter->BindRoute(FHttpPath(TEXT("/data")), EHttpServerRequestVerbs::VERB_GET, DataHandler);


    HttpModule.StartAllListeners();

    UE_LOG(LogTemp, Log, TEXT("HTTP server started on http://%s:8080"), *LocalIP);

    // Start WebSocket server on port 8081
    IWebSocketNetworkingModule* WsModule =
        &FModuleManager::LoadModuleChecked<IWebSocketNetworkingModule>("WebSocketNetworking");

    WebSocketServer = WsModule->CreateServer();

    FWebSocketClientConnectedCallBack ConnectCallback;
    ConnectCallback.BindUObject(this, &UMaydayGameInstance::OnClientConnected);

    if (!WebSocketServer->Init(8081, ConnectCallback))
    {
        UE_LOG(LogTemp, Error, TEXT("WebSocket server failed to start on port 8081"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("WebSocket server started on port 8081"));

    // Tick WebSocket server every frame
    GetTimerManager().SetTimer(
        ServerTickHandle,
        [this]() { WebSocketServer->Tick(); },
        0.016f,
        true
    );

    // Broadcast dummy data
    GetTimerManager().SetTimer(
        BroadcastTimerHandle,
        this,
        &UMaydayGameInstance::BroadcastDummyData,
        1.0f,
        true
    );
}

void UMaydayGameInstance::Shutdown()
{
    GetTimerManager().ClearTimer(BroadcastTimerHandle);
    GetTimerManager().ClearTimer(ServerTickHandle);
    FHttpServerModule::Get().StopAllListeners();
    HttpRouter = nullptr;
    WebSocketServer = nullptr;
    Super::Shutdown();
}

void UMaydayGameInstance::OnClientConnected(INetworkingWebSocket* Client)
{
    UE_LOG(LogTemp, Log, TEXT("WebSocket client connected"));
    ConnectedClients.Add(Client);

    FWebSocketPacketReceivedCallBack ReceiveCallback;
    ReceiveCallback.BindUObject(this, &UMaydayGameInstance::OnClientMessage);
    Client->SetReceiveCallBack(ReceiveCallback);
}

void UMaydayGameInstance::OnClientMessage(void* Data, int32 DataSize)
{
    // Handle incoming messages later
}

void UMaydayGameInstance::BroadcastDummyData()
{
    TSharedPtr<FJsonObject> Root = MakeShared<FJsonObject>();

    TSharedPtr<FJsonObject> Instruments = MakeShared<FJsonObject>();
    Instruments->SetNumberField("ALT", 4200.0);
    Instruments->SetNumberField("ASI", 134.0);
    Instruments->SetNumberField("HDG", 270.0);
    Instruments->SetNumberField("VSI", -400.0);
    Instruments->SetNumberField("FUEL", 0.62);

    TSharedPtr<FJsonObject> Radar = MakeShared<FJsonObject>();
    Radar->SetNumberField("x", 1240.5);
    Radar->SetNumberField("y", 880.3);
    Radar->SetNumberField("altitude", 4200.0);
    Radar->SetNumberField("heading", 270.0);
    Radar->SetNumberField("speed", 134.0);
    Radar->SetNumberField("last_updated", FDateTime::UtcNow().ToUnixTimestamp());

    Root->SetObjectField("instruments", Instruments);
    Root->SetObjectField("radar", Radar);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(Root.ToSharedRef(), Writer);

    for (INetworkingWebSocket* Client : ConnectedClients)
    {
        FTCHARToUTF8 UTF8String(*JsonString);
        Client->Send((uint8*)UTF8String.Get(), UTF8String.Length(), false);
    }
}