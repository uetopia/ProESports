// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerController.h"
#include "Comp.h"
#include "MyGameInstance.h"
#include "OnlineSubsystemUtils.h"
#include "ILoginFlowModule.h"
//#include "OnlinePartyUEtopia.h"
#include "MyPlayerState.h"



// FOnlinePartyIdUEtopia
const uint8* FOnlinePartyIdUEtopiaDup::GetBytes() const
{
	return 0;
}
int32 FOnlinePartyIdUEtopiaDup::GetSize() const
{
	return 0;
}
bool FOnlinePartyIdUEtopiaDup::IsValid() const
{
	return false;
}
FString FOnlinePartyIdUEtopiaDup::ToString() const
{
	return key_id;
}
FString FOnlinePartyIdUEtopiaDup::ToDebugString() const
{
	return "";
}

//Constructor
AMyPlayerController::AMyPlayerController()
	:
	APlayerController()
{
	//... custom defaults ...
	const auto OnlineSub = IOnlineSubsystem::Get();
	check(OnlineSub);

	const auto FriendsInterface = OnlineSub->GetFriendsInterface();
	check(FriendsInterface.IsValid());

	const auto PartyInterface = OnlineSub->GetPartyInterface();
	check(PartyInterface.IsValid());

	const auto ChatInterface = OnlineSub->GetChatInterface();
	check(ChatInterface.IsValid());

	const auto TournamentInterface = OnlineSub->GetTournamentInterface();
	check(TournamentInterface.IsValid());

	FriendsInterface->AddOnFriendsChangeDelegate_Handle(0, FOnFriendsChangeDelegate::CreateUObject(this, &AMyPlayerController::OnFriendsChange));
	FriendsInterface->AddOnInviteReceivedDelegate_Handle(FOnInviteReceivedDelegate::CreateUObject(this, &AMyPlayerController::OnFriendInviteReceivedComplete));
	FriendsInterface->AddOnQueryRecentPlayersCompleteDelegate_Handle(FOnQueryRecentPlayersCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnReadRecentPlayersComplete));

	PartyInterface->AddOnPartyJoinedDelegate_Handle(FOnPartyJoinedDelegate::CreateUObject(this, &AMyPlayerController::OnPartyJoinedComplete));
	PartyInterface->AddOnPartyInviteReceivedDelegate_Handle(FOnPartyInviteReceivedDelegate::CreateUObject(this, &AMyPlayerController::OnPartyInviteReceivedComplete));
	PartyInterface->AddOnPartyInviteResponseReceivedDelegate_Handle(FOnPartyInviteResponseReceivedDelegate::CreateUObject(this, &AMyPlayerController::OnPartyInviteResponseReceivedComplete));
	PartyInterface->AddOnPartyDataReceivedDelegate_Handle(FOnPartyDataReceivedDelegate::CreateUObject(this, &AMyPlayerController::OnPartyDataReceivedComplete));

	ChatInterface->AddOnChatRoomMessageReceivedDelegate_Handle(FOnChatRoomMessageReceivedDelegate::CreateUObject(this, &AMyPlayerController::OnChatRoomMessageReceivedComplete));
	ChatInterface->AddOnChatPrivateMessageReceivedDelegate_Handle(FOnChatPrivateMessageReceivedDelegate::CreateUObject(this, &AMyPlayerController::OnChatPrivateMessageReceivedComplete));
	ChatInterface->AddOnChatRoomListReadCompleteDelegate_Handle(FOnChatRoomListReadCompleteDelegate::CreateUObject(this, &AMyPlayerController::OnChatRoomListReadComplete));

	TournamentInterface->AddOnTournamentListDataChangedDelegate_Handle(FOnTournamentListDataChangedDelegate::CreateUObject(this, &AMyPlayerController::OnTournamentListDataChanged));
	TournamentInterface->AddOnTournamentDetailsReadDelegate_Handle(FOnTournamentDetailsReadDelegate::CreateUObject(this, &AMyPlayerController::OnTournamentDetailsReadComplete));

	// Bind delegates into the OSS
	OnReadFriendsListCompleteDelegate = FOnReadFriendsListComplete::CreateUObject(this, &AMyPlayerController::OnReadFriendsComplete);
	OnCreatePartyCompleteDelegate = FOnCreatePartyComplete::CreateUObject(this, &AMyPlayerController::OnCreatePartyComplete);

	// Get the notification handler so we can tie alert notifications in.
	const auto NotificationHandler = OnlineSub->GetOnlineNotificationHandler();
	//check(NotificationHandler.IsValid());



	// Why is this not working?
	//NotificationHandler->AddPlayerNotificationBinding_Handle()

	//OnlineSub->GetOnlineNotificationTransportManager()->

	// Start a player as captain so they can join matchmaker queue without being in a party first
	IAmCaptain = true;


}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Only run this on client
	if (!IsRunningDedicatedServer())
	{
		// Check to see if we are on the EntryLevel.
		// If we are, we need to display the mainmenu widget.
		FString mapName = GetWorld()->GetMapName().Mid(GetWorld()->StreamingLevelsPrefix.Len());
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] BeginPlay mapName: %s"), *mapName);

		if (mapName == "EntryLevel")
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] BeginPlay on EntryLevel"));

			if (wMainMenu) // Check if the Asset is assigned in the blueprint.
			{
				// Create the widget and store it.
				MyMainMenu = CreateWidget<UUserWidget>(this, wMainMenu);

				// now you can use the widget directly since you have a referance for it.
				// Extra check to  make sure the pointer holds the widget.
				if (MyMainMenu)
				{
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] MainMenu Found"));
					//let add it to the view port
					MyMainMenu->AddToViewport();
				}

				//Show the Cursor.
				bShowMouseCursor = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] BeginPlay on Regular Level"));

			//Hide the Cursor.
			// TODO - you probably want to do something more fancy to allow users access to the mouse in-game.
			bShowMouseCursor = false;

		}
		// IN both cases we want to refresh our UI data
		const auto OnlineSub = IOnlineSubsystem::Get();
		if (OnlineSub)
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] Got Online Sub"));
			const auto IdentityInterface = OnlineSub->GetIdentityInterface();
			if (IdentityInterface.IsValid())
			{
				// Creating a local player where we can get the UserID from
				ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
				TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());
				if (UserId.IsValid())
				{
					const auto LoginStatus = IdentityInterface->GetLoginStatus(*UserId);
					if (LoginStatus == ELoginStatus::LoggedIn)
					{
						
						OnlineSub->GetFriendsInterface()->ReadFriendsList(0, "default");
						OnlineSub->GetFriendsInterface()->QueryRecentPlayers(*UserId, "default");
						// TODO we also want to get the party information here.
						// But the OSS has no FetchJoinedParties.
						// Can't use OnAuthenticated either.  Again no OSS support

						// Last resort - sending it as a push from the backend.

						
					}
				}
			}
		}
	}




}


