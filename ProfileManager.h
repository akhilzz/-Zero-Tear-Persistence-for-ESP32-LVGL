#ifndef PROFILE_MANAGER_H
#define PROFILE_MANAGER_H

#include "AppTypes.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <Arduino.h>
#include <Preferences.h>


#define NVS_NAMESPACE "profiles"
#define NVS_KEY_BLOB "prof_blob"
#define NVS_KEY_COUNT "prof_count"
#define NVS_KEY_VERSION "nvs_ver"
#define NVS_VERSION_VAL 1

class ProfileManager {
public:
  ProfileManager();
  bool begin();

  // Data access
  bool loadProfiles(profile_t *profiles, int *count);
  bool saveProfiles(const profile_t *profiles, int count);
  bool forceSave(); // Immediate commit

private:
  SemaphoreHandle_t _lock;
  TaskHandle_t _saveTaskHandle;

  volatile bool _dirty;
  volatile uint32_t _lastChangeTime;

  profile_t _cachedProfiles[MAX_PROFILES];
  int _cachedCount;
  bool _cacheLoaded;
  bool isInitialized;

  static void saveTaskWorker(void *param);
  bool saveToNVS(const profile_t *profiles, int count);
};

extern ProfileManager Profiles;

#endif
