#include "Manager/AccountManager.h"

#include "Engine/World.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "TimerManager.h"

namespace
{
	FString GetBaseUrl(const FString& InBaseUrl)
	{
		FString Result = InBaseUrl;
		while (Result.EndsWith(TEXT("/")))
		{
			Result.LeftChopInline(1);
		}
		return Result;
	}

	FString GetStringFieldAny(const TSharedPtr<FJsonObject>& Object, std::initializer_list<const TCHAR*> Names)
	{
		if (!Object.IsValid())
		{
			return FString();
		}
		for (const TCHAR* Name : Names)
		{
			FString Value;
			if (Object->TryGetStringField(Name, Value))
			{
				return Value;
			}
			double Number = 0.0;
			if (Object->TryGetNumberField(Name, Number))
			{
				return LexToString(static_cast<int64>(Number));
			}
		}
		return FString();
	}

	FString GetResponseMessage(const TSharedPtr<FJsonObject>& Object)
	{
		if (!Object.IsValid())
		{
			return FString();
		}
		FString Message;
		if (Object->TryGetStringField(TEXT("message"), Message) || Object->TryGetStringField(TEXT("detail"), Message))
		{
			return Message;
		}
		const TArray<TSharedPtr<FJsonValue>>* Details = nullptr;
		if (Object->TryGetArrayField(TEXT("detail"), Details) && Details)
		{
			TArray<FString> Parts;
			for (const TSharedPtr<FJsonValue>& DetailValue : *Details)
			{
				const TSharedPtr<FJsonObject> DetailObject = DetailValue.IsValid() ? DetailValue->AsObject() : nullptr;
				if (DetailObject.IsValid())
				{
					FString Error;
					if (DetailObject->TryGetStringField(TEXT("msg"), Error))
					{
						Parts.Add(Error);
					}
				}
			}
			return FString::Join(Parts, TEXT("; "));
		}
		return FString();
	}

	FAccountRoomPlayer ParseRoomPlayer(const TSharedPtr<FJsonObject>& Object)
	{
		FAccountRoomPlayer Player;
		Player.UserId = GetStringFieldAny(Object, { TEXT("user_id"), TEXT("id") });
		Player.Username = GetStringFieldAny(Object, { TEXT("username"), TEXT("name") });
		if (Object.IsValid())
		{
			Object->TryGetBoolField(TEXT("is_ready"), Player.bReady);
			if (!Object->TryGetBoolField(TEXT("is_online"), Player.bOnline))
			{
				Object->TryGetBoolField(TEXT("online"), Player.bOnline);
			}
		}
		return Player;
	}

	FAccountRoomInfo ParseRoomInfo(const TSharedPtr<FJsonObject>& Root)
	{
		FAccountRoomInfo Room;
		Room.JoinRequestId = GetStringFieldAny(Root, { TEXT("request_id"), TEXT("join_request_id") });
		Room.JoinRequestStatus = GetStringFieldAny(Root, { TEXT("request_status") });
		TSharedPtr<FJsonObject> Object = Root;
		if (Root.IsValid())
		{
			const TSharedPtr<FJsonObject>* NestedRoom = nullptr;
			if (Root->TryGetObjectField(TEXT("room"), NestedRoom) && NestedRoom && NestedRoom->IsValid())
			{
				Object = *NestedRoom;
			}
		}
		if (!Object.IsValid())
		{
			return Room;
		}
		Room.RoomId = GetStringFieldAny(Object, { TEXT("room_id"), TEXT("id") });
		Room.OwnerUserId = GetStringFieldAny(Object, { TEXT("owner_user_id"), TEXT("owner_id") });
		Room.OwnerUsername = GetStringFieldAny(Object, { TEXT("owner_username"), TEXT("owner_name") });
		Room.MapName = GetStringFieldAny(Object, { TEXT("map_name"), TEXT("map") });
		Room.Status = GetStringFieldAny(Object, { TEXT("status"), TEXT("state") });
		Room.ServerAddress = GetStringFieldAny(Object, { TEXT("server_address"), TEXT("host") });
		const FString Port = GetStringFieldAny(Object, { TEXT("server_port"), TEXT("port") });
		Room.ServerPort = FCString::Atoi(*Port);

		const TArray<TSharedPtr<FJsonValue>>* Members = nullptr;
		if (Object->TryGetArrayField(TEXT("members"), Members) && Members)
		{
			for (const TSharedPtr<FJsonValue>& MemberValue : *Members)
			{
				if (MemberValue.IsValid() && MemberValue->Type == EJson::Object)
				{
					Room.Members.Add(ParseRoomPlayer(MemberValue->AsObject()));
				}
			}
		}
		return Room;
	}