void AMyPlayerController::TravelPlayer(const FString& ServerUrl)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] TransferPlayer - Client Side"));

	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] TransferPlayer ServerUrl: %s"), *ServerUrl);

	// Just testing to see if the client can just handle it without the server being involved
	// This does need a serverside check to prevent cheating, but for now this is fine.

	ClientTravel(ServerUrl, ETravelType::TRAVEL_Absolute);

}

void AMyPlayerController::RequestBeginPlay_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] RequestBeginPlay"));

	// We just want to accept the request here from the player and have the game instance do the transfer.

	UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	//TheGameInstance->RequestBeginPlay();

}
bool AMyPlayerController::RequestBeginPlay_Validate()
{
	return true;
}


void AMyPlayerController::OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyPlayerController::OnReadFriendsComplete"));
	UE_LOG(LogOnline, Log,
		TEXT("ReadFriendsList() for player (%d) was success=%d error=%s"), LocalPlayer, bWasSuccessful, *ErrorStr);

	if (bWasSuccessful)
	{
		const auto OnlineSub = IOnlineSubsystem::Get();
		TArray< TSharedRef<FOnlineFriend> > Friends;
		// Grab the friends data so we can print it out
		if (OnlineSub->GetFriendsInterface()->GetFriendsList(LocalPlayer, ListName, Friends))
		{
			UE_LOG(LogOnline, Log,
				TEXT("GetFriendsList(%d) returned %d friends"), LocalPlayer, Friends.Num());

			// Clear old entries
			MyCachedFriends.Empty();
			//InvitesToAccept.Empty();
			//FriendsToDelete.Empty();

			//GetPrimaryPlayerController()->PlayerState->

			// Log each friend's data out
			for (int32 Index = 0; Index < Friends.Num(); Index++)
			{
				const FOnlineFriend& Friend = *Friends[Index];
				const FOnlineUserPresence& Presence = Friend.GetPresence();
				UE_LOG(LogOnline, Log, TEXT("\t%s has unique id (%s)"), *Friend.GetDisplayName(), *Friend.GetUserId()->ToDebugString());
				UE_LOG(LogOnline, Log, TEXT("\t\t Invite status (%s)"), EInviteStatus::ToString(Friend.GetInviteStatus()));
				UE_LOG(LogOnline, Log, TEXT("\t\t Presence: %s"), *Presence.Status.StatusStr);
				UE_LOG(LogOnline, Log, TEXT("\t\t State: %s"), EOnlinePresenceState::ToString(Presence.Status.State));
				UE_LOG(LogOnline, Log, TEXT("\t\t bIsOnline (%s)"), Presence.bIsOnline ? TEXT("true") : TEXT("false"));
				UE_LOG(LogOnline, Log, TEXT("\t\t bIsPlaying (%s)"), Presence.bIsPlaying ? TEXT("true") : TEXT("false"));
				UE_LOG(LogOnline, Log, TEXT("\t\t bIsPlayingThisGame (%s)"), Presence.bIsPlayingThisGame ? TEXT("true") : TEXT("false"));
				UE_LOG(LogOnline, Log, TEXT("\t\t bIsJoinable (%s)"), Presence.bIsJoinable ? TEXT("true") : TEXT("false"));
				UE_LOG(LogOnline, Log, TEXT("\t\t bHasVoiceSupport (%s)"), Presence.bHasVoiceSupport ? TEXT("true") : TEXT("false"));

				FMyFriend ThisFriend;
				ThisFriend.playerTitle = Friend.GetDisplayName();
				ThisFriend.playerKeyId = Friend.GetUserId()->ToDebugString();
				ThisFriend.bIsOnline = Presence.bIsOnline;
				ThisFriend.bIsPlayingThisGame = Presence.bIsPlayingThisGame;

				MyCachedFriends.Add(ThisFriend);

				// keep track of pending invites from the list and accept them
				if (Friend.GetInviteStatus() == EInviteStatus::PendingInbound)
				{
					//InvitesToAccept.AddUnique(Friend.GetUserId());
				}
				// keep track of list of friends to delete
				//FriendsToDelete.AddUnique(Friend.GetUserId());
			}
		}
		else
		{
			UE_LOG(LogOnline, Log,
				TEXT("GetFriendsList(%d) failed"), LocalPlayer);
		}
	}
	// this is causing duplicates
	//OnFriendsChanged.Broadcast();

	// Can't have this in here.
	//OnFriendsChange();
}

