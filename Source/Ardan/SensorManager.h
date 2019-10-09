// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
//#include <librdkafka/rdkafka.h>

#include "Sensor.h"
/**
 * 
 */
class ARDAN_API SensorManager
{
public:
	SensorManager(UWorld * w, AActor * con);
	~SensorManager();

	void AddSensor(ASensor * sensor);

	void FindSensors();

	void ReceivePacket(struct pkt* pkt);
	void Replay();
	void SnapshotState(float timeStamp);

	void RewindState(float timeStamp);

	void FastForwardState(float timeStamp);

	void NewTimeline();

	void ResetTimeline();

	void ChangeTimeline(int index);

	bool DiffState(int index, float timeStamp);
	void CopyOutState(TMap<FString, FSensorHistory>* sensorHistory);
	void CopyInState(TMap<FString, FSensorHistory>* sensorHistory);
	void SetProducer(rd_kafka_t *rk, rd_kafka_topic_t *rkt);

	TMap<int, ASensor*> sensorTable;

private:
	UWorld* world;
	TArray<ASensor*> sensors;
	bool bHasHistory = false;
	AActor *controller;

	FColor colours[12] = { FColor(255, 0, 0),
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
		FColor(0,0,128) };
};