	void ParsePlayerResults(const TSharedPtr<FJsonObject>& Root, TArray<FAccountRoomPlayer>& OutPlayers)
	{
		if (!Root.IsValid())
		{
			return;
		}
		const TArray<TSharedPtr<FJsonValue>>* Players = nullptr;
		if (!Root->TryGetArrayField(TEXT("players"), Players))
		{
			Root->TryGetArrayField(TEXT("results"), Players);
		}
		if (Players)
		{
			for (const TSharedPtr<FJsonValue>& PlayerValue : *Players)
			{
				if (PlayerValue.IsValid() && PlayerValue->Type == EJson::Object)
				{
					OutPlayers.Add(ParseRoomPlayer(PlayerValue->AsObject()));
				}
			}
		}
	}

	void ParseRoomInvites(const TSharedPtr<FJsonObject>& Root, TArray<FAccountRoomInvite>& OutInvites)
	{
		if (!Root.IsValid())
		{
			return;
		}
		const TArray<TSharedPtr<FJsonValue>>* Invites = nullptr;
		if (!Root->TryGetArrayField(TEXT("invites"), Invites))
		{
			Root->TryGetArrayField(TEXT("results"), Invites);
		}
		if (Invites)
		{
			for (const TSharedPtr<FJsonValue>& InviteValue : *Invites)
			{
				if (!InviteValue.IsValid() || InviteValue->Type != EJson::Object)
				{
					continue;
				}
				const TSharedPtr<FJsonObject> InviteObject = InviteValue->AsObject();
				FAccountRoomInvite& Invite = OutInvites.AddDefaulted_GetRef();
				Invite.InviteId = GetStringFieldAny(InviteObject, { TEXT("invite_id"), TEXT("id") });
				Invite.RoomId = GetStringFieldAny(InviteObject, { TEXT("room_id") });
				Invite.InviterUsername = GetStringFieldAny(InviteObject, { TEXT("inviter_username"), TEXT("from_username") });
				Invite.MapName = GetStringFieldAny(InviteObject, { TEXT("map_name"), TEXT("map") });
			}
		}
	}

	void ParseRoomJoinRequests(const TSharedPtr<FJsonObject>& Root, TArray<FAccountRoomJoinRequest>& OutRequests)
	{
		if (!Root.IsValid())
		{
			return;
		}
		const TArray<TSharedPtr<FJsonValue>>* Requests = nullptr;
		if (!Root->TryGetArrayField(TEXT("requests"), Requests))
		{
			Root->TryGetArrayField(TEXT("results"), Requests);
		}
		if (!Requests)
		{
			return;
		}
		for (const TSharedPtr<FJsonValue>& RequestValue : *Requests)
		{
			if (!RequestValue.IsValid() || RequestValue->Type != EJson::Object)
			{
				continue;
			}
			const TSharedPtr<FJsonObject> RequestObject = RequestValue->AsObject();
			FAccountRoomJoinRequest& Request = OutRequests.AddDefaulted_GetRef();
			Request.RequestId = GetStringFieldAny(RequestObject, { TEXT("request_id"), TEXT("id") });
			Request.RequesterUsername = GetStringFieldAny(RequestObject, { TEXT("requester_username"), TEXT("username") });
			Request.MapName = GetStringFieldAny(RequestObject, { TEXT("map_name"), TEXT("map") });
		}
	}

	bool SerializeJsonObject(const TSharedPtr<FJsonObject>& JsonObject, FString& OutString)
	{
		if (!JsonObject.IsValid())
		{
			OutString.Reset();
			return true;
		}
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutString);
		return FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
	}
}

