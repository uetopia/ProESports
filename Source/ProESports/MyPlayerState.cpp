// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPlayerState.h"
#include "Comp.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/ImageWrapper/Public/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/IImageWrapperModule.h"
#include "DDSLoader.h"
#include "DynamicTextureStaticMeshActor.h"
#include "MyGameInstance.h"




void AMyPlayerState::BroadcastChatMessage_Implementation(const FText& ChatMessageIn)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation "));

	// Use Game Instance because it does not get erased on level change
	// Get the game instance and cast to our game instance.
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation - get game instance "));
	UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

	// Get the activePlayer record
	FMyActivePlayer* playerRecord = TheGameInstance->getPlayerByPlayerId(PlayerId);

	//Check to see if it was a / command, in which case we can process it here.
	FString ChatMessageInString = ChatMessageIn.ToString();
	if (ChatMessageInString.StartsWith("/"))
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation - Found slash command "));
		if (ChatMessageInString.StartsWith("/balance"))
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation -  /balance  "));
			FString commandResponseSender = "SYSTEM";
			FText ChatSenderSYS = FText::FromString(commandResponseSender);

			FString playerBalance = FString::FromInt(playerRecord->currencyCurrent);
			FString commandResponse = "Balance: " + playerBalance;

			// old way
			//FString playerBalance = FString::FromInt(TheGameInstance->getPlayerBalanceByPlayerID(PlayerId));

			FText ChatMessageOut = FText::FromString(commandResponse);
			ReceiveChatMessage(ChatSenderSYS, ChatMessageOut);

		}
	}
	else
	{


		//Send the message upstream to the UETOPIA API
		//TheGameInstance->OutgoingChat(PlayerId, ChatMessageIn);

		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Looping over Player controllers "));
		// Trying the chatacter controller iterator instead of playerarray
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			// old way
			/*
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Casting to MyPlayerController "));
			AMyPlayerController* Controller = Cast<AMyPlayerController>(*Iterator);
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Getting PlayerState "));
			AMyPlayerState* pstate = Cast<AMyPlayerState>(Controller->PlayerState);
			// Go through the activelist to grab our playerName
			UMyGameInstance* TheGameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
			FString thePlayerTitle = TheGameInstance->getPlayerTitleByPlayerID(PlayerId);
			*/

			FText thePlayerName = FText::FromString(playerRecord->playerTitle);
			this->ReceiveChatMessage(thePlayerName, ChatMessageIn);

		}
	}


	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] BroadcastChatMessage_Implementation Done. "));
}

bool AMyPlayerState::BroadcastChatMessage_Validate(const FText& ChatMessageIn)
{
	UE_LOG(LogTemp, Log, TEXT("Validate"));
	return true;
}

void AMyPlayerState::ReceiveChatMessage_Implementation(const FText& ChatSender, const FText& ChatMessageD)
{
	// do stuff on client here
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] ReceiveChatMessage_Implementation "));
	//UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] ReceiveChatMessage_Implementation Broadcasting Delegate"));
	OnTextDelegate.Broadcast(ChatSender, ChatMessageD);


	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerState] ReceiveChatMessage_Implementation Done."));
}


void AMyPlayerState::OnRep_OnCustomTextureChange_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] OnRep_OnCustomTextureChange_Implementation."));

	// First check to see if they are already loaded.
	// Since we're in player state, this should trigger on travel from lobby to game levels.
	// WE don't need to actually load them again.  We just need to apply them.

	if (customTextures.Num() != LoadedTextures.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] OnRep_OnCustomTextureChange_Implementation.  customTextures.Num != LoadedTextures.Num"));
		// loop over the custom textures
		// Make sure valid filename was specified
		for (int32 b = 0; b < customTextures.Num(); b++) {
			if (customTextures[b].IsEmpty() || customTextures[b].Contains(TEXT(" ")))
			{
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] OnRep_OnCustomTextureChange_Implementation.  INVALID FILENAME"));
				customTextures[b] = "https://apitest-dot-ue4topia.appspot.com/img/groups/M_Banners_BaseColor.png";
			}

			// Create the Http request and add to pending request list
			FHttpModule* Http = &FHttpModule::Get();
			if (!Http) { return; }
			if (!Http->IsHttpEnabled()) { return; }

			TSharedRef < IHttpRequest > Request = Http->CreateRequest();
			Request->SetVerb("GET");
			Request->SetURL(customTextures[b]);
			Request->SetHeader("User-Agent", "UETOPIA_UE4_API_CLIENT/1.0");
			Request->SetHeader("Content-Type", "application/x-www-form-urlencoded");

			//TSharedRef<class IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
			//FileRequests.Add(&HttpRequest.Get(), FPendingFileRequest(FileName));

			//Request->OnProcessRequestComplete().BindUObject(this, delegateCallback);

			Request->OnProcessRequestComplete().BindUObject(this, &AMyPlayerState::ReadCustomTexture_HttpRequestComplete);
			//HttpRequest->SetURL(customTexture);
			//HttpRequest->SetVerb(TEXT("GET"));

			Request->ProcessRequest();

		}
	}

	return;
}