void AMyPlayerController::OnFriendsChange() {
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnFriendsChange"));
	OnReadFriendsComplete(0, true, "default", "Success");
	OnFriendsChanged.Broadcast();
}

void AMyPlayerController::OnReadRecentPlayersComplete(const FUniqueNetId& UserId, const FString& ListName, bool bWasSuccessful, const FString& ErrorStr)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyPlayerController::OnReadRecentPlayersComplete"));
	if (bWasSuccessful)
	{
		const auto OnlineSub = IOnlineSubsystem::Get();
		TArray< TSharedRef<FOnlineRecentPlayer> > RecentPlayers;
		// Grab the friends data so we can print it out
		if (OnlineSub->GetFriendsInterface()->GetRecentPlayers(UserId, ListName, RecentPlayers))
		{
			//UE_LOG(LogOnline, Log,TEXT("GetRecentPlayers(%s) returned %d friends"), UserId.ToString(), RecentPlayers.Num());

			// Clear old entries
			MyCachedRecentPlayers.MyRecentPlayers.Empty();

			// Log each friend's data out
			for (int32 Index = 0; Index < RecentPlayers.Num(); Index++)
			{
				const FOnlineRecentPlayer& RecentPlayer = *RecentPlayers[Index];

				UE_LOG(LogOnline, Log, TEXT("\t%s has unique id (%s)"), *RecentPlayer.GetDisplayName(), *RecentPlayer.GetUserId()->ToDebugString());


				FMyRecentPlayer ThisRecentPlayer;
				ThisRecentPlayer.playerTitle = RecentPlayer.GetDisplayName();
				ThisRecentPlayer.playerKeyId = RecentPlayer.GetUserId()->ToDebugString();

				MyCachedRecentPlayers.MyRecentPlayers.Add(ThisRecentPlayer);

			}
		}
		else
		{
			//UE_LOG(LogOnline, Log,TEXT("GetRecentPlayers failed"));
		}
	}

	// TODO delegates
	OnRecentPlayersChange();
}


void AMyPlayerController::OnRecentPlayersChange() {
UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerControllere::OnRecentPlayersChange"));
OnRecentPlayersChanged.Broadcast();
}


void AMyPlayerController::InviteUserToFriends(const FString& UserKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::InviteUserToFriends"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// Fabricate a new FUniqueNetId by the userKeyId
	const TSharedPtr<const FUniqueNetId> UserToInviteId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(UserKeyId);

	FString UserToInviteKeyId = UserToInviteId.Get()->ToString();
	UE_LOG_ONLINE(Log, TEXT("UserToInviteKeyId: %s"), *UserToInviteKeyId);

	// Tell the OSS to send the invite 
	OnlineSub->GetFriendsInterface()->SendInvite(0, *UserToInviteId, "default");

}

