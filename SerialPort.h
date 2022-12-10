#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
/*
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>
*/

enum EListenerType
{
	MainAvionic,
	PayloadAvionic,
	RefereeStation
};

typedef struct MainAvionicData
{
	float gpsAltitude;
	float latitude;
	float longitude;
	float altitude;
	float x;
	float y;
	float z;
	float accX;
	float accY;
	float accZ;
	float angle;
	uint8_t rocketState;
	uint8_t parity;
} MainAvionicData;

typedef struct PayloadAvionicData
{
	float temperature;
	float latitude;
	float humidity;
	float gpsAltitude;
	float longitude;
	float altitude;
	uint8_t parity;
} PayloadAvionicData;

typedef struct RocketData
{
	MainAvionicData MainData;
	PayloadAvionicData PayloadData;
} RocketData;

class FPortListener {
public:
	virtual ~FPortListener() {}
	virtual void Update(const RocketData NowData, EListenerType AvionicType, const float DTime) = 0;
};

class FPortObserver {
public:
	inline static bool FirstData = false;
	static void Attach(FPortListener* PortListener) { Listeners.Add(PortListener); }
	static void Detach(FPortListener* PortListener) { Listeners.Remove(PortListener); }

		
	static void Notify(const RocketData Data, EListenerType AvionicType)
	{
		float DTime = -1;
		if (!FirstData) {
			PrevTime = clock();
			FirstData = true;
		}

		const long NowTime = clock();
		DTime = static_cast<float>(NowTime - PrevTime) / CLOCKS_PER_SEC;
		PrevTime = NowTime;

		for (const auto Listener : Listeners)
		{
			FFunctionGraphTask::CreateAndDispatchWhenReady([Listener, Data, DTime, AvionicType]() {
				Listener->Update(Data, AvionicType, DTime);
			}	
			, TStatId(), nullptr, ENamedThreads::GameThread);
		}
	}
private:
	inline static TArray<FPortListener*> Listeners;
	inline static clock_t PrevTime = 0;
};


using namespace std;
class ALLAHIMINSALLAH_API SerialPort : public FRunnable
{
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;
	vector<string> Split(string str, char seperator);
	FRunnableThread* thread;
	bool bRunThread;
	bool connected_;
	RocketData* rocketData;
	EListenerType AvionicType;
	ofstream AvionicFile;
	ofstream PayloadFile;

public:
	static inline RocketData StationRocketData = *static_cast<RocketData*>(calloc(1, sizeof(RocketData)));;
	SerialPort(EListenerType AvionicType, char* com_port, DWORD COM_BAUD_RATE);
	SerialPort(SerialPort& other) = delete;
	void operator=(const SerialPort&) = delete;
	~SerialPort() override;
	string ReadSerialPort(int replyWaitTime);
	bool writeSerialPort(uint8_t data_sent);
	bool CloseSerialPort();
	bool GetConnected() { return connected_; }
	void SendDataRefereeStation(RocketData rocketData, uint8_t Counter);
	// Overriden from FRunnable
	virtual bool Init() override; // Do your setup here, allocate memory, ect.
	virtual uint32 Run() override; // Main data processing happens here
	virtual void Stop() override; // Clean up any memory you allocated here

};
