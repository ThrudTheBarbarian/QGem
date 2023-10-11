#include "aes.h"

AES::AES(QObject *parent)
	: QObject{parent}
	,_vdi(nullptr)
	,_nextApp(1)
	{}
