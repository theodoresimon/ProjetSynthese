#include "runSchedule_RunSchedule.h"
#include "algo.h"
#include "instance.h"
#include "schedule.h"
#include  <string.h>


JNIEXPORT void JNICALL Java_runSchedule_RunSchedule_runSchedule
  (JNIEnv * env, jobject obj, jstring instanceInFile, jstring scheduleOutFile, jint machines, jint preemption, jint balanced) {
  
	const char * filenameInstanceIn = (*env)->GetStringUTFChars(env, instanceInFile, NULL);
	const char * filenameScheduleOut = (*env)->GetStringUTFChars(env, scheduleOutFile, NULL);

	Instance instance = read_instance(filenameInstanceIn);

	struct schedule_t * schedule = create_schedule(instance, machines, preemption, balanced);
	
	char* convert = strdup(filenameScheduleOut);
	save_schedule(schedule, convert);

}
