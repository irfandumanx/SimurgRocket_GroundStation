#include "SerialPort.h"

SerialPort::SerialPort(EListenerType AvionicType, char* com_port, DWORD COM_BAUD_RATE)
{
	this->AvionicType = AvionicType;
	connected_ = true;
	rocketData = static_cast<RocketData*>(calloc(1, sizeof(RocketData)));
	rocketData->MainData.rocketState = 1;
	if (AvionicType == MainAvionic) AvionicFile.open("C:\\Users\\erhan\\Desktop\\AvionicFile.txt", ios_base::app);
	else if(AvionicType == PayloadAvionic) PayloadFile.open("C:\\Users\\erhan\\Desktop\\PayloadFile.txt", ios_base::app);

	io_handler_ = CreateFileA(static_cast<LPCSTR>(com_port),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (io_handler_ == INVALID_HANDLE_VALUE) {

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			UE_LOG(LogTemp, Warning, TEXT("Warning: Handle was not attached. Reason: %s not available\n"), *FString(com_port));
	}
	else {

		DCB dcbSerialParams = { 0 };

		if (!GetCommState(io_handler_, &dcbSerialParams)) {

			UE_LOG(LogTemp, Warning, TEXT("Warning: Failed to get current serial params"));
		}

		else {
			dcbSerialParams.BaudRate = COM_BAUD_RATE;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(io_handler_, &dcbSerialParams)) {
				UE_LOG(LogTemp, Warning, TEXT("Warning: could not set serial port params\n"));
			}
			else {
				connected_ = true;
				PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
				thread = FRunnableThread::Create(this, TEXT("SERIALPORT"));
			}
		}
	}
}

vector<string> SerialPort::Split(string str, char seperator)
{
	vector<string> strings;
	int i = 0, startIndex = 0, endIndex = 0;
	while (i <= str.length())
	{
		if (str[i] == seperator || i == str.length())
		{
			endIndex = i;
			string subStr = "";
			subStr.append(str, startIndex, endIndex - startIndex);
			strings.push_back(subStr);
			startIndex = endIndex + 1;
		}
		i++;
	}
	return strings;
}

string SerialPort::ReadSerialPort(int replyWaitTime) {
	string compMsg;
	DWORD bytes_read;
	char inc_msg[1];
	bool began = false;
	unsigned long start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);

	while ((time(nullptr) - start_time) < replyWaitTime) {
		if (ReadFile(io_handler_, inc_msg, 1, &bytes_read, NULL)) {
			if (inc_msg[0] == '!') {
				began = true;
				continue;
			}
			if (began) {
				if (inc_msg[0] == '|') return compMsg;
				compMsg.push_back(inc_msg[0]);
			}
		}
		else return "ERROR";
	}
	return compMsg;
}

bool SerialPort::writeSerialPort(uint8_t data_sent)
{
	DWORD bytes_sent;

	unsigned int data_sent_length = sizeof(data_sent);
	std::cout << data_sent_length << std::endl;

	if (!WriteFile(io_handler_, &data_sent, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io_handler_, &errors_, &status_);
		return false;
	}
	else
		return true;
}

bool SerialPort::CloseSerialPort()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
		return true;
	}
	return false;
}

typedef union {
	float sayi;
	uint8_t array[4];
}FLOAT32_UINT8_DONUSTURUCU;

uint8_t olusturalacak_paket[78];