void AMyPlayerController::OnFriendInviteReceivedComplete(const FUniqueNetId& LocalUserId, const FUniqueNetId& SenderId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnFriendInviteReceivedComplete"));

	// Get the Sender Title
	const auto OnlineSub = IOnlineSubsystem::Get();
	TSharedPtr <FOnlineFriend> SenderFriend = OnlineSub->GetFriendsInterface()->GetFriend(0, SenderId, "default");

	if (SenderFriend.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnFriendInviteReceivedComplete Found Sender record in friends list"));
		FString SenderKeyId;
		FString SenderUserTitle;
		SenderFriend->GetUserAttribute("key_id", SenderKeyId);
		UE_LOG_ONLINE(Log, TEXT("SenderKeyId: %s"), *SenderKeyId);
		SenderFriend->GetUserAttribute("title", SenderUserTitle);
		UE_LOG_ONLINE(Log, TEXT("SenderUserTitle: %s"), *SenderUserTitle);
		OnFriendInviteReceivedDisplayUI.Broadcast( SenderUserTitle, SenderKeyId);
	}
	return;
}

void AMyPlayerController::RejectFriendInvite(const FString& senderUserKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::RejectFriendInvite"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// Fabricate a new FUniqueNetId by the userKeyId
	const TSharedPtr<const FUniqueNetId> SenderUserId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(senderUserKeyId);

	OnlineSub->GetFriendsInterface()->RejectInvite(0, *SenderUserId, "default");
}

void AMyPlayerController::AcceptFriendInvite(const FString& senderUserKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::AcceptFriendInvite"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// Fabricate a new FUniqueNetId by the userKeyId
	const TSharedPtr<const FUniqueNetId> SenderUserId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(senderUserKeyId);

	OnlineSub->GetFriendsInterface()->AcceptInvite(0, *SenderUserId, "default");
}

void AMyPlayerController::CreateParty(const FString& PartyTitle, const FString& TournamentKeyId, bool TournamentParty)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::CreateParty"));
	const auto OnlineSub = IOnlineSubsystem::Get();
	FOnlinePartyTypeId PartyTypeId;
	if (TournamentParty) {
		PartyTypeId = FOnlinePartyTypeId(1);
	}
	else {
		PartyTypeId = FOnlinePartyTypeId(0);
	}
	FPartyConfiguration PartyConfiguration = FPartyConfiguration();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());
	OnlineSub->GetPartyInterface()->CreateParty(*UserId, PartyTypeId, PartyConfiguration, OnCreatePartyCompleteDelegate);
}

void AMyPlayerController::RejectPartyInvite(const FString& senderUserKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::RejectPartyInvite"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// Fabricate a new FUniqueNetId by the userKeyId
	const TSharedPtr<const FUniqueNetId> SenderUserId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(senderUserKeyId);

	OnlineSub->GetPartyInterface()->RejectInvitation(*UserId, *SenderUserId);
}

void AMyPlayerController::AcceptPartyInvite(const FString& senderUserKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::AcceptPartyInvite"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// Fabricate a new FUniqueNetId by the userKeyId
	const TSharedPtr<const FUniqueNetId> SenderUserId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(senderUserKeyId);

	OnlineSub->GetPartyInterface()->AcceptInvitation(*UserId, *SenderUserId);
}

void AMyPlayerController::OnCreatePartyComplete(const FUniqueNetId& UserId, const TSharedPtr<const FOnlinePartyId>&, const ECreatePartyCompletionResult)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnCreatePartyComplete"));
}

void AMyPlayerController::OnPartyJoinedComplete(const FUniqueNetId& UserId, const FOnlinePartyId& PartyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyJoinedComplete"));
}

void AMyPlayerController::InviteUserToParty(const FString& PartyKeyId, const FString& UserKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::InviteUserToParty"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// Fabricate a new FUniqueNetId by the userKeyId
	const TSharedPtr<const FUniqueNetId> UserToInviteId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(UserKeyId);

	/* */
	UE_LOG_ONLINE(Log, TEXT("PartyKeyId: %s"), *PartyKeyId);
	FOnlinePartyIdUEtopiaDup PartyId = FOnlinePartyIdUEtopiaDup(PartyKeyId);

	// Create a FPartyInvitationRecipient
	FPartyInvitationRecipient PartyInviteRecipient = FPartyInvitationRecipient(*UserToInviteId);

	FString UserToInviteKeyId = UserToInviteId.Get()->ToString();
	UE_LOG_ONLINE(Log, TEXT("UserToInviteKeyId: %s"), *UserToInviteKeyId);
	PartyInviteRecipient.PlatformData = UserToInviteKeyId;

	FOnlinePartyData PartyData = FOnlinePartyData();
	PartyData.SetAttribute("key_id", PartyKeyId);

	// Tell the OSS to send the invite 
	OnlineSub->GetPartyInterface()->SendInvitation(*UserId, PartyId, PartyInviteRecipient, PartyData, OnSendPartyInvitationComplete);
	
}

