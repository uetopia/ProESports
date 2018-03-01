// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Online.h"
#include "OnlineFriendsInterface.h"
#include "OnlineTournamentsInterface.h"
#include "ILoginFlowManager.h"
#include "MyPlayerController.generated.h"

// 4.16 login flow thing
#ifndef UETOPIA_SUBSYSTEM
#define UETOPIA_SUBSYSTEM FName(TEXT("UEtopia"))
#endif

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendsChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFriendInviteReceivedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFriendInviteReceivedUETopiaDisplayUIDelegate, FString, SenderUserTitle, FString, SenderUserKeyId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRecentPlayersChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyJoinedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyInviteReceivedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyInviteReceivedUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPartyInviteResponseReceivedUETopiaDisplayUIDelegate, FString, SenderUserTitle, FString, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyDataReceivedUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChatChannelsChangedUETopiaDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnChatRoomMessageReceivedDisplayUIDelegate, FString, SenderUserKeyId, FString, ChatMessage, FString, chatRoomId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatPrivateMessageReceivedDisplayUIDelegate, FString, SenderUserKeyId, FString, ChatMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTournamentListChangedUETopiaDisplayUIDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTournamentDataReadUETopiaDisplayUIDelegate);

USTRUCT(BlueprintType)
struct FMyFriend {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsOnline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsPlayingThisGame;
};

USTRUCT(BlueprintType)
struct FMyFriends {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyFriend> MyFriends;
};

USTRUCT(BlueprintType)
struct FMyRecentPlayer {

	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerTitle;
};

USTRUCT(BlueprintType)
struct FMyRecentPlayers {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyRecentPlayer> MyRecentPlayers;
};

USTRUCT(BlueprintType)
struct FMyPartyInvitation {

	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString playerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString partyTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString partyKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString senderUserTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString senderUserKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsInvited;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsInviteAccepted;
};

USTRUCT(BlueprintType)
struct FMyPartyInvitations {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyPartyInvitation> MyPartyInvitations;
};

USTRUCT(BlueprintType)
struct FMyChatChannel {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString chatChannelKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString chatChannelTitle;
};

USTRUCT(BlueprintType)
struct FMyChatChannels {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyChatChannel> MyChatChannels;
};


/** Tournaments have a heirarchy of data:
* Tournament List
* -- Tournament
* ---- Tournament Configuration
* ---- Tournament Team List
* ------ Tournament Team
* ---- Tournament Round List
* ------ Tournament Round
* -------- Tournament Round Match List
* ---------- Tournament Round Match
*/

USTRUCT(BlueprintType)
struct FMyTournamentRoundMatch {

	GENERATED_USTRUCT_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Status;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team1Title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team1KeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team1Winner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team1Loser;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team2Title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString Team2KeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team2Winner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool Team2Loser;

};

USTRUCT(BlueprintType)
struct FMyTournamentRound {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 RoundIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournamentRoundMatch> RoundMatchList;

};

USTRUCT(BlueprintType)
struct FMyTournamentTeam {

	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString KeyId;


};


USTRUCT(BlueprintType)
struct FMyTournament {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 tournamentId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString tournamentKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString tournamentTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bIsGroupTournament;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString groupTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString PrizeDistributionType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString region;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString GameMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 donationAmount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerBuyIn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournamentTeam> TeamList;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournamentRound> RoundList;

};

/**
* Info associated with a party identifier
* THis is a duplicate that is also declared in the UEtopia plugin
* I could not get the include to work, so I'm putting this here for now
* TODO delete this and import it properly.
*/
class FOnlinePartyIdUEtopiaDup :
	public FOnlinePartyId
{
public:
	FOnlinePartyIdUEtopiaDup(const FString& InKeyId)
		: key_id(InKeyId)
	{
	}
	//~FOnlinePartyId() {};
	const uint8* GetBytes() const override;
	int32 GetSize() const override;
	bool IsValid() const override;
	FString ToString() const override;
	FString ToDebugString() const override;
private:
	const FString key_id;
};

/**
*
*/
UCLASS()
class PROESPORTS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	//Constructor
	AMyPlayerController();

