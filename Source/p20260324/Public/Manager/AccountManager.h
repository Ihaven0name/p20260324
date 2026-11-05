#pragma once

#include "CoreMinimal.h"
#include "TimerManager.h"
#include "HttpFwd.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AccountManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAccountRegisterCompleted, bool, bSuccess, int32, StatusCode, const FString&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAccountLoginCompleted, bool, bSuccess, int32, StatusCode, const FString&, Message);

UENUM(BlueprintType)
enum class EAccountRoomOperation : uint8
{
	CreateRoom,
	SearchPlayers,
	InvitePlayer,
	JoinRoom,
	RoomStatus,
	SetReady,
	StartRoom,
	GetInvites,
	AcceptInvite,
	RejectInvite,
	GetJoinRequests,
	JoinRequestStatus,
	AcceptJoinRequest,
	RejectJoinRequest,
	PresenceHeartbeat,
	LeaveRoom
};

USTRUCT(BlueprintType)
struct FAccountRoomPlayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString UserId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString Username;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	bool bReady = false;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	bool bOnline = false;
};

USTRUCT(BlueprintType)
struct FAccountRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString RoomId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString OwnerUserId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString OwnerUsername;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString MapName;

	/** waiting、starting、in_game、finished 等房间状态。 */
	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString Status;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	TArray<FAccountRoomPlayer> Members;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString ServerAddress;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	int32 ServerPort = 0;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString JoinRequestId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString JoinRequestStatus;
};

USTRUCT(BlueprintType)
struct FAccountRoomInvite
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString InviteId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString RoomId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString InviterUsername;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString MapName;
};

USTRUCT(BlueprintType)
struct FAccountRoomJoinRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString RequestId;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString RequesterUsername;

	UPROPERTY(BlueprintReadOnly, Category="账号服务|房间")
	FString MapName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FAccountRoomRequestCompleted,
	EAccountRoomOperation, Operation, bool, bSuccess, int32, StatusCode,
	const FString&, Message, const FAccountRoomInfo&, Room, const TArray<FAccountRoomPlayer>&, Players);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAccountRoomInvitesReceived,
	bool, bSuccess, int32, StatusCode, const FString&, Message, const TArray<FAccountRoomInvite>&, Invites);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FAccountRoomJoinRequestsReceived,
	bool, bSuccess, int32, StatusCode, const FString&, Message, const TArray<FAccountRoomJoinRequest>&, Requests);

/** 通过项目账号 API 处理注册请求。 */
UCLASS(Config=Game)
class P20260324_API UAccountManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	/** 账号 API 根地址，例如 http://192.168.1.35:8000。 */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="账号服务")
	FString ApiBaseUrl = TEXT("http://192.168.88.129:8000");

	/** 注册请求完成时广播。Message 为服务端返回的提示或错误信息。 */
	UPROPERTY(BlueprintAssignable, Category="账号服务")
	FAccountRegisterCompleted OnRegisterCompleted;

	/** 登录请求完成时广播。Message 为服务端返回的提示或错误信息。 */
	UPROPERTY(BlueprintAssignable, Category="账号服务")
	FAccountLoginCompleted OnLoginCompleted;

	/** 房间、玩家搜索及服务器分配请求完成时广播。 */
	UPROPERTY(BlueprintAssignable, Category="账号服务|房间")
	FAccountRoomRequestCompleted OnRoomRequestCompleted;

	UPROPERTY(BlueprintAssignable, Category="账号服务|房间")
	FAccountRoomInvitesReceived OnRoomInvitesReceived;

	UPROPERTY(BlueprintAssignable, Category="账号服务|房间")
	FAccountRoomJoinRequestsReceived OnRoomJoinRequestsReceived;

	/** 向 POST /v1/auth/register 发送 JSON { username, password }。 */
	UFUNCTION(BlueprintCallable, Category="账号服务", meta=(DisplayName="注册账号"))
	void RegisterAccount(const FString& Username, const FString& Password);

	/** 向 POST /v1/auth/login 发送 JSON { username, password }。 */
	UFUNCTION(BlueprintCallable, Category="账号服务", meta=(DisplayName="登录账号"))
	void LoginAccount(const FString& Username, const FString& Password);

	/** 创建房间。成功后在 OnRoomRequestCompleted 中返回 RoomId。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="创建房间"))
	void CreateRoom(const FString& MapName, int32 MaxPlayers = 4);

	/** 按用户名搜索玩家。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="搜索玩家"))
	void SearchPlayers(const FString& UsernameQuery);

	/** 向指定用户名发送房间邀请。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="邀请玩家"))
	void InvitePlayer(const FString& RoomId, const FString& Username);

	/** 查询当前登录账号收到的房间邀请。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="查询房间邀请"))
	void GetMyRoomInvites();

	/** 接受邀请并加入对应房间。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="接受房间邀请"))
	void AcceptRoomInvite(const FString& InviteId);

	/** 拒绝收到的房间邀请。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="拒绝房间邀请"))
	void RejectRoomInvite(const FString& InviteId);

	/** 按房主玩家名称申请加入其可加入的等待房间，等待房主审批。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="申请加入房间"))
	void JoinRoom(const FString& PlayerUsername);

	/** 查询房间内待审批的加入申请（仅房主可用）。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="查询加入申请"))
	void GetRoomJoinRequests(const FString& RoomId);

	/** 房主处理加入申请。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="处理加入申请"))
	void RespondToJoinRequest(const FString& RequestId, bool bAccept);

	/** 申请者查询房主的审批结果。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="查询加入申请状态"))
	void GetJoinRequestStatus(const FString& RequestId);

	/** 查询房间成员、准备状态和对局状态。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="查询房间状态"))
	void GetRoomState(const FString& RoomId);

	/** 设置当前玩家的准备状态。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="设置准备状态"))
	void SetRoomReady(const FString& RoomId, bool bReady);

	/** 离开等待中的房间；如果调用者是房主则关闭该房间。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="离开房间"))
	void LeaveRoom(const FString& RoomId);

	/** 房主请求分配专用服务器并启动所选地图。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="启动房间对局"))
	void StartRoom(const FString& RoomId);

	/** 在大厅定时查询房间状态；服务器分配完成后自动连接所有房间成员。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="开始等待房间开局"))
	void BeginRoomPolling(const FString& RoomId);

	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="停止等待房间开局"))
	void StopRoomPolling();

	/** 连接到指定的 UE 专用服务器。 */
	UFUNCTION(BlueprintCallable, Category="账号服务|房间", meta=(DisplayName="连接游戏服务器"))
	void ConnectToGameServer(const FString& ServerAddress, int32 ServerPort);

	virtual void Deinitialize() override;

private:
	void HandleRegisterResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void HandleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SendRoomRequest(EAccountRoomOperation Operation, const FString& Verb, const FString& Path,
		const TSharedPtr<class FJsonObject>& RequestBody = nullptr);
	void HandleRoomResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful,
		EAccountRoomOperation Operation);
	void PollRoomState();
	void SendPresenceHeartbeat();

	FString AuthToken;
	FString CurrentUsername;
	FString ActiveRoomId;
	bool bConnectionTravelRequested = false;
	FTimerHandle RoomPollingTimerHandle;
	FTimerHandle PresenceHeartbeatTimerHandle;
};
