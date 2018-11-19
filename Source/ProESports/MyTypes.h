// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once

//#include "GameplayEffectTypes.h"
//#include "Engine.h"
//#include "GameplayEffect.h"
//#include "GameplayAbilitiesModule.h"
#include "Online.h"
#include "MyTypes.generated.h"

// ConnectUI state is used to display the proper UI when clients connect to the server
// If they are connecting and loading, this is "Loading"
// If they have connected and have not set up a new character yet, this is CharacterCustomize
// And lastly if they have connected and the character has been set up, this is Play.
UENUM(BlueprintType)
enum class EConnectUIState : uint8
{
	Play 	UMETA(DisplayName = "Play"),
	Loading 	UMETA(DisplayName = "Loading"),
	CharacterCustomize	UMETA(DisplayName = "CharacterCustomize")
};

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
* Inventory contains the commonly used variables, as well as Inventory "stats" that can be customized for eash game without modifying the cpp code
*/


USTRUCT(BlueprintType)
struct FMyInventoryItemStat {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemStatKey;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 itemStatValue;

};

USTRUCT(BlueprintType)
struct FMyInventorySlot {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
		int32 quantity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bCanBeUsed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FText UseText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool bCanBeStacked;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 MaxStackSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<float> Attributes;
};


USTRUCT(BlueprintType)
struct FMyVendorItem {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
		int32 quantity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemDescription;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = "Empty"))
		FString itemClassPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 PricePerUnit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString buyingOfferExpires;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool buyingOffer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool selling;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool claimable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<float> Attributes;
};


USTRUCT(BlueprintType)
struct FMyCharacterRecord {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString key_id;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString description;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString characterType; // regular, hardcore, etc
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString characterState; // creating, sleeping, etc
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool characterAlive; // Permadeath.  Character can't be selected or used.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool currentlySelectedActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		UTexture2D* Icon;

};


USTRUCT(BlueprintType)
struct FMySessionSearchResult {
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString OwningUserName;

	UPROPERTY(BlueprintReadWrite)
		FString ServerTitle;
	// TODO - add more data that we care about
	UPROPERTY(BlueprintReadWrite)
		FString ServerKey;

	UPROPERTY(BlueprintReadWrite)
		int32 SearchIdx;
};

USTRUCT(BlueprintType)
struct FUserEvent {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString EventIcon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString EventType;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString EventSummary;
};

// KEY ID is actually an INT, but we'll keep it as a string for now.
USTRUCT(BlueprintType)
struct FMyServerLink {
	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString targetServerTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString targetServerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetStatusIsContinuous;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetStatusCreating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetStatusProvisioned;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetStatusOnline;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetStatusFull;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetStatusDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool permissionCanMount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool permissionCanUserTravel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool permissionCanDismount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString resourcesUsedToTravel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString resourceAmountsUsedToTravel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 currencyCostToTravel;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		float coordLocationX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		float coordLocationY;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		float coordLocationZ;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		float rotationX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		float rotationY;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		float rotationZ;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString hostConnectionLink;
	// Testing to see if this will work
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
	//	AMyServerPortalActor* ServerPortalActorReference;

	// Additions to support parallel/instanced servers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool targetInstanced;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString targetInstanceConfiguration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 targetInstancePartySizeMaximum;
};

// KEY ID is actually an INT, but we'll keep it as a string for now.
USTRUCT(BlueprintType)
struct FMyServerShard {
	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString targetServerKeyId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		bool online;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString hostConnectionLink;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 shardId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 playerCapacityMaximum;
};

USTRUCT(BlueprintType)
struct FMyServerLinks {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyServerLink> links;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyServerShard> shards;
};


USTRUCT(BlueprintType)
struct FMyServerClusterSearchResult {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		FString Title;
	UPROPERTY(BlueprintReadWrite)
		FString Description;
	// TODO - add more data that we care about
	UPROPERTY(BlueprintReadWrite)
		FString key_id_str; // previous engine versions would always add .0 to int keys, so we pass them as strings as well.
	UPROPERTY(BlueprintReadWrite)
		int32 key_id;
	UPROPERTY(BlueprintReadWrite)
		FString travelMode; // "free" or "restricted"
	// There are more fields avaialble here if you need them.  Cheack the API docs
};

USTRUCT(BlueprintType)
struct FMyServerClusterSearchResults {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyServerClusterSearchResult> server_clusters;
};

USTRUCT(BlueprintType)
struct FMyServerSearchResult {
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)
		FString Title;
	UPROPERTY(BlueprintReadWrite)
		FString Description;
	// TODO - add more data that we care about
	UPROPERTY(BlueprintReadWrite)
		FString key_id_str; // previous engine versions would always add .0 to int keys, so we pass them as strings as well.
	UPROPERTY(BlueprintReadWrite)
		int32 key_id; 
	UPROPERTY(BlueprintReadWrite)
		bool continuous_server_provisioned;
	// There are more fields avaialble here if you need them.  Cheack the API docs
};

USTRUCT(BlueprintType)
struct FMyServerSearchResults {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyServerSearchResult> servers;
};

// This is used to store and pass around character customizations
// You can set this up however you like, but it's best to use INT or Enum as the variable type
// Because this gets encoded and passed up to the backend.
USTRUCT(BlueprintType)
struct FMyAppearance {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
		int32 mesh;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
	//	int32 texture;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA", meta = (MakeStructureDefaultValue = 0))
	//	int32 animBP;
	// TODO - additional config options
};

// This is used to store all of the available cusomizations and it is set up in UEtopiaCompetitiveCharacterBP 
USTRUCT(BlueprintType)
struct FCharacterOptions {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UETOPIA")
		USkeletalMesh* mesh;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UETOPIA")
	//	TArray<UAnimBlueprintGeneratedClass*> animbps;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UETOPIA")
	//	TArray<UMaterial*> materials;
};