void AMyPlayerController::OnPartyInviteReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const FUniqueNetId& SenderId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyInviteReceivedComplete"));
	// It would be nice to be able to display a UI widget here.
	// we have to get the array from the OSS party interface

	const auto OnlineSub = IOnlineSubsystem::Get();
	OnlineSub->GetPartyInterface()->GetPendingInvites(LocalUserId, PendingInvitesArray);

	// Now we need to do something with it.  
	// Just going to stick it in a Struct

	MyCachedPartyInvitations.Empty();

	for (int32 Index = 0; Index < PendingInvitesArray.Num(); Index++)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyInviteReceivedComplete - adding invite to struct"));
		IOnlinePartyJoinInfo& PartyInfo = *PendingInvitesArray[Index];

		FMyPartyInvitation ThisPartyInvitation;
		FOnlinePartyData ThisPartyData;
		ThisPartyData = PartyInfo.GetClientData();

		FVariantData partyKeyId;
		FVariantData partyTitle ;
		FVariantData senderUserKeyId;
		FVariantData senderUserTitle;
		FVariantData recipientUserKeyId;


		ThisPartyData.GetAttribute("partyTitle", partyTitle);
		ThisPartyData.GetAttribute("senderUserKeyId", senderUserKeyId);
		ThisPartyData.GetAttribute("senderUserTitle", senderUserTitle);
		ThisPartyData.GetAttribute("partyKeyId", partyKeyId);

		partyTitle.GetValue(ThisPartyInvitation.partyTitle);
		senderUserKeyId.GetValue(ThisPartyInvitation.senderUserKeyId);
		senderUserTitle.GetValue(ThisPartyInvitation.senderUserTitle);
		partyKeyId.GetValue(ThisPartyInvitation.partyKeyId);

		MyCachedPartyInvitations.Add(ThisPartyInvitation);

	}
	OnPartyInviteReceivedDisplayUI.Broadcast();

}

void AMyPlayerController::OnPartyInviteResponseReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const FUniqueNetId& SenderId, const EInvitationResponse Response)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyInviteResponseReceivedComplete"));

	FString senderTitle = "Unknown";

	// Try to find the sender in the friends list so we can get the userTitle
	for (int32 Index = 0; Index < MyCachedFriends.Num(); Index++)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyInviteResponseReceivedComplete - Checking Friend"));
		UE_LOG_ONLINE(Log, TEXT("SenderId.ToString(): %s"), *SenderId.ToString());
		UE_LOG_ONLINE(Log, TEXT("MyCachedFriends[Index].playerKeyId: %s"), *MyCachedFriends[Index].playerKeyId);
		if (MyCachedFriends[Index].playerKeyId == SenderId.ToString())
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyInviteResponseReceivedComplete - Found Match"));
			
			senderTitle = MyCachedFriends[Index].playerTitle;
		}
	}

	FString responseString = "Unknown";
	if (Response == EInvitationResponse::Rejected)
	{
		responseString = "Rejected";
	}
	if (Response == EInvitationResponse::Accepted)
	{
		responseString = "Accepted";
	}


	OnPartyInviteResponseReceivedDisplayUI.Broadcast(senderTitle, responseString);
}

void AMyPlayerController::LeaveParty(const FString& PartyKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::LeaveParty"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	UE_LOG_ONLINE(Log, TEXT("PartyKeyId: %s"), *PartyKeyId);
	FOnlinePartyIdUEtopiaDup PartyId = FOnlinePartyIdUEtopiaDup(PartyKeyId);

	OnlineSub->GetPartyInterface()->LeaveParty(*UserId, PartyId);
}

