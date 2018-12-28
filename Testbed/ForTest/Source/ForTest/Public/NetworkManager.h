// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ForTest.h"
#include "Networking.h"
#include "GameFramework/Actor.h"
#include "NetworkManager.generated.h"

UCLASS()
class FORTEST_API ANetworkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FSocket* listenerSocket;
	FSocket* connectionSocket;
	FIPv4Endpoint RemoteAddressForConnection;

	FTimerHandle listenTimerHandle;
	FTimerHandle acceptTimerHandle;

public:
	void StartNetworkFunction();

protected:
	bool StartTCPReceiver(const FString& YourChosenSocketName);

	FSocket* CreateTCPConnectionListener(const FString& YourChosenSocketName, const int32 ReceiveBufferSize = 2 * 1024 * 1024);
	bool FormatIP4ToNumber( uint8(&Out)[4] );

	//Timer functions, could be threads
	void TCPConnectionListener(); 	//can thread this eventually
	void TCPSocketListener();					//can thread this eventually

	//Rama's StringFromBinaryArray
	FString StringFromBinaryArray(const TArray<uint8>& BinaryArray);
};
