#pragma once
#include <atomic>

//NONE = 0, don't send
#define ALERT_0	"ALERT0"
//PREVENTIVE 1,
#define ALERT_1	"ALERT1"
//CORRECTIVE = 2,
#define ALERT_2 "ALERT2"
//WARNING = 3
#define ALERT_3 "ALERT3"

//shared variables with thread
static inline std::atomic_bool theadEnds = false;

//alert thread proxy
void alertProxyTask(
	unsigned assessmentPort,
	std::string avoidanceAddress,
	unsigned avoidancePort);