void UAccountManager::RegisterAccount(const FString& Username, const FString& Password)
{
	const FString TrimmedUsername = Username.TrimStartAndEnd();
	if (TrimmedUsername.IsEmpty() || Password.IsEmpty())
	{
		OnRegisterCompleted.Broadcast(false, 0, TEXT("用户名和密码不能为空。"));
		return;
	}

	TSharedRef<FJsonObject> RequestBody = MakeShared<FJsonObject>();
	RequestBody->SetStringField(TEXT("username"), TrimmedUsername);
	RequestBody->SetStringField(TEXT("password"), Password);

	FString SerializedBody;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SerializedBody);
	if (!FJsonSerializer::Serialize(RequestBody, Writer))
	{
		OnRegisterCompleted.Broadcast(false, 0, TEXT("无法生成注册请求。"));
		return;
	}

	const FString BaseUrl = GetBaseUrl(ApiBaseUrl);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(BaseUrl + TEXT("/v1/auth/register"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetContentAsString(SerializedBody);
	Request->SetTimeout(15.0f);
	Request->OnProcessRequestComplete().BindUObject(this, &UAccountManager::HandleRegisterResponse);

	if (!Request->ProcessRequest())
	{
		OnRegisterCompleted.Broadcast(false, 0, TEXT("无法启动注册请求，请检查 API 地址。"));
	}
}

void UAccountManager::HandleRegisterResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnRegisterCompleted.Broadcast(false, 0, TEXT("无法连接账号服务器，请检查服务器状态和网络地址。"));
		return;
	}

	const int32 StatusCode = Response->GetResponseCode();
	const FString ResponseBody = Response->GetContentAsString();
	TSharedPtr<FJsonObject> ResponseJson;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	FJsonSerializer::Deserialize(Reader, ResponseJson);
	// 同时兼容 FastAPI 的字符串 detail 和字段校验 detail 数组。
	FString Message = GetResponseMessage(ResponseJson);

	const bool bSuccess = StatusCode >= 200 && StatusCode < 300;
	if (Message.IsEmpty())
	{
		Message = bSuccess ? TEXT("注册成功。") : FString::Printf(TEXT("注册失败（HTTP %d）。"), StatusCode);
	}

	OnRegisterCompleted.Broadcast(bSuccess, StatusCode, Message);
}

void UAccountManager::LoginAccount(const FString& Username, const FString& Password)
{
	const FString TrimmedUsername = Username.TrimStartAndEnd();
	if (TrimmedUsername.IsEmpty() || Password.IsEmpty())
	{
		OnLoginCompleted.Broadcast(false, 0, TEXT("用户名和密码不能为空。"));
		return;
	}

	TSharedRef<FJsonObject> RequestBody = MakeShared<FJsonObject>();
	RequestBody->SetStringField(TEXT("username"), TrimmedUsername);
	RequestBody->SetStringField(TEXT("password"), Password);

	FString SerializedBody;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&SerializedBody);
	if (!FJsonSerializer::Serialize(RequestBody, Writer))
	{
		OnLoginCompleted.Broadcast(false, 0, TEXT("无法生成登录请求。"));
		return;
	}

	const FString BaseUrl = GetBaseUrl(ApiBaseUrl);

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(BaseUrl + TEXT("/v1/auth/login"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetContentAsString(SerializedBody);
	Request->SetTimeout(15.0f);
	Request->OnProcessRequestComplete().BindUObject(this, &UAccountManager::HandleLoginResponse);

	if (!Request->ProcessRequest())
	{
		OnLoginCompleted.Broadcast(false, 0, TEXT("无法启动登录请求，请检查 API 地址。"));
	}
}

void UAccountManager::HandleLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, const bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnLoginCompleted.Broadcast(false, 0, TEXT("无法连接账号服务器，请检查服务器状态和网络地址。"));
		return;
	}

	const int32 StatusCode = Response->GetResponseCode();
	const FString ResponseBody = Response->GetContentAsString();
	TSharedPtr<FJsonObject> ResponseJson;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseBody);
	if (FJsonSerializer::Deserialize(Reader, ResponseJson) && ResponseJson.IsValid())
	{
		AuthToken = GetStringFieldAny(ResponseJson, { TEXT("access_token"), TEXT("token") });
		const TSharedPtr<FJsonObject>* DataObject = nullptr;
		if (AuthToken.IsEmpty() && ResponseJson->TryGetObjectField(TEXT("data"), DataObject)
			&& DataObject && DataObject->IsValid())
		{
			AuthToken = GetStringFieldAny(*DataObject, { TEXT("access_token"), TEXT("token") });
		}
	}

	FString Message = GetResponseMessage(ResponseJson);
	const bool bHttpSuccess = StatusCode >= 200 && StatusCode < 300;
	const bool bSuccess = bHttpSuccess && !AuthToken.IsEmpty();
	if (!bSuccess)
	{
		AuthToken.Reset();
	}
	if (Message.IsEmpty())
	{
		Message = bSuccess ? TEXT("登录成功。")
			: bHttpSuccess ? TEXT("登录接口未返回 access_token 或 token，无法进行房间操作。")
			: FString::Printf(TEXT("登录失败（HTTP %d）。"), StatusCode);
	}
	if (bSuccess)
	{
		SendPresenceHeartbeat();
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(PresenceHeartbeatTimerHandle, this,
				&UAccountManager::SendPresenceHeartbeat, 5.0f, true);
		}
	}

	OnLoginCompleted.Broadcast(bSuccess, StatusCode, Message);
}