unsigned char check_sum_hesapla() {
	int check_sum = 0;

	for (int i = 4; i < 75; i++) {
		check_sum += olusturalacak_paket[i];
	}

	return (unsigned char)(check_sum % 256);
}
void SerialPort::SendDataRefereeStation(RocketData SendRocketData, uint8_t Counter)
{
	olusturalacak_paket[0] = 0xFF; // Sabit
	olusturalacak_paket[1] = 0xFF; // Sabit
	olusturalacak_paket[2] = 0x54; // Sabit
	olusturalacak_paket[3] = 0x52; // Sabit

	olusturalacak_paket[4] = 20; // Takim ID = 0
	olusturalacak_paket[5] = Counter; // SAYAC
	FLOAT32_UINT8_DONUSTURUCU roket_irtifa;
	roket_irtifa.sayi = SendRocketData.MainData.altitude;
	olusturalacak_paket[6] =
		roket_irtifa.array[0];
	olusturalacak_paket[7] =
		roket_irtifa.array[1];
	olusturalacak_paket[8] =
		roket_irtifa.array[2];
	olusturalacak_paket[9] =
		roket_irtifa.array[3];

	FLOAT32_UINT8_DONUSTURUCU roket_gps_irtifa_float32_uint8_donusturucu;
	roket_gps_irtifa_float32_uint8_donusturucu.sayi = SendRocketData.MainData.gpsAltitude;
	olusturalacak_paket[10] =
		roket_gps_irtifa_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[11] =
		roket_gps_irtifa_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[12] =
		roket_gps_irtifa_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[13] =
		roket_gps_irtifa_float32_uint8_donusturucu.array[3];

	if ((float)SendRocketData.MainData.latitude != 0)
	{
		FLOAT32_UINT8_DONUSTURUCU roket_enlem_float32_uint8_donusturucu;
		roket_enlem_float32_uint8_donusturucu.sayi = SendRocketData.MainData.latitude;
		olusturalacak_paket[14] =
			roket_enlem_float32_uint8_donusturucu.array[0];
		olusturalacak_paket[15] =
			roket_enlem_float32_uint8_donusturucu.array[1];
		olusturalacak_paket[16] =
			roket_enlem_float32_uint8_donusturucu.array[2];
		olusturalacak_paket[17] =
			roket_enlem_float32_uint8_donusturucu.array[3];
	}

	if ((float)SendRocketData.MainData.longitude != 0)
	{
		FLOAT32_UINT8_DONUSTURUCU
			roket_boylam_irtifa_float32_uint8_donusturucu;
		roket_boylam_irtifa_float32_uint8_donusturucu.sayi = SendRocketData.MainData.longitude;
		olusturalacak_paket[18] =
			roket_boylam_irtifa_float32_uint8_donusturucu.array[0];
		olusturalacak_paket[19] =
			roket_boylam_irtifa_float32_uint8_donusturucu.array[1];
		olusturalacak_paket[20] =
			roket_boylam_irtifa_float32_uint8_donusturucu.array[2];
		olusturalacak_paket[21] =
			roket_boylam_irtifa_float32_uint8_donusturucu.array[3];
	}

	FLOAT32_UINT8_DONUSTURUCU
		gorev_yuku_gps_irtifa_float32_uint8_donusturucu;
	gorev_yuku_gps_irtifa_float32_uint8_donusturucu.sayi = SendRocketData.PayloadData.gpsAltitude;
	olusturalacak_paket[22] =
		gorev_yuku_gps_irtifa_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[23] =
		gorev_yuku_gps_irtifa_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[24] =
		gorev_yuku_gps_irtifa_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[25] =
		gorev_yuku_gps_irtifa_float32_uint8_donusturucu.array[3];

	if ((float) SendRocketData.PayloadData.latitude != 0)
	{
		FLOAT32_UINT8_DONUSTURUCU gorev_yuku_enlem_float32_uint8_donusturucu;
		gorev_yuku_enlem_float32_uint8_donusturucu.sayi = SendRocketData.PayloadData.latitude;
		olusturalacak_paket[26] =
			gorev_yuku_enlem_float32_uint8_donusturucu.array[0];
		olusturalacak_paket[27] =
			gorev_yuku_enlem_float32_uint8_donusturucu.array[1];
		olusturalacak_paket[28] =
			gorev_yuku_enlem_float32_uint8_donusturucu.array[2];
		olusturalacak_paket[29] =
			gorev_yuku_enlem_float32_uint8_donusturucu.array[3];
	}

	if ((float)SendRocketData.PayloadData.longitude != 0) {
		FLOAT32_UINT8_DONUSTURUCU gorev_yuku_boylam_irtifa_float32_uint8_donusturucu;
		gorev_yuku_boylam_irtifa_float32_uint8_donusturucu.sayi = SendRocketData.PayloadData.longitude;
		olusturalacak_paket[30] =
			gorev_yuku_boylam_irtifa_float32_uint8_donusturucu.array[0];
		olusturalacak_paket[31] =
			gorev_yuku_boylam_irtifa_float32_uint8_donusturucu.array[1];
		olusturalacak_paket[32] =
			gorev_yuku_boylam_irtifa_float32_uint8_donusturucu.array[2];
		olusturalacak_paket[33] =
			gorev_yuku_boylam_irtifa_float32_uint8_donusturucu.array[3];
	}

	FLOAT32_UINT8_DONUSTURUCU kademe_gps_irtifa_float32_uint8_donusturucu;
	kademe_gps_irtifa_float32_uint8_donusturucu.sayi = 0;
	olusturalacak_paket[34] =
		kademe_gps_irtifa_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[35] =
		kademe_gps_irtifa_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[36] =
		kademe_gps_irtifa_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[37] =
		kademe_gps_irtifa_float32_uint8_donusturucu.array[3];

	FLOAT32_UINT8_DONUSTURUCU kademe_enlem_float32_uint8_donusturucu;
	kademe_enlem_float32_uint8_donusturucu.sayi = 0;
	olusturalacak_paket[38] =
		kademe_enlem_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[39] =
		kademe_enlem_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[40] =
		kademe_enlem_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[41] =
		kademe_enlem_float32_uint8_donusturucu.array[3];

	FLOAT32_UINT8_DONUSTURUCU
		kademe_boylam_irtifa_float32_uint8_donusturucu;
	kademe_boylam_irtifa_float32_uint8_donusturucu.sayi = 0; // Kademe boylam degerinin atamasini yapiyoruz.
	olusturalacak_paket[42] =
		kademe_boylam_irtifa_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[43] =
		kademe_boylam_irtifa_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[44] =
		kademe_boylam_irtifa_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[45] =
		kademe_boylam_irtifa_float32_uint8_donusturucu.array[3];

	FLOAT32_UINT8_DONUSTURUCU roll;
	roll.sayi = SendRocketData.MainData.x;
	olusturalacak_paket[46] = roll.array[0];
	olusturalacak_paket[47] = roll.array[1];
	olusturalacak_paket[48] = roll.array[2];
	olusturalacak_paket[49] = roll.array[3];

	FLOAT32_UINT8_DONUSTURUCU yaw;
	yaw.sayi = SendRocketData.MainData.y;
	olusturalacak_paket[50] = yaw.array[0];
	olusturalacak_paket[51] = yaw.array[1];
	olusturalacak_paket[52] = yaw.array[2];
	olusturalacak_paket[53] = yaw.array[3];

	FLOAT32_UINT8_DONUSTURUCU pitch;
	pitch.sayi = SendRocketData.MainData.z;
	olusturalacak_paket[54] = pitch.array[0];
	olusturalacak_paket[55] = pitch.array[1];
	olusturalacak_paket[56] = pitch.array[2];
	olusturalacak_paket[57] = pitch.array[3];

	FLOAT32_UINT8_DONUSTURUCU ivme_x_float32_uint8_donusturucu;
	ivme_x_float32_uint8_donusturucu.sayi = SendRocketData.MainData.accX;
	olusturalacak_paket[58] = ivme_x_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[59] = ivme_x_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[60] = ivme_x_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[61] = ivme_x_float32_uint8_donusturucu.array[3];

	FLOAT32_UINT8_DONUSTURUCU ivme_y_float32_uint8_donusturucu;
	ivme_y_float32_uint8_donusturucu.sayi = SendRocketData.MainData.accY;
	olusturalacak_paket[62] = ivme_y_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[63] = ivme_y_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[64] = ivme_y_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[65] = ivme_y_float32_uint8_donusturucu.array[3];

	FLOAT32_UINT8_DONUSTURUCU ivme_z_float32_uint8_donusturucu;
	ivme_z_float32_uint8_donusturucu.sayi = SendRocketData.MainData.accZ;
	olusturalacak_paket[66] = ivme_z_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[67] = ivme_z_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[68] = ivme_z_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[69] = ivme_z_float32_uint8_donusturucu.array[3];

	FLOAT32_UINT8_DONUSTURUCU aci_float32_uint8_donusturucu;
	aci_float32_uint8_donusturucu.sayi = SendRocketData.MainData.y;
	olusturalacak_paket[70] = aci_float32_uint8_donusturucu.array[0];
	olusturalacak_paket[71] = aci_float32_uint8_donusturucu.array[1];
	olusturalacak_paket[72] = aci_float32_uint8_donusturucu.array[2];
	olusturalacak_paket[73] = aci_float32_uint8_donusturucu.array[3];

	olusturalacak_paket[74] = SendRocketData.MainData.rocketState;
	olusturalacak_paket[75] = check_sum_hesapla();
	olusturalacak_paket[76] = 0x0D; // Sabit
	olusturalacak_paket[77] = 0x0A; // Sabit
	for (int i = 0; i < 78; i++) {
		writeSerialPort(olusturalacak_paket[i]);
	}
}

