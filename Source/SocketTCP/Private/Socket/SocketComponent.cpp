// Fill out your copyright notice in the Description page of Project Settings.

#include "SocketComponent.h"
#include "string"


// Sets default values for this component's properties
USocketComponent::USocketComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USocketComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void USocketComponent::EndPlay(const EEndPlayReason::Type EEndPlayReason)
{
	Super::EndPlay(EEndPlayReason);
	if (ListenSocek!=NULL)
	{
		ListenSocek->Close();
		
	}
}


// Called every frame
void USocketComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USocketComponent::StartTCPReceiver(const FString & YourChosenSocketName, const FString & IP, const int32 thePort, bool & success)
{
	TSharedRef<FInternetAddr> targetAddres = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	FIPv4Address Addr;
	FIPv4Address::Parse(IP, Addr);
	int uiIp = (127 << 24) | 0 | 0 | 1;
	targetAddres->SetIp(uiIp);
	targetAddres->SetPort(thePort);
	FIPv4Endpoint ipadd(targetAddres);
	ListenSocek = FTcpSocketBuilder(TEXT("FTcpMessageTransport.RemoteConnection"));
	if (ListenSocek&&ListenSocek->Connect(ipadd.ToInternetAddr().Get())) {}

	{

	}

}

FString USocketComponent::StringFromBinaryArray(TArray<uint8> BinaryArray)
{
	std::string cstr(reinterpret_cast<const char*>(BinaryArray.GetData()), BinaryArray.Num());

	 FString frameAsFString = cstr.c_str();
	 return frameAsFString;
	
}
	


void USocketComponent::TCPSocketListener()
{
	if (!ListenSocek)
	{
		return;
	}
	TArray<uint8> ReceiveData;
	uint32 Size;
	while (ListenSocek->HasPendingData(Size))
	{
		ReceiveData.Init(FMath::Min<uint32>(Size,65507u),Size);
		int32 Read = 0;
		ListenSocek->Recv(ReceiveData.GetData(), ReceiveData.Num(), Read);
	}
	if (ReceiveData.Num()<=0)
	{
		return;
	}
	 FString ReceivedUE4String = StringFromBinaryArray(ReceiveData);
	ReceiveDataEvent.Broadcast(ReceivedUE4String);
	UE_LOG(LogTemp,Warning,TEXT("%s"),*ReceivedUE4String);
	
	
	//RecieveMessage(ReceivedUE4String);
}

void USocketComponent::TCPSend(FString message)
{
	TCHAR *SerializedChar = message.GetCharArray().GetData();
	int32 Size = FCString::Strlen(SerializedChar);
	int32 Sent = 0;
	uint8 *ResultChars = (uint8*)TCHAR_TO_UTF8(SerializedChar);
	if (!ListenSocek->Send(ResultChars, Size, Sent)) {};
	{

	}
}

void USocketComponent::ListenerFuntion()
{
	ReceiveThread *thread;
	thread = new ReceiveThread(this);
	thread->CreateThread(thread);
	delete thread;
	thread = NULL;
}

uint32 ReceiveThread::Run()
{


	Object->TCPSocketListener();


	
	
	return 0;
}

void ReceiveThread::CreateThread(ReceiveThread * thread)
{
	MangeThread = FRunnableThread::Create(thread,TEXT("ReceiveThread"));
}
