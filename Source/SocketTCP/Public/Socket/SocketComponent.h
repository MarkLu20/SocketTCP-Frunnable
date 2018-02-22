// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Runtime/Sockets/Public/Sockets.h"
#include  "Runtime/Networking/Public/Networking.h"
#include "Runnable.h"
#include "RunnableThread.h"
#include "SocketComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FrceiveData,FString,Data);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOCKETTCP_API USocketComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USocketComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EEndPlayReason)   override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FSocket *ListenSocek;
	UFUNCTION(BlueprintCallable, Category = "TCP")
		void StartTCPReceiver(const FString &YourChosenSocketName, const FString &IP, const int32 thePort, bool &success);
	UFUNCTION(BlueprintImplementableEvent, Category = "TCPServer")
		void RecieveMessage(const FString &message);
	
	FString StringFromBinaryArray(TArray<uint8> BinaryArray);
	UFUNCTION(BlueprintCallable, Category = "TCPServer")
		void TCPSocketListener();
	UFUNCTION(BlueprintCallable, Category = "TCPServer")
		void TCPSend(FString message);
	UPROPERTY(BlueprintAssignable, Category = "TCPServer")
		FrceiveData ReceiveDataEvent;
	UFUNCTION(BlueprintCallable, Category = "TCPServer")
		void ListenerFuntion();
		
	
};
class ReceiveThread :public FRunnable
{ public:
	FRunnableThread *MangeThread;
	USocketComponent *Object;
	

	~ReceiveThread()
	{
		MangeThread->WaitForCompletion();
		delete MangeThread;
		MangeThread = NULL;
	
	}
	ReceiveThread(USocketComponent *object)
	{
		Object = object;
	
	}





	// Í¨¹ý FRunnable ¼Ì³Ð
	virtual uint32 Run() override;
	void CreateThread(ReceiveThread *thread);

};