bool SerialPort::Init()
{
	bRunThread = true;
	return true;
}
uint8_t Counter = 0;
uint32 SerialPort::Run()
{
	while (bRunThread)
	{

		if (AvionicType == RefereeStation)
		{
			if (Counter == 255) Counter = 0;
			SendDataRefereeStation(StationRocketData, ++Counter);
			FPlatformProcess::Sleep(0.1);
			continue;
		}
		string msg = ReadSerialPort(2);
		vector<string> strings = Split(msg, '*');
		try {
			if (AvionicType == MainAvionic)
			{
				if (strings.size() == 13)
				{
					rocketData->MainData.altitude = FCString::Atof(*FString(strings.at(0).c_str()));
					rocketData->MainData.x = FCString::Atof(*FString(strings.at(1).c_str()));
					rocketData->MainData.y = FCString::Atof(*FString(strings.at(2).c_str()));
					rocketData->MainData.z = FCString::Atof(*FString(strings.at(3).c_str()));
					rocketData->MainData.accX = FCString::Atof(*FString(strings.at(4).c_str()));
					rocketData->MainData.accY = FCString::Atof(*FString(strings.at(5).c_str()));
					rocketData->MainData.accZ = FCString::Atof(*FString(strings.at(6).c_str()));
					rocketData->MainData.latitude = FCString::Atof(*FString(strings.at(7).c_str()));
					rocketData->MainData.longitude = FCString::Atof(*FString(strings.at(8).c_str()));
					rocketData->MainData.gpsAltitude = FCString::Atof(*FString(strings.at(9).c_str()));
					rocketData->MainData.rocketState = FCString::Atof(*FString(strings.at(10).c_str()));
					rocketData->MainData.angle = FCString::Atof(*FString(strings.at(11).c_str()));
					rocketData->MainData.parity = static_cast<uint8_t>(FCString::Atoi(*FString(strings.at(12).c_str())));
					StationRocketData.MainData.altitude = rocketData->MainData.altitude;
					StationRocketData.MainData.x = rocketData->MainData.x;
					StationRocketData.MainData.y = rocketData->MainData.y;
					StationRocketData.MainData.z = rocketData->MainData.z;
					StationRocketData.MainData.accX = rocketData->MainData.accX;
					StationRocketData.MainData.accY = rocketData->MainData.accY;
					StationRocketData.MainData.accZ = rocketData->MainData.accZ;
					StationRocketData.MainData.latitude = rocketData->MainData.latitude;
					StationRocketData.MainData.longitude = rocketData->MainData.longitude;
					StationRocketData.MainData.gpsAltitude = rocketData->MainData.gpsAltitude;
					StationRocketData.MainData.rocketState = rocketData->MainData.rocketState;
					StationRocketData.MainData.angle = rocketData->MainData.angle;
					StationRocketData.MainData.parity = rocketData->MainData.parity;
				}
			}
			else if (AvionicType == PayloadAvionic)
			{
				if (strings.size() == 7)
				{
					rocketData->PayloadData.temperature = stof(strings.at(0));
					rocketData->PayloadData.altitude = stof(strings.at(1));
					rocketData->PayloadData.humidity = stof(strings.at(2));
					rocketData->PayloadData.latitude = stof(strings.at(3));
					rocketData->PayloadData.longitude = stof(strings.at(4));
					rocketData->PayloadData.gpsAltitude = stof(strings.at(5));
					rocketData->PayloadData.parity = stof(strings.at(6));
					StationRocketData.PayloadData.temperature = rocketData->PayloadData.temperature;
					StationRocketData.PayloadData.altitude = rocketData->PayloadData.altitude;
					StationRocketData.PayloadData.humidity = rocketData->PayloadData.humidity;
					StationRocketData.PayloadData.latitude = rocketData->PayloadData.latitude;
					StationRocketData.PayloadData.longitude = rocketData->PayloadData.longitude;
					StationRocketData.PayloadData.gpsAltitude = rocketData->PayloadData.gpsAltitude;
					StationRocketData.PayloadData.parity = rocketData->PayloadData.parity;
					PayloadFile << msg << endl;
				}
			}
		}
		catch (...) {
			continue;
		}
		if (AvionicType == MainAvionic) AvionicFile << msg << endl;
		else if (AvionicType == PayloadAvionic) PayloadFile << msg << endl;
		FPortObserver::Notify(*rocketData, AvionicType);
	}

	return 0;
}

void SerialPort::Stop()
{
	UE_LOG(LogTemp, Warning, TEXT("Thread Stop"));

	bRunThread = false;
}

SerialPort::~SerialPort()
{
	if (connected_) 
	{
		thread->Kill();
		delete thread;
		AvionicFile.close();
		PayloadFile.close();
		connected_ = false;
		CloseHandle(io_handler_);
	}
}