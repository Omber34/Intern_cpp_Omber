#include "task.h"
#include <iostream>
int main()
{
	srand(time(NULL));
	Task t1("test_json.json");
	t1.writeResult();
    return 0;
}