void AMyPlayerController::OnPartyDataReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const TSharedRef<FOnlinePartyData>& PartyData)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyDataReceivedComplete"));

	// Extract our data out of the PartyData.
	// There might be a better way to do this, but I couldn't find any way to strore member data inside PartyData other than this hack-y way.
	// TODO research this.

	// First we need the general data

	FVariantData partyKeyId;
	FVariantData partyTitle;
	FVariantData partySizeMax;
	FVariantData partySizeCurrent;
	FVariantData userIsCaptain;

	PartyData->GetAttribute("title", partyTitle);
	PartyData->GetAttribute("key_id", partyKeyId);
	PartyData->GetAttribute("size_max", partySizeMax);
	PartyData->GetAttribute("size_current", partySizeCurrent);
	PartyData->GetAttribute("userIsCaptain", userIsCaptain);

	FString partySizeMaxTemp = partySizeMax.ToString();
	UE_LOG_ONLINE(Log, TEXT("partySizeMax: %s"), *partySizeMaxTemp);
	MyPartyMaxMemberCount = FCString::Atoi(*partySizeMaxTemp);

	FString partySizeCurrentTemp = partySizeCurrent.ToString();
	UE_LOG_ONLINE(Log, TEXT("partySizeCurrent: %s"), *partySizeCurrentTemp);
	MyPartyCurrentMemberCount = FCString::Atoi(*partySizeCurrentTemp);

	UE_LOG_ONLINE(Log, TEXT("userIsCaptain.ToString(): %s"), *userIsCaptain.ToString());

	IAmCaptain = false;
	if (userIsCaptain.ToString() == "true")
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyDataReceivedComplete I AM CAPTAIN"));
		IAmCaptain = true;
	}

	// Set the teamTitle in the playerState
	//AMyPlayerState* myPlayerState = Cast<AMyPlayerState>(PlayerState);
	//myPlayerState->teamTitle = partyTitle.ToString();


	// Cache the Party data in our local struct, so we can get it easily in BP
	// Clear it first
	MyCachedPartyMembers.Empty();

	FString AttributePrefix = "";

	for (int32 Index = 0; Index < MyPartyCurrentMemberCount; Index++)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyDataReceivedComplete - Adding Member"));

		FString IndexString = FString::FromInt(Index);
		AttributePrefix = "member:" + IndexString + ":";

		FVariantData userTitle;
		FVariantData userKeyId;
		FVariantData captain;
		


		FMyFriend ThisPartyMember;

		PartyData->GetAttribute(AttributePrefix + "title", userTitle);
		PartyData->GetAttribute(AttributePrefix + "key_id", userKeyId);
		PartyData->GetAttribute(AttributePrefix + "captain", captain);


		ThisPartyMember.playerTitle = userTitle.ToString();
		ThisPartyMember.playerKeyId = userKeyId.ToString();

		UE_LOG_ONLINE(Log, TEXT("captain.ToString(): %s"), *captain.ToString());

		

		// TODO add captain to partymember
		//ThisPartyMember.

		MyCachedPartyMembers.Add(ThisPartyMember);
	}

	// If there is no party, reset this player's captain status to true
	if (MyPartyCurrentMemberCount == 0)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnPartyDataReceivedComplete - No party members - resetting captain status"));
		IAmCaptain = true;
	}
	
	// delegate
	OnPartyDataReceivedUETopiaDisplayUI.Broadcast();
}


void AMyPlayerController::RefreshChatChannelList(const FUniqueNetId& LocalUserId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::RefreshChatChannelList"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Dump our cached arrays
	MyCachedChatChannels.MyChatChannels.Empty();

	// Creating a local player where we can get the UserID from
	//ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	//TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	TArray<FChatRoomId> CachedRooms;

	OnlineSub->GetChatInterface()->GetJoinedRooms(LocalUserId, CachedRooms);

	// Loop over them and get the room info 
	for (int32 Index = 0; Index < CachedRooms.Num(); Index++)
	{
		TSharedPtr<FChatRoomInfo> CachedChatRoomInfo = OnlineSub->GetChatInterface()->GetRoomInfo(LocalUserId, *CachedRooms[Index]);

		FMyChatChannel CachedChatInstance;
		CachedChatInstance.chatChannelKeyId = CachedChatRoomInfo->GetRoomId();
		CachedChatInstance.chatChannelTitle = CachedChatRoomInfo->GetSubject();

		// Populate our USTRUCT
		MyCachedChatChannels.MyChatChannels.Add(CachedChatInstance);

	}

	//DELEGATE
	OnChatChannelsChangedUETopia.Broadcast();
	
}

void AMyPlayerController::CreateChatChannel(const FString& ChatChannelTitle)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::CreateChatChannel"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	FChatRoomId ChatRoomId;  //unused at this point
	FChatRoomConfig ChatRoomConfig;

	OnlineSub->GetChatInterface()->CreateRoom(*UserId, ChatRoomId, ChatChannelTitle, ChatRoomConfig);
}

void AMyPlayerController::JoinChatChannel(const FString& ChatChannelTitle)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::JoinChatChannel"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	FChatRoomId ChatRoomId;  //unused at this point
	FChatRoomConfig ChatRoomConfig;

	OnlineSub->GetChatInterface()->JoinPublicRoom(*UserId, ChatRoomId, ChatChannelTitle, ChatRoomConfig);
}

void AMyPlayerController::OnChatRoomListReadComplete(const FUniqueNetId& LocalUserId, const FString& ErrorStr)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnChatRoomListChanged"));
	const auto OnlineSub = IOnlineSubsystem::Get();
	RefreshChatChannelList(LocalUserId);
}

void AMyPlayerController::SendChatMessage(int32 CurrentChannelIndex, FString ChatMessage)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::SendChatMessage"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Get the ChatChannel by index
	//MyCachedChatChannels.MyChatChannels[CurrentChannelIndex].chatChannelKeyId;

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	OnlineSub->GetChatInterface()->SendRoomChat(*UserId, MyCachedChatChannels.MyChatChannels[CurrentChannelIndex].chatChannelKeyId, ChatMessage);
	return;
}