void AMyPlayerState::ReadCustomTexture_HttpRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete."));
	if (!HttpRequest.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete REQUEST INVALID."));
		return;
	}

	/*

	const FPendingFileRequest* PendingRequest = FileRequests.Find(HttpRequest.Get());

	if (PendingRequest == nullptr)
	{
	return;
	}


	*/
	bool bResult = false;
	FString ResponseStr, ErrorStr;

	/*

	// Cloud file being operated on
	{
	FCloudFile* CloudFile = GetCloudFile(PendingRequest->FileName, true);
	CloudFile->AsyncState = EOnlineAsyncTaskState::Failed;
	CloudFile->Data.Empty();
	}
	*/

	if (bSucceeded &&
		HttpResponse.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. bSucceeded and IsValid "));
		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			UE_LOG(LogTemp, Log, TEXT("ReadFile request complete. url=%s code=%d"),
				*HttpRequest->GetURL(), HttpResponse->GetResponseCode());

			// https://answers.unrealengine.com/questions/75929/is-it-possible-to-load-bitmap-or-jpg-files-at-runt.html

			// update the memory copy of the file with data that was just downloaded
			//FCloudFile* CloudFile = GetCloudFile(PendingRequest->FileName, true);

			// only tracking a single file....
			// const FString& InFileName
			const FString& CloudFileTitle = "GroupTexture";
			FCloudFile CloudFile = FCloudFile(CloudFileTitle);
			CloudFile.FileName = CloudFileTitle;

			CloudFile.AsyncState = EOnlineAsyncTaskState::Done;
			CloudFile.Data = HttpResponse->GetContent();

			// cache to disk on successful download
			SaveCloudFileToDisk(CloudFile.FileName, CloudFile.Data);

			IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
			// Note: PNG format.  Other formats are supported
			TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

			//FString TexturePath = FPaths::GameContentDir() + TEXT("../Saved/Cloud/") + CloudFileTitle;
			FString TexturePath = FPaths::CloudDir() + CloudFileTitle;
			UE_LOG(LogTemp, Warning, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. TexturePath: %s"), *TexturePath);
			TArray<uint8> FileData;

			if (FFileHelper::LoadFileToArray(FileData, *TexturePath))
			{
				UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. LoadFileToArray "));
				if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
				{
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. ImageWrapper->SetCompressed "));
					const TArray<uint8>* UncompressedBGRA = NULL;
					if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
					{
						UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. ImageWrapper->GetRaw "));
						int32 LoadedTextureIndex;
						LoadedTextureIndex = LoadedTextures.Add(UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8));

						/* Create transient texture */
						//LoadedTexture = UTexture2D::CreateTransient(DDSLoadHelper.DDSHeader->dwWidth, DDSLoadHelper.DDSHeader->dwHeight, Format);

						//LoadedTexture->MipGenSettings = TMGS_LeaveExistingMips; // apparently this is editor only functionality???
						LoadedTextures[LoadedTextureIndex]->PlatformData->NumSlices = 1;
						LoadedTextures[LoadedTextureIndex]->NeverStream = true;
						//LoadedTextures[LoadedTextureIndex]->Rename(*CloudFileTitle);

						/* Get pointer to actual data */
						//uint8* DataPtr = (uint8*)DDSLoadHelper.GetDDSDataPointer();

						//uint32 CurrentWidth = DDSLoadHelper.DDSHeader->dwWidth;
						//uint32 CurrentHeight = DDSLoadHelper.DDSHeader->dwHeight;

						//Copy!
						void* TextureData = LoadedTextures[LoadedTextureIndex]->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
						LoadedTextures[LoadedTextureIndex]->PlatformData->Mips[0].BulkData.Unlock();


						LoadedTextures[LoadedTextureIndex]->UpdateResource();
					}


				}

				// make sure all of the textures are loaded first.
				if (customTextures.Num() == LoadedTextures.Num())
				{
					UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] OnRep_OnCustomTextureChange_Implementation.  customTextures.Num == LoadedTextures.Num"));
					LoadTexturesOntoActors();
				}

			
			}

			bResult = true;
		}
		else
		{
			ErrorStr = FString::Printf(TEXT("Invalid response. code=%d error=%s"),
				HttpResponse->GetResponseCode(), *HttpResponse->GetContentAsString());

			UE_LOG(LogTemp, Log, TEXT("Invalid response. code=%d error=%sd"),
				HttpResponse->GetResponseCode(), *HttpRequest->GetURL());
		}
	}
	else
	{
		ErrorStr = TEXT("No response");
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ReadFile request failed. %s"), *ErrorStr);
	}

	//TriggerOnReadFileCompleteDelegates(bResult, PendingRequest->FileName);

	//FileRequests.Remove(HttpRequest.Get());
}