void UAccountManager::CreateRoom(const FString& MapName, const int32 MaxPlayers)
{
	if (MapName.TrimStartAndEnd().IsEmpty() || MaxPlayers < 2)
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::CreateRoom, false, 0,
			TEXT("地图名不能为空，房间人数至少为 2。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("map_name"), MapName.TrimStartAndEnd());
	Body->SetNumberField(TEXT("max_players"), MaxPlayers);
	SendRoomRequest(EAccountRoomOperation::CreateRoom, TEXT("POST"), TEXT("/v1/rooms"), Body);
}

void UAccountManager::SearchPlayers(const FString& UsernameQuery)
{
	const FString Query = UsernameQuery.TrimStartAndEnd();
	if (Query.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::SearchPlayers, false, 0,
			TEXT("请输入要搜索的用户名。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	const FString Path = FString::Printf(TEXT("/v1/players/search?username=%s"), *FGenericPlatformHttp::UrlEncode(Query));
	SendRoomRequest(EAccountRoomOperation::SearchPlayers, TEXT("GET"), Path);
}

void UAccountManager::InvitePlayer(const FString& RoomId, const FString& Username)
{
	if (RoomId.IsEmpty() || Username.TrimStartAndEnd().IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::InvitePlayer, false, 0,
			TEXT("房间 ID 和用户名不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetStringField(TEXT("username"), Username.TrimStartAndEnd());
	SendRoomRequest(EAccountRoomOperation::InvitePlayer, TEXT("POST"),
		FString::Printf(TEXT("/v1/rooms/%s/invites"), *FGenericPlatformHttp::UrlEncode(RoomId)), Body);
}

void UAccountManager::GetMyRoomInvites()
{
	SendRoomRequest(EAccountRoomOperation::GetInvites, TEXT("GET"), TEXT("/v1/invites"));
}

void UAccountManager::AcceptRoomInvite(const FString& InviteId)
{
	if (InviteId.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::AcceptInvite, false, 0,
			TEXT("邀请 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::AcceptInvite, TEXT("POST"),
		FString::Printf(TEXT("/v1/invites/%s/accept"), *FGenericPlatformHttp::UrlEncode(InviteId)));
}

void UAccountManager::RejectRoomInvite(const FString& InviteId)
{
	if (InviteId.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::RejectInvite, false, 0,
			TEXT("邀请 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::RejectInvite, TEXT("POST"),
		FString::Printf(TEXT("/v1/invites/%s/reject"), *FGenericPlatformHttp::UrlEncode(InviteId)));
}

void UAccountManager::GetRoomJoinRequests(const FString& RoomId)
{
	if (RoomId.IsEmpty())
	{
		OnRoomJoinRequestsReceived.Broadcast(false, 0, TEXT("房间 ID 不能为空。"), TArray<FAccountRoomJoinRequest>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::GetJoinRequests, TEXT("GET"),
		FString::Printf(TEXT("/v1/rooms/%s/join-requests"), *FGenericPlatformHttp::UrlEncode(RoomId)));
}

void UAccountManager::RespondToJoinRequest(const FString& RequestId, const bool bAccept)
{
	if (RequestId.IsEmpty())
	{
		const EAccountRoomOperation Operation = bAccept
			? EAccountRoomOperation::AcceptJoinRequest : EAccountRoomOperation::RejectJoinRequest;
		OnRoomRequestCompleted.Broadcast(Operation, false, 0,
			TEXT("加入申请 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	const EAccountRoomOperation Operation = bAccept
		? EAccountRoomOperation::AcceptJoinRequest : EAccountRoomOperation::RejectJoinRequest;
	const FString Action = bAccept ? TEXT("accept") : TEXT("reject");
	SendRoomRequest(Operation, TEXT("POST"), FString::Printf(
		TEXT("/v1/room-join-requests/%s/%s"), *FGenericPlatformHttp::UrlEncode(RequestId), *Action));
}

void UAccountManager::GetJoinRequestStatus(const FString& RequestId)
{
	if (RequestId.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::JoinRequestStatus, false, 0,
			TEXT("加入申请 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::JoinRequestStatus, TEXT("GET"),
		FString::Printf(TEXT("/v1/room-join-requests/%s"), *FGenericPlatformHttp::UrlEncode(RequestId)));
}

void UAccountManager::JoinRoom(const FString& PlayerUsername)
{
	FString NormalizedUsername = PlayerUsername;
	NormalizedUsername.TrimStartAndEndInline();
	if (NormalizedUsername.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::JoinRoom, false, 0,
			TEXT("玩家名称不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::JoinRoom, TEXT("POST"),
		FString::Printf(TEXT("/v1/players/%s/join-requests"),
			*FGenericPlatformHttp::UrlEncode(NormalizedUsername)));
}

void UAccountManager::GetRoomState(const FString& RoomId)
{
	if (RoomId.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::RoomStatus, false, 0,
			TEXT("房间 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::RoomStatus, TEXT("GET"),
		FString::Printf(TEXT("/v1/rooms/%s"), *FGenericPlatformHttp::UrlEncode(RoomId)));
}

void UAccountManager::SetRoomReady(const FString& RoomId, const bool bReady)
{
	if (RoomId.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::SetReady, false, 0,
			TEXT("房间 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	TSharedPtr<FJsonObject> Body = MakeShared<FJsonObject>();
	Body->SetBoolField(TEXT("is_ready"), bReady);
	SendRoomRequest(EAccountRoomOperation::SetReady, TEXT("POST"),
		FString::Printf(TEXT("/v1/rooms/%s/ready"), *FGenericPlatformHttp::UrlEncode(RoomId)), Body);
}

void UAccountManager::StartRoom(const FString& RoomId)
{
	if (RoomId.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::StartRoom, false, 0,
			TEXT("房间 ID 不能为空。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	SendRoomRequest(EAccountRoomOperation::StartRoom, TEXT("POST"),
		FString::Printf(TEXT("/v1/rooms/%s/start"), *FGenericPlatformHttp::UrlEncode(RoomId)));
}

void UAccountManager::BeginRoomPolling(const FString& RoomId)
{
	StopRoomPolling();
	if (RoomId.IsEmpty())
	{
		return;
	}
	bConnectionTravelRequested = false;
	ActiveRoomId = RoomId;
	GetRoomState(ActiveRoomId);
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(RoomPollingTimerHandle, this, &UAccountManager::PollRoomState, 2.0f, true);
	}
}

void UAccountManager::StopRoomPolling()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(RoomPollingTimerHandle);
	}
	ActiveRoomId.Reset();
}

void UAccountManager::PollRoomState()
{
	if (!ActiveRoomId.IsEmpty())
	{
		GetRoomState(ActiveRoomId);
	}
}

void UAccountManager::ConnectToGameServer(const FString& ServerAddress, const int32 ServerPort)
{
	if (bConnectionTravelRequested)
	{
		UE_LOG(LogTemp, Warning, TEXT("ConnectToGameServer ignored: travel already requested for room %s"), *ActiveRoomId);
		return;
	}

	if (ServerAddress.IsEmpty() || ServerPort <= 0 || ServerPort > 65535)
	{
		OnRoomRequestCompleted.Broadcast(EAccountRoomOperation::StartRoom, false, 0,
			TEXT("服务器地址或端口无效。"), FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}
	if (UWorld* World = GetWorld())
	{
		bConnectionTravelRequested = true;
		UE_LOG(LogTemp, Log, TEXT("ConnectToGameServer: room=%s address=%s:%d"), *ActiveRoomId, *ServerAddress, ServerPort);
		StopRoomPolling();
		const FString Address = FString::Printf(TEXT("%s:%d"), *ServerAddress, ServerPort);
		UGameplayStatics::OpenLevel(World, FName(*Address), true);
	}
}

void UAccountManager::SendRoomRequest(const EAccountRoomOperation Operation, const FString& Verb,
	const FString& Path, const TSharedPtr<FJsonObject>& RequestBody)
{
	if (AuthToken.IsEmpty())
	{
		OnRoomRequestCompleted.Broadcast(Operation, false, 401, TEXT("请先登录账号。"),
			FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}

	FString SerializedBody;
	if (!SerializeJsonObject(RequestBody, SerializedBody))
	{
		OnRoomRequestCompleted.Broadcast(Operation, false, 0, TEXT("无法生成房间请求数据。"),
			FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}

	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(GetBaseUrl(ApiBaseUrl) + Path);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AuthToken);
	if (!SerializedBody.IsEmpty())
	{
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetContentAsString(SerializedBody);
	}
	Request->SetTimeout(15.0f);
	Request->OnProcessRequestComplete().BindUObject(this, &UAccountManager::HandleRoomResponse, Operation);
	if (!Request->ProcessRequest())
	{
		OnRoomRequestCompleted.Broadcast(Operation, false, 0, TEXT("无法启动房间请求，请检查 API 地址。"),
			FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
	}
}

void UAccountManager::HandleRoomResponse(FHttpRequestPtr Request, FHttpResponsePtr Response,
	const bool bWasSuccessful, const EAccountRoomOperation Operation)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		OnRoomRequestCompleted.Broadcast(Operation, false, 0, TEXT("无法连接房间服务。"),
			FAccountRoomInfo(), TArray<FAccountRoomPlayer>());
		return;
	}

	const int32 StatusCode = Response->GetResponseCode();
	TSharedPtr<FJsonObject> ResponseJson;
	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseJson);
	const bool bSuccess = StatusCode >= 200 && StatusCode < 300;
	FString Message = GetResponseMessage(ResponseJson);
	if (Message.IsEmpty())
	{
		Message = bSuccess ? TEXT("请求成功。") : FString::Printf(TEXT("房间请求失败（HTTP %d）。"), StatusCode);
	}

	FAccountRoomInfo Room = ParseRoomInfo(ResponseJson);
	TArray<FAccountRoomPlayer> Players;
	if (Operation == EAccountRoomOperation::SearchPlayers)
	{
		ParsePlayerResults(ResponseJson, Players);
	}
	if (Operation == EAccountRoomOperation::GetInvites)
	{
		TArray<FAccountRoomInvite> Invites;
		ParseRoomInvites(ResponseJson, Invites);
		OnRoomInvitesReceived.Broadcast(bSuccess, StatusCode, Message, Invites);
	}
	if (Operation == EAccountRoomOperation::GetJoinRequests)
	{
		TArray<FAccountRoomJoinRequest> JoinRequests;
		ParseRoomJoinRequests(ResponseJson, JoinRequests);
		OnRoomJoinRequestsReceived.Broadcast(bSuccess, StatusCode, Message, JoinRequests);
	}

	OnRoomRequestCompleted.Broadcast(Operation, bSuccess, StatusCode, Message, Room, Players);

	const bool bRoomIsStarting = Room.Status.Equals(TEXT("starting"), ESearchCase::IgnoreCase)
		|| Room.Status.Equals(TEXT("in_game"), ESearchCase::IgnoreCase)
		|| Room.Status.Equals(TEXT("in-game"), ESearchCase::IgnoreCase);
	if (bSuccess && bRoomIsStarting && !Room.ServerAddress.IsEmpty() && Room.ServerPort > 0
		&& (Operation == EAccountRoomOperation::RoomStatus || Operation == EAccountRoomOperation::StartRoom))
	{
		ConnectToGameServer(Room.ServerAddress, Room.ServerPort);
	}
}

void UAccountManager::Deinitialize()
{
	StopRoomPolling();
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PresenceHeartbeatTimerHandle);
	}
	AuthToken.Reset();
	Super::Deinitialize();
}

void UAccountManager::SendPresenceHeartbeat()
{
	if (!AuthToken.IsEmpty())
	{
		SendRoomRequest(EAccountRoomOperation::PresenceHeartbeat, TEXT("POST"), TEXT("/v1/presence/heartbeat"));
	}
}