void AMyPlayerController::ExitChatChannel(int32 CurrentChannelIndex)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::ExitChatChannel"));
	const auto OnlineSub = IOnlineSubsystem::Get();
	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	OnlineSub->GetChatInterface()->ExitRoom(*UserId, MyCachedChatChannels.MyChatChannels[CurrentChannelIndex].chatChannelKeyId);
	return;
}

void AMyPlayerController::OnChatRoomMessageReceivedComplete(const FUniqueNetId& SenderUserId, const FChatRoomId& RoomId, const TSharedRef<FChatMessage>& ChatMessage)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnChatRoomMessageReceivedComplete"));

	FString ChatMessageCombined = ChatMessage->GetNickname() + ": " + ChatMessage->GetBody();
	FString SenderUserKeyId = SenderUserId.ToString();

	OnChatRoomMessageReceivedDisplayUIDelegate.Broadcast(SenderUserKeyId, ChatMessageCombined, RoomId);
}


void AMyPlayerController::OnChatPrivateMessageReceivedComplete(const FUniqueNetId& SenderUserId, const TSharedRef<FChatMessage>& ChatMessage)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnChatRoomMessageReceivedComplete"));

	//FString ChatMessageCombined = ChatMessage->GetNickname() + ": " + ChatMessage->GetBody();
	FString ChatMessageCombined = ChatMessage->GetBody();
	FString SenderUserKeyId = SenderUserId.ToString();

	OnChatPrivateMessageReceivedDisplayUIDelegate.Broadcast(SenderUserKeyId, ChatMessageCombined);
}

void AMyPlayerController::FetchJoinableTournaments()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::FetchJoinableTournaments"));
	const auto OnlineSub = IOnlineSubsystem::Get();
	OnlineSub->GetTournamentInterface()->FetchJoinableTournaments();
}

void AMyPlayerController::CreateTournament(const FString& TournamentTitle, const FString& GameMode, const FString& Region, int32 TeamMin, int32 TeamMax, int32 donationAmount, int32 playerBuyIn)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::CreateTournament"));

	const auto OnlineSub = IOnlineSubsystem::Get();
	FOnlinePartyTypeId PartyTypeId;

	FTournamentConfiguration TournamentConfiguration = FTournamentConfiguration();
	TournamentConfiguration.GameMode = GameMode;
	TournamentConfiguration.MinTeams = TeamMin;
	TournamentConfiguration.MaxTeams = TeamMax;
	TournamentConfiguration.Region = Region;
	TournamentConfiguration.Title = TournamentTitle;
	TournamentConfiguration.donationAmount = donationAmount;
	TournamentConfiguration.playerBuyIn = playerBuyIn;

	// Creating a local player where we can get the UserID from
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(LocalPlayer->GetControllerId());

	// const FUniqueNetId& LocalUserId, const FOnlinePartyTypeId TournamentTypeId, const FTournamentConfiguration& TournamentConfig, const FOnCreateTournamentComplete& Delegate = FOnCreateTournamentComplete()
	
	OnlineSub->GetTournamentInterface()->CreateTournament(*UserId, PartyTypeId, TournamentConfiguration, OnCreateTournamentCompleteDelegate);
		//OnlineSub->GetPartyInterface()->CreateParty(*UserId, PartyTypeId, PartyConfiguration, OnCreatePartyCompleteDelegate);
}

void AMyPlayerController::OnTournamentListDataChanged(const FUniqueNetId& LocalUserId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA]AMyPlayerController::OnTournamentListDataChanged"));
	const auto OnlineSub = IOnlineSubsystem::Get();
	 

	TArray< TSharedRef<FOnlineTournament> > TournamentListCache;

	if (OnlineSub->GetTournamentInterface()->GetTournamentList(0, TournamentListCache))
	{
		//UE_LOG(LogOnline, Log,TEXT("GetRecentPlayers(%s) returned %d friends"), UserId.ToString(), RecentPlayers.Num());

		// Clear old entries
		MyCachedTournaments.Empty();

		// Log each friend's data out
		for (int32 Index = 0; Index < TournamentListCache.Num(); Index++)
		{
			const FOnlineTournament& ThisTournament = *TournamentListCache[Index];

			UE_LOG(LogOnline, Log, TEXT("\t%s has unique id (%s)"), *ThisTournament.GetTitle(), *ThisTournament.GetTournamentId()->ToString());


			FMyTournament ThisMyTournamentStruct;
			ThisMyTournamentStruct.tournamentKeyId = ThisTournament.GetTournamentId()->ToString();
			ThisMyTournamentStruct.tournamentTitle = ThisTournament.GetTitle();

			MyCachedTournaments.Add(ThisMyTournamentStruct);

		}
		
	}
	else
	{
		//UE_LOG(LogOnline, Log,TEXT("GetRecentPlayers failed"));

	}

	OnTournamentListChangedUETopiaDisplayUIDelegate.Broadcast();
	return;
	
}

