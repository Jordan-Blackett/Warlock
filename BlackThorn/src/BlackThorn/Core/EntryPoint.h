#pragma once

#ifdef BT_PLATFORM_WINDOWS

extern BlackThorn::Application* BlackThorn::CreateApplication();

int main(int argc, char** argv)
{
	BlackThorn::Log::Init();
	BT_CORE_WARN("Initialized Log!");

	// Network
	//std::thread TCPListenerThread;
	//std::thread UDPThread;
	//if (BlackThorn::Network::Initialize())
	//{
	//	BT_CORE_WARN("Initialized Network!");

	//	char localIP[10] = "127.0.0.1";
	//	TCPListenerThread = std::thread(BlackThorn::Network::InitializeTCP, localIP, PORT);
	//	UDPThread = std::thread(BlackThorn::Network::InitializeUDP, localIP, PORT);
	//}

	printf("BlackThorn Engine\n");
	auto app = BlackThorn::CreateApplication();
	app->Run();
	delete app;
}

#endif

