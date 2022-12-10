// Fill out your copyright notice in the Description page of Project Settings.


#include "MapSystem.h"

JavaVM* jvm;
JNIEnv* env;
jclass mainClass;
jmethodID mainMethod;
jmethodID addNormalPoint;
jmethodID addPredictPoint;

MapSystem::MapSystem()
{
	JavaVMInitArgs vmArgs;
	JavaVMOption* options = new JavaVMOption[1];
	options[0].optionString = _strdup("-Djava.class.path=C:\\Users\\erhan\\Desktop\\MapJar.jar");
	vmArgs.version = JNI_VERSION_1_6;
	vmArgs.nOptions = 1;
	vmArgs.options = options;
	vmArgs.ignoreUnrecognized = false;

	HINSTANCE hinstLib = LoadLibrary(TEXT("C:\\Program Files\\Java\\jdk-11.0.15\\bin\\server\\jvm.dll"));
	typedef jint(JNICALL* PtrCreateJavaVM)(JavaVM**, void**, void*);
	PtrCreateJavaVM ptrCreateJavaVM = (PtrCreateJavaVM)GetProcAddress(hinstLib, "JNI_CreateJavaVM");
	jint rc = ptrCreateJavaVM(&jvm, (void**)&env, &vmArgs);
	delete[] options;
	if (rc != JNI_OK) return;

	mainClass = env->FindClass("org/oscim/test/MapsforgeTest");
	mainMethod = env->GetStaticMethodID(mainClass, "main", "([Ljava/lang/String;)V");
	env->CallStaticVoidMethod(mainClass, mainMethod, NULL);

	addNormalPoint = env->GetStaticMethodID(mainClass, "addNormalPoint", "(FF)V");
	addPredictPoint = env->GetStaticMethodID(mainClass, "addPredictPoint", "(FF)V");


	thread = FRunnableThread::Create(this, TEXT("MapSystem"));
}

bool MapSystem::Init()
{
	FPlatformProcess::Sleep(2); // load map system
	FPortObserver::Attach(this);
	return true;
}

void MapSystem::Stop()
{
}
uint32 MapSystem::Run()
{
	return uint32();
}
void MapSystem::Update(const RocketData Data, EListenerType AvionicType, const float DTime)
{
	if ((float)Data.MainData.latitude != 0 && (float)Data.MainData.longitude != 0) 
		env->CallStaticVoidMethod(mainClass, addNormalPoint, (jfloat)Data.MainData.latitude, (jfloat)Data.MainData.longitude);
	if ((float)Data.PayloadData.latitude != 0 && (float)Data.PayloadData.longitude != 0) 
		env->CallStaticVoidMethod(mainClass, addPredictPoint, (jfloat)Data.PayloadData.latitude, (jfloat)Data.PayloadData.longitude);
}