void AMyPlayerState::LoadTexturesOntoActors_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] LoadTexturesOntoActors_Implementation."));

	// now assign the textures to all of the banners
	// we want to loop over the actors first, 
	// and loop over the textures inside.
	// Actor1 - Texture1
	// Actor2 - Texture2
	// Actor3 - Texture1
	// Actor4 - Texture2
	// ActorIndex % len(textures) = TextureIndex  // I think...
	int32 ActorIndex = 0;
	int32 TextureIndex = 0;
	TActorIterator< ADynamicTextureStaticMeshActor > AllActorsItr = TActorIterator< ADynamicTextureStaticMeshActor >(GetWorld());

	//While not reached end (overloaded bool operator)
	while (AllActorsItr)
	{
		UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. Found ADynamicTextureStaticMeshActor "));

		if (AllActorsItr->MaterialInstance->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Log, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. Material Instance IsValid "));

			TextureIndex = ActorIndex % LoadedTextures.Num();
			UE_LOG(LogTemp, Warning, TEXT("[UETOPIA] [AMyPlayerController] ReadCustomTexture_HttpRequestComplete. TextureIndex: %d"), TextureIndex);

			AllActorsItr->MaterialInstance->SetTextureParameterValue(FName("BaseColor"), LoadedTextures[TextureIndex]);
		}

		//next actor
		++AllActorsItr;
		++ActorIndex;
	}
}

void AMyPlayerState::SaveCloudFileToDisk(const FString& Filename, const TArray<uint8>& Data)
{
	// save local disk copy as well
	FString LocalFilePath = GetLocalFilePath(Filename);
	bool bSavedLocal = FFileHelper::SaveArrayToFile(Data, *LocalFilePath);
	if (bSavedLocal)
	{
		UE_LOG(LogTemp, Verbose, TEXT("WriteUserFile request complete. Local file cache updated =%s"),
			*LocalFilePath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WriteUserFile request complete. Local file cache failed to update =%s"),
			*LocalFilePath);
	}
}

FString AMyPlayerState::GetLocalFilePath(const FString& FileName)
{
	return FPaths::CloudDir() + FPaths::GetCleanFilename(FileName);
}


void AMyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, playerTitle);
	DOREPLIFETIME(AMyPlayerState, serverTitle);
	//DOREPLIFETIME(AMyPlayerState, InventoryCubes);
	DOREPLIFETIME(AMyPlayerState, Currency);
	DOREPLIFETIME(AMyPlayerState, ServerPortalKeyIdsAuthorized);
	DOREPLIFETIME(AMyPlayerState, TeamId);
	DOREPLIFETIME(AMyPlayerState, customTextures);
}

/* handles copying properties when we do seamless travel */
void AMyPlayerState::CopyProperties(class APlayerState* PlayerState)
{
	Super::CopyProperties(PlayerState);

	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(PlayerState);

	if (MyPlayerState)
	{
		//MyPlayerState->InventoryCubes = InventoryCubes;
		MyPlayerState->playerKeyId = playerKeyId;
		MyPlayerState->Currency = Currency;
		MyPlayerState->TeamId = TeamId;
		MyPlayerState->playerTitle = playerTitle;
		MyPlayerState->customTextures = customTextures;
	}

}