void AMyPlayerController::ReadTournamentDetails(const FString& TournamentKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyPlayerController::ReadTournamentDetails"));

	// Dump our cached tournament data
	MyCachedTournament.tournamentTitle = "loading...";
	MyCachedTournament.tournamentKeyId = "0";
	MyCachedTournament.RoundList.Empty();
	MyCachedTournament.TeamList.Empty();

	const auto OnlineSub = IOnlineSubsystem::Get();

	FOnlinePartyIdUEtopiaDup TournamentId = FOnlinePartyIdUEtopiaDup(TournamentKeyId);

	OnlineSub->GetTournamentInterface()->ReadTournamentDetails(0, TournamentId);
}

void AMyPlayerController::OnTournamentDetailsReadComplete()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyPlayerController::OnTournamentDetailsReadComplete"));

	const auto OnlineSub = IOnlineSubsystem::Get();

	// this is just junk
	TSharedPtr<const FUniqueNetId> UserId = OnlineSub->GetIdentityInterface()->GetUniquePlayerId(0);

	TSharedPtr<FTournament> TournamentDetailsFromOSS = OnlineSub->GetTournamentInterface()->GetTournament(0, *UserId);

	// Empty out local struct arrays
	MyCachedTournament.RoundList.Empty();
	MyCachedTournament.TeamList.Empty();

	// Copy over the OSS data into our local struct
	MyCachedTournament.tournamentKeyId = TournamentDetailsFromOSS->TournamentKeyId;
	MyCachedTournament.tournamentTitle = TournamentDetailsFromOSS->Configuration.Title;
	MyCachedTournament.region = TournamentDetailsFromOSS->Configuration.Region;
	MyCachedTournament.donationAmount = TournamentDetailsFromOSS->Configuration.donationAmount;
	MyCachedTournament.playerBuyIn = TournamentDetailsFromOSS->Configuration.playerBuyIn;
	MyCachedTournament.GameMode = TournamentDetailsFromOSS->Configuration.GameMode;
	MyCachedTournament.PrizeDistributionType = TournamentDetailsFromOSS->Configuration.PrizeDistributionType;

	for (int32 Index = 0; Index < TournamentDetailsFromOSS->TeamList.Num(); Index++)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] Found Team"));
		FMyTournamentTeam TempTeamData;
		TempTeamData.KeyId = TournamentDetailsFromOSS->TeamList[Index].KeyId;
		TempTeamData.title = TournamentDetailsFromOSS->TeamList[Index].Title;

		MyCachedTournament.TeamList.Add(TempTeamData);
	}

	for (int32 RoundIndex = 0; RoundIndex < TournamentDetailsFromOSS->RoundList.Num(); RoundIndex++)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] Found Round"));
		FMyTournamentRound TempRoundData;

		TempRoundData.RoundIndex = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundIndex;

		for (int32 RoundMatchIndex = 0; RoundMatchIndex < TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList.Num(); RoundMatchIndex++)
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] Found Round Match"));
			FMyTournamentRoundMatch TempRoundMatchData;
			TempRoundMatchData.Team1KeyId = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team1KeyId;
			TempRoundMatchData.Team1Title = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team1Title;
			TempRoundMatchData.Team1Winner = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team1Winner;
			TempRoundMatchData.Team1Loser = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team1Loser;

			TempRoundMatchData.Team2KeyId = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team2KeyId;
			TempRoundMatchData.Team2Title = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team2Title;
			TempRoundMatchData.Team2Winner = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team2Winner;
			TempRoundMatchData.Team2Loser = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Team2Loser;

			TempRoundMatchData.Status = TournamentDetailsFromOSS->RoundList[RoundIndex].RoundMatchList[RoundMatchIndex].Status;

			TempRoundData.RoundMatchList.Add(TempRoundMatchData);
		}

		MyCachedTournament.RoundList.Add(TempRoundData);

	}

	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyPlayerController::OnTournamentDetailsReadComplete - triggering OnTournamentDataReadUETopiaDisplayUIDelegate"));
	OnTournamentDataReadUETopiaDisplayUIDelegate.Broadcast();

	return;
}

void AMyPlayerController::JoinTournament(const FString& TournamentKeyId)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] AMyPlayerController::JoinTournament"));
	const auto OnlineSub = IOnlineSubsystem::Get();

	// Fabricate a new FUniqueNetId by the TournamentKeyId
	const TSharedPtr<const FUniqueNetId> TournamentId = OnlineSub->GetIdentityInterface()->CreateUniquePlayerId(TournamentKeyId);

	OnlineSub->GetTournamentInterface()->JoinTournament(0, *TournamentId);

}