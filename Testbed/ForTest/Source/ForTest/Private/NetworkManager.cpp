// Fill out your copyright notice in the Description page of Project Settings.

#include "NetworkManager.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"

// Sets default values
ANetworkManager::ANetworkManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void ANetworkManager::BeginPlay()
{
	Super::BeginPlay();
	StartNetworkFunction();
}

// Called every frame
void ANetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ANetworkManager::StartNetworkFunction()
{
	if (!StartTCPReceiver("RamaSocketListener"))
	{
		//UE_LOG  "TCP Socket Listener Created!"
		return;
	}

	//UE_LOG  "TCP Socket Listener Created! Yay!"
}

bool ANetworkManager::StartTCPReceiver(const FString& YourChosenSocketName) 
{
	listenerSocket = CreateTCPConnectionListener(YourChosenSocketName);

	//Not created?
	if (!listenerSocket)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("StartTCPReceiver>> Listen socket could not be created! ~> %s %d"), *TheIP, ThePort));
		LOG_NORMAL(Warning, TEXT("Lisiten SockerError!"));
		return false;
	}

	//Start the Listener! //thread this eventually
	GetWorldTimerManager().SetTimer(listenTimerHandle, &ANetworkManager::TCPConnectionListener, 0.01, true);
	return true;
}

bool ANetworkManager::FormatIP4ToNumber(uint8(&Out)[4])
{
	FString TheIP;

	//IP Formatting
	TheIP.Replace(TEXT(" "), TEXT(""));

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						   IP 4 Parts

	//String Parts
	TArray<FString> Parts;
	//TheIP.ParseIntoArray(&Parts, TEXT("."), true);
	TheIP.ParseIntoArray(Parts, TEXT("."), true);
	if (Parts.Num() != 4)
		return false;

	//String to Number Parts
	for (int32 i = 0; i < 4; ++i)
	{
		Out[i] = FCString::Atoi(*Parts[i]);
	}

	return true;
}

FSocket* ANetworkManager::CreateTCPConnectionListener(const FString& YourChosenSocketName, const int32 ReceiveBufferSize)
{
	uint8 IP4Nums[4];
	int32 ThePort;

	if (!FormatIP4ToNumber(IP4Nums))
	{
		LOG_NORMAL(Warning, TEXT("INVAILD IP!"));
		return false;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create Socket
	FIPv4Endpoint Endpoint(FIPv4Address(IP4Nums[0], IP4Nums[1], IP4Nums[2], IP4Nums[3]), ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	//Set Buffer Size
	int32 NewSize = 0;
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);

	//Done!
	return ListenSocket;
}

void ANetworkManager::TCPConnectionListener()
{
	//~~~~~~~~~~~~~
	if (!listenerSocket) return;
	//~~~~~~~~~~~~~

	//Remote address
	TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	bool Pending;

	// handle incoming connections
	if (listenerSocket->HasPendingConnection(Pending) && Pending)
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//Already have a Connection? destroy previous
		if (connectionSocket)
		{
			connectionSocket->Close();
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(connectionSocket);
		}
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		//New Connection receive!
		connectionSocket = listenerSocket->Accept(*RemoteAddress, TEXT("RamaTCP Received Socket Connection"));

		if (connectionSocket != NULL)
		{
			//Global cache of current Remote Address
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);

			//UE_LOG "Accepted Connection! WOOOHOOOO!!!";

			//can thread this too
			GetWorldTimerManager().SetTimer(acceptTimerHandle, &ANetworkManager::TCPSocketListener, 0.01, true);
		}
	}
}

FString ANetworkManager::StringFromBinaryArray(const TArray<uint8>& BinaryArray)
{
	//BinaryArray.Add(0); // Add 0 termination. Even if the string is already 0-terminated, it doesn't change the results.
						// Create a string from a byte array. The string is expected to be 0 terminated (i.e. a byte set to 0).
						// Use UTF8_TO_TCHAR if needed.
						// If you happen to know the data is UTF-16 (USC2) formatted, you do not need any conversion to begin with.
						// Otherwise you might have to write your own conversion algorithm to convert between multilingual UTF-16 planes.
	return FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(BinaryArray.GetData())));
}

void ANetworkManager::TCPSocketListener()
{
	//~~~~~~~~~~~~~
	if (!connectionSocket) return;
	//~~~~~~~~~~~~~

	//Binary Array!
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (connectionSocket->HasPendingData(Size))
	{
		ReceivedData.Init(FMath::Min(Size, 65507u));

		int32 Read = 0;
		connectionSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Read! %d"), ReceivedData.Num()));
	}
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	if (ReceivedData.Num() <= 0)
	{
		//No Data Received
		return;
	}

	LOG_NORMAL(Warning, "Total Data read! %d", ReceivedData.Num());
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Data Bytes Read ~> %d"), ReceivedData.Num()));

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//						Rama's String From Binary Array
	const FString ReceivedUE4String = StringFromBinaryArray(ReceivedData);
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	LOG_NORMAL(Warning, "As String!!!!! ~> %s", ReceivedUE4String);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("As String Data ~> %s"), *ReceivedUE4String));
}