public:

	// These are just temporary vars to hold data until we are ready to activate player.
	FUniqueNetIdRepl UniqueId;
	FString playerKeyId;
	int32 playerIDTemp;

	///////////////////////////////////////////////////////////
	// This section from
	// https://wiki.unrealengine.com/UMG,_Referencing_UMG_Widgets_in_Code

	// Note: that am using forward declaration Because am not including the
	// widget in the header and to prevent circular dependency.
	// you dont need to do that if you include the Widget Class in the .h
	// forward declaration is just putting "class" before the class name so the compiler know its a
	// class but its not included in the header and don't freak out. Ex.

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> wMainMenu;

	// Variable to hold the widget After Creating it.
	UUserWidget* MyMainMenu;

	// Override BeginPlay()
	virtual void BeginPlay() override;

	// Deprecated - Unused.
	// Use the login function inside the LoginFlow Widget Instead.
	//UFUNCTION(BlueprintCallable, Category = "UETOPIA")
	//	void Login();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA", Server, Reliable, WithValidation)
		void RequestBeginPlay();

	FString PlayerUniqueNetId;

	// We need to keep track of the Access token for each user.
	// The server needs to be able to send this token along with certain requests for user validation
	FString CurrentAccessTokenFromOSS;

	// Set this player's access token on the server
	UFUNCTION(Server, Reliable, WithValidation)
		void ServerSetCurrentAccessTokenFromOSS(const FString& CurrentAccessTokenFromOSSIn);

	// Tell the client to update the player access token
	UFUNCTION(Client, Reliable)
		void ClientGetCurrentAccessTokenFromOSS();

	//////////////////////////////////

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void TravelPlayer(const FString& ServerUrl);

	// Friend Functions

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void InviteUserToFriends(const FString& UserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void RejectFriendInvite(const FString& senderUserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void AcceptFriendInvite(const FString& senderUserKeyId);

	// Party Related Functions

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void CreateParty(const FString& PartyTitle, const FString& TournamentKeyId, bool TournamentParty);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void RejectPartyInvite(const FString& senderUserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void InviteUserToParty(const FString& PartyKeyId, const FString& UserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void AcceptPartyInvite(const FString& senderUserKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void LeaveParty(const FString& PartyKeyId);

	// Chat related
	//UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void RefreshChatChannelList(const FUniqueNetId& LocalUserId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void CreateChatChannel(const FString& ChatChannelTitle);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void JoinChatChannel(const FString& ChatChannelTitle);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void SendChatMessage(int32 CurrentChannelIndex, FString ChatMessage);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void ExitChatChannel(int32 CurrentChannelIndex);

	// TOURNAMENT
	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void FetchJoinableTournaments();

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void CreateTournament(const FString& TournamentTitle, const FString& GameMode, const FString& Region, int32 TeamMin, int32 TeamMax, int32 donationAmount, int32 playerBuyIn);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void ReadTournamentDetails(const FString& TournamentKeyId);

	UFUNCTION(BlueprintCallable, Category = "UETOPIA")
		void JoinTournament(const FString& TournamentKeyId);

	// MATCHMAKER
	// Can't get these working in here.
	//UFUNCTION(BlueprintCallable, Category = "UETOPIA")
	//	bool StartMatchmaking(ULocalPlayer* PlayerOwner, FString MatchType);

	//UFUNCTION(BlueprintCallable, Category = "UETOPIA")
	//	bool CancelMatchmaking(ULocalPlayer* PlayerOwner);


	// This is the delegate to grab on to in the UI
	// When it fires, it signals that you should refresh the friends list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnFriendsChangedDelegate OnFriendsChanged;

	// when this fires, the OSS has received a friend invite.
	//UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	//	FOnFriendInviteReceivedUETopiaDelegate OnFriendInviteReceived;

	// when this fires, display the friend invitation UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnFriendInviteReceivedUETopiaDisplayUIDelegate OnFriendInviteReceivedDisplayUI;

	// When this fires, it signals that you should refresh the recent player's list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnRecentPlayersChangedDelegate OnRecentPlayersChanged;
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnPartyJoinedUETopiaDelegate OnPartyJoined;
	// when this fires, the OSS has received a party invite.
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyInviteReceivedUETopiaDelegate OnPartyInviteReceived;

	// when this fires, display the party invitation UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyInviteReceivedUETopiaDisplayUIDelegate OnPartyInviteReceivedDisplayUI;

	// when this fires, display the party invitation response UI
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyInviteResponseReceivedUETopiaDisplayUIDelegate OnPartyInviteResponseReceivedDisplayUI;

	// when this fires, update the party UI widget
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnPartyDataReceivedUETopiaDisplayUIDelegate OnPartyDataReceivedUETopiaDisplayUI;
	
	// when this fires, update the Chat Channel List
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnChatChannelsChangedUETopiaDelegate OnChatChannelsChangedUETopia;

	// when this fires, add a chat room message to the chat list
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnChatRoomMessageReceivedDisplayUIDelegate OnChatRoomMessageReceivedDisplayUIDelegate;

	// And this one is a private or system chat - not coming from a room.
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnChatPrivateMessageReceivedDisplayUIDelegate OnChatPrivateMessageReceivedDisplayUIDelegate;

	// when this fires, refresh the tournament UI list 
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
	FOnTournamentListChangedUETopiaDisplayUIDelegate OnTournamentListChangedUETopiaDisplayUIDelegate;

	// when this fires, refresh the tournament details panel
	UPROPERTY(BlueprintAssignable, Category = "UETOPIA")
		FOnTournamentDataReadUETopiaDisplayUIDelegate OnTournamentDataReadUETopiaDisplayUIDelegate;

	// Handle to our delegates
	// This is bound into the online subsystem.

	FOnReadFriendsListComplete OnReadFriendsListCompleteDelegate;
	FOnQueryRecentPlayersComplete OnQueryRecentPlayersCompleteDelegate;
	FOnCreatePartyComplete OnCreatePartyCompleteDelegate;
	FOnSendPartyInvitationComplete OnSendPartyInvitationComplete;
	//FOnPartyJoined FOnPartyJoinedUETopiaDelegate;
	FOnPartyInviteReceived OnPartyInviteReceivedUEtopiaDelegate;
	//FOnPartyInviteResponseReceived OnPartyInviteResponseReceivedComplete;
	//FOnChatRoomMessageReceivedDisplayUIDelegate OnChatRoomMessageReceivedDisplayUIDelegate;

	FOnCreateTournamentComplete OnCreateTournamentCompleteDelegate;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyFriend> MyCachedFriends;
	//FMyFriends MyCachedFriends;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	FMyRecentPlayers MyCachedRecentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyPartyInvitation> MyCachedPartyInvitations;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	TArray<FMyFriend> MyCachedPartyMembers;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
	FMyChatChannels MyCachedChatChannels;

	// tournament cache
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyTournament> MyCachedTournaments;

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FMyTournament MyCachedTournament;

	// General Party Data

	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyPartyTitle;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		FString MyPartyKeyId;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		int32 MyPartyMaxMemberCount;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		int32 MyPartyCurrentMemberCount;
	UPROPERTY(BlueprintReadOnly, Category = "UETOPIA")
		bool IAmCaptain;

	ILoginFlowManager::FOnPopupDismissed OnPopupDismissedUEtopiaDelegate;
private:

	// 4.16 login flow stuff - moving to loginwidget
	//void FOnPopupDismissed(const TSharedRef<SWidget>& DisplayWidget);
	//ILoginFlowManager::FOnPopupDismissed OnDisplayLoginWidget(const TSharedRef<SWidget>& DisplayWidget);
	//void OnDismissLoginWidget();
	//FReply CancelLoginFlow();

	TSharedPtr<ILoginFlowManager> LoginFlowManager;

	//TSharedPtr<class SWebBrowser> WebBrowserWidget;

	TSharedPtr<SWidget> DisplayWidgetRef;
	

	// This is fired by the Online subsystem after it polls the friend list.
	void OnReadFriendsComplete(int32 LocalPlayer, bool bWasSuccessful, const FString& ListName, const FString& ErrorStr);

	// This function is called by a delegate whenever the friends list changes.
	// The purpose of this is to emit a delegate which we can grab onto in the UI
	void OnFriendsChange();

	// THis is called when the OSS sees a friend request
	void OnFriendInviteReceivedComplete(const FUniqueNetId& LocalUserId, const FUniqueNetId& SenderId);

	// This is fired by the Online subsystem after it polls the recent players list.
	//(FOnQueryRecentPlayersComplete, const FUniqueNetId& /*UserId*/, const FString& /*Namespace*/, bool /*bWasSuccessful*/, const FString& /*Error*/);
	//UFUNCTION()
	void OnReadRecentPlayersComplete(const FUniqueNetId& UserId, const FString& ListName, bool bWasSuccessful, const FString& ErrorStr);

	// This function is called by a delegate whenever the RecentPlayers list changes.
	// The purpose of this is to emit a delegate which we can grab onto in the UI
	void OnRecentPlayersChange();

	// This is fired by the Online subsystem after it Creates a party.
	void OnCreatePartyComplete(const FUniqueNetId& UserId, const TSharedPtr<const FOnlinePartyId>&, const ECreatePartyCompletionResult);

	void OnPartyJoinedComplete(const FUniqueNetId& UserId, const FOnlinePartyId& PartyId);

	void OnPartyInviteReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const FUniqueNetId& SenderId);

	void OnPartyInviteResponseReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const FUniqueNetId& SenderId, const EInvitationResponse Response);

	// This is fired by the online subsystem when it receives new party data
	// DECLARE_MULTICAST_DELEGATE_ThreeParams(F_PREFIX(OnPartyDataReceived), const FUniqueNetId& /*LocalUserId*/, const FOnlinePartyId& /*PartyId*/, const TSharedRef<FOnlinePartyData>& /*PartyData*/);

	void OnPartyDataReceivedComplete(const FUniqueNetId& LocalUserId, const FOnlinePartyId& PartyId, const TSharedRef<FOnlinePartyData>& PartyData);

	// THis is fired by the online subsystem when it receives notification that the user's chat room list has changed.
	// ADD THIS TO /Engine/Plugins/Online/OnlineSubsystem/Source/Public/Interfaces/OnlineChatInterface.h
	// DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChatRoomListChanged, const FUniqueNetId& /*UserId*/, const FString& /*Error*/);
	void OnChatRoomListReadComplete(const FUniqueNetId& LocalUserId, const FString& ErrorStr);

	// This is fired by the online subsystem when it receives a new chat message coming from a room
	//DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnChatRoomMessageReceived, const FUniqueNetId& /*UserId*/, const FChatRoomId& /*RoomId*/, const TSharedRef<FChatMessage>& /*ChatMessage*/);
	void OnChatRoomMessageReceivedComplete(const FUniqueNetId& SenderUserId, const FChatRoomId& RoomId, const TSharedRef<FChatMessage>& ChatMessage);

	// THis one is a private/system chat - not originating from a room
	//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChatPrivateMessageReceived, const FUniqueNetId& /*UserId*/, const TSharedRef<FChatMessage>& /*ChatMessage*/);
	void OnChatPrivateMessageReceivedComplete(const FUniqueNetId& SenderUserId, const TSharedRef<FChatMessage>& ChatMessage);

	// Tournament stuff
	//DECLARE_MULTICAST_DELEGATE_OneParam(F_PREFIX(OnTournamentListDataChanged), const FUniqueNetId& /*LocalUserId*/);
	void OnTournamentListDataChanged(const FUniqueNetId& LocalUserId);

	// The subsystem has finished reading the tournament details.  Copy them into our local struct and trigger the delegate to update the UI
	void OnTournamentDetailsReadComplete();

	// The subsystem login details have changed.  We need to update the cached token
	// DECLARE_MULTICAST_DELEGATE_FourParams(FOnLoginStatusChanged, int32 /*LocalUserNum*/, ELoginStatus::Type /*OldStatus*/, ELoginStatus::Type /*NewStatus*/, const FUniqueNetId& /*NewId*/);
	void OnLoginStatusChanged(int32 LocalUserNum, ELoginStatus::Type OldStatus, ELoginStatus::Type NewStatus, const FUniqueNetId& NewId);
	void HandleUserLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);


	TArray<TSharedRef<IOnlinePartyJoinInfo>> PendingInvitesArray;
};
