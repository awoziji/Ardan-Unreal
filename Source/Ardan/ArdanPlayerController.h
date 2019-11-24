// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "IPAddress.h"
#include "IPv4SubnetMask.h"
#include "IPv4Address.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include <flatbuffers/flatbuffers.h>
#include "RunnableConnection.h"
#include "Sensor.h"
#include "SensorManager.h"
#include "TimeSphere.h"
#include "ParticleDefinitions.h"
#if PLATFORM_WINDOWS
#include "WindowsHWrapper.h"
#include "AllowWindowsPlatformTypes.h"
#endif
//#include <cppkafka/producer.h>
#include <librdkafka/rdkafka.h>
#if PLATFORM_WINDOWS
#include "HideWindowsPlatformTypes.h"
#endif
#include "flatbuffers/c/unrealpkts_generated.h"
#include "ActorManager.h"
#include "GameFramework/PlayerController.h"
#include "ArdanPlayerController.generated.h"



UCLASS()
class AArdanPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	AArdanPlayerController();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Network)
	int32 port = 5011;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Network)
	FString address = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Time)
	TArray<AStaticMeshActor*> ReplayActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = actors)
	TArray<APawn*> pawnActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = fire)
	TArray<FVector> firePoints;

protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	bool reset = false;
	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PlayerTick(float DeltaTime) override;
	void LoadArdanState();
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor();

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();
	void Record();
	void ScrollUp();
	void ScrollDown();
	void StartAFire();
	void Pause();
	void NextCamera();
	void update_sensors();
	void SaveArdanState();
	void reversePressed();
	void reverseReleased();
	void speedSlow();
	void speedNormal();
	void speedFast();
	
	void NewTimeline();
	
	void replayPressed();

	void JumpForwardPressed();

	void JumpBackwardPressed();


	private:
		SensorManager* sensorManager;
		UPROPERTY()
		UActorManager*	actorManager;
		TArray<ATimeSphere*> timeSpheres;

		float replayTime = 0;
		float curTime = 0;
		bool bReplayHistory = false;
		bool bReverse = false;
		bool bReplay = false;
		bool bRecording = false;
		int currentCam = 0;
		TArray<AActor*> cameras;
		const float SmoothBlendTime = 0.75f;

		bool slow = false;
		float elapsed = 0;
		float tenth = 0;
		float rtick = 0;
		int tickCount = 0;
		ISocketSubsystem* sockSubSystem;
		FSocket* socket;
		FIPv4Address ip;
		TSharedPtr<FInternetAddr> addr;
		flatbuffers::FlatBufferBuilder fbb;
		int32 RecvSize = 0x8000;
		int32 SendSize = 0x8000;

		FRunnableConnection *conns;
		TQueue<rd_kafka_message_t *, EQueueMode::Spsc> packetQ;
		rd_kafka_t *rk;         /* Producer instance handle */
		rd_kafka_topic_t *rkt;  /* Topic object */
		rd_kafka_t *sensorrk;         /* Producer instance handle */
		rd_kafka_topic_t *sensorrkt;  /* Topic object */

		FColor colours[12] = {FColor(255, 0, 0),
													FColor(0, 255, 0),
													FColor(0, 0, 255),
													FColor(255,255,0),
													FColor(0,255,255),
													FColor(255,0,255),
													FColor(192,192,192),
													FColor(128,0,0),
													FColor(0,128,0),
													FColor(128,0,128),
													FColor(0,128,128),
													FColor(0,0,128)};
};
