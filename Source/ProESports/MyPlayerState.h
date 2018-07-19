// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "Online.h"
#include "Engine.h"
#include "MyPlayerState.generated.h"

/**
*
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
struct FMyInventoryItem {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		int32 quantity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		FString itemTitle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyInventoryItemStat> InventoryItemStats;

};

USTRUCT(BlueprintType)
struct FMyInventory {

	GENERATED_USTRUCT_BODY()
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UETOPIA")
		TArray<FMyInventoryItem> InventoryItems;
};

//THis delegate is working.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTextDelegate, FText, chatSender, FText, chatMessage);




UCLASS()
class PROESPORTS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

		FMyInventory Inventory;

	virtual void CopyProperties(class APlayerState* PlayerState) override;


public:
	// This is the function that gets called in the widget Blueprint
	UFUNCTION(BlueprintCallable, Category = "UETOPIA", Server, Reliable, WithValidation)
		void BroadcastChatMessage(const FText& ChatMessageIn);

	// This function is called remotely by the server.
	UFUNCTION(NetMulticast, Category = "UETOPIA", Unreliable)
		void ReceiveChatMessage(const FText& ChatSender, const FText& ChatMessage);


	UPROPERTY(BlueprintAssignable)
		FTextDelegate OnTextDelegate;

	/** List of online friends 
	struct FOnlineFriendsList
	{
		TArray< TSharedRef<FOnlineFriendUEtopia> > Friends;
	};
	*/

	UPROPERTY(BlueprintReadOnly)
		FString playerIdUEInternal;

	UPROPERTY(BlueprintReadOnly)
		FString playerKeyId;
	UPROPERTY(Replicated, BlueprintReadOnly)
		FString playerTitle;
	UPROPERTY(Replicated, BlueprintReadOnly)
		FString serverTitle;
	// Current Inventory item count - cubes
	// Since this demo is so simple we don't need to use the full inventory TArray.
	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 InventoryCubes;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 Currency;

	UPROPERTY(Replicated, BlueprintReadOnly)
		int32 TeamId;

	UPROPERTY(Replicated, BlueprintReadOnly)
		FString teamTitle;

	// Variables set via get game player api call

	// doubles are not supported.
	//UPROPERTY(BlueprintReadOnly)
	double savedCoordLocationX;
	//UPROPERTY(BlueprintReadOnly)
	double savedCoordLocationY;
	//UPROPERTY(BlueprintReadOnly)
	double savedCoordLocationZ;

	UPROPERTY(BlueprintReadOnly)
		FString savedZoneName;
	UPROPERTY(BlueprintReadOnly)
		FString savedZoneKey;

	UPROPERTY(BlueprintReadOnly)
		FString savedInventory;
	UPROPERTY(BlueprintReadOnly)
		FString savedEquipment;
	UPROPERTY(BlueprintReadOnly)
		FString savedAbilities;
	UPROPERTY(BlueprintReadOnly)
		FString savedInterface;

	// Authorizations for servers go in here.  These are server Key Ids.
	UPROPERTY(Replicated, BlueprintReadOnly)
		TArray<FString> ServerPortalKeyIdsAuthorized;

	// Custom texture url strings.
	// THis is set via getServerInfo, and normally stored in gamestate or game instance
	// the duplicate here is because the player needs to have a hard notification that this changes.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing = OnRep_OnCustomTextureChange, Category = "UETOPIA")
		TArray<FString> customTextures;

	UFUNCTION(Client, Reliable)
		virtual void OnRep_OnCustomTextureChange();

	UFUNCTION(Client, Reliable)
		virtual void LoadTexturesOntoActors();

	TArray<UTexture2D*> LoadedTextures;

	/**
	* Delegate called when a Http request completes for reading a cloud file
	*/
	void ReadCustomTexture_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	/**
	* Save a file from a given user to disk
	*
	* @param FileName name of file being saved
	* @param Data data to write to disk
	*/
	void SaveCloudFileToDisk(const FString& Filename, const TArray<uint8>& Data);

	/**
	* Converts filename into a local file cache path
	*
	* @param FileName name of file being loaded
	*
	* @return unreal file path to be used by file manager
	*/
	FString GetLocalFilePath(const FString& FileName